#include "FileSystem.hpp"
#include "Partition.hpp"
#include "random.hpp"
#include "memory.hpp"
#include "time.hpp"
#include "KernelOut.hpp"
#include "wait.hpp"
#include "kernelMem.hpp"
#include "checksum.hpp"
#include "stddef.h"
#include "process.hpp"

namespace Kernel {

EXT4::EXT4(Device* device, uint8_t partitionID): device(device), partitionID(partitionID) {
    partition = getPartition(*device, partitionID);
    superBlock = new SuperBlock();
    device->read(partition.LBAStart + 2, 2, (uint8_t*)superBlock);
    if(!superBlock->s_feature_incompat & 0x80) { //64 bit
        superBlock->s_desc_size = 32;
    }
    sectorsPerBlock = (1 << (10 + superBlock->s_log_block_size)) / 512 /*sector size*/;
    blockSize = (1 << (10 + superBlock->s_log_block_size));

    inodeBitmapSize = superBlock->s_inodes_per_group / 8; // every inode is one bit
    blockBitmapSize = superBlock->s_blocks_per_group / 8; // every block is one bit

    lastBlockBitmapIndex = 0;
    lastBlockBitmap = nullptr;
}

uint32_t EXT4::generateINodeChecksum(INode node, uint64_t inodeNum) {
    uint8_t* buffer = (uint8_t*)&node;
    constexpr auto EXT4_GOOD_OLD_INODE_SIZE = 128;
    uint32_t csum = 0;

    uint16_t dummy_csum = 0;
    int offset = offsetof(INode, osd2.linux2.l_i_checksum_lo);
    unsigned int csum_size = sizeof(dummy_csum);

    uint32_t csumSeed = 0;
    
    csumSeed = crc32c(0, superBlock->s_uuid, sizeof(superBlock->s_uuid));
    csumSeed = crc32c(csumSeed, (uint8_t*)&inodeNum, sizeof(uint32_t));
    csumSeed = crc32c(csumSeed, (uint8_t*)&node.i_generation, sizeof(uint32_t));

    csum = crc32c(csumSeed, buffer, offset);
    csum = crc32c(csum, (uint8_t *)&dummy_csum, csum_size);
    offset += csum_size;
    csum = crc32c(csum, buffer + offset,
                  EXT4_GOOD_OLD_INODE_SIZE - offset);

    if (superBlock->s_inode_size > EXT4_GOOD_OLD_INODE_SIZE) {
        offset = offsetof(INode, i_checksum_hi);
        csum = crc32c(csum, buffer + EXT4_GOOD_OLD_INODE_SIZE,
                      offset - EXT4_GOOD_OLD_INODE_SIZE);
        csum = crc32c(csum, (uint8_t *)&dummy_csum,
                      csum_size);
        offset += csum_size;
        csum = crc32c(csum, (uint8_t *)buffer + offset,
                      sizeof(INode) - offset);
        uint8_t empty = 0;
        for(uint64_t i = 0; i < superBlock->s_inode_size - sizeof(INode); ++i) {
            csum = crc32c(csum, &empty, 1);
        }
    }
    return ~csum;
}

uint32_t EXT4::getSuperblockChecksum() {
    return ~crc32c(0, (uint8_t*)superBlock, sizeof(SuperBlock) - sizeof(superBlock->s_checksum));
}

uint16_t EXT4::getGroupDescChecksum(uint32_t groupNum) {
    GroupDesc desc = getGroupDesc(groupNum);
    uint16_t checkSum = desc.bg_checksum;
    uint32_t res = crc32c(0, (uint8_t*)superBlock->s_uuid, sizeof(superBlock->s_uuid));
    res = crc32c(res, (uint8_t*)&groupNum, sizeof(groupNum));
    desc.bg_checksum = 0;
    res = crc32c(res, (uint8_t*)&desc, sizeof(desc));
    desc.bg_checksum = checkSum;

    return ~res;
}

bool EXT4::travelExtentTree(const uint8_t* extentPtr, uint64_t sector, void(*callback)(void*, uint64_t), void* self) {
    uint64_t block = sector / sectorsPerBlock;
    ExtentHeader* header = (ExtentHeader*)extentPtr;
    if(header->eh_magic == 0xF30A) {
        for(uint16_t entryIndex = 0; entryIndex < header->eh_entries; ++entryIndex) {
            const uint8_t* entry = (extentPtr + (entryIndex + 1) * 12);
            if(header->eh_depth == 0) {
                Extent* e = (Extent*) entry;
                if(e->ee_block <= block && e->ee_block + e->ee_len > block) {
                    uint64_t start = e->ee_start_lo | ((uint64_t)e->ee_start_hi) << 32; // address
                    start += block - e->ee_block; // block offset in extent entry thing
                    uint64_t offsetInBlock = sector % sectorsPerBlock;
                    
                    callback(self, partition.LBAStart + offsetInBlock + start * sectorsPerBlock);
                    
                    return true;
                }
            } else {
                ExtentIdx* e = (ExtentIdx*) entry;
                if(e->ei_block <= block && (entryIndex + 1 == header->eh_entries /*the current ExtentIdx is the last*/ 
                                        || ((ExtentIdx*)(entry + 12))->ei_block > block) /*the next ExtentIdx does not contain the requested block*/) {
                    uint64_t blockPosition = e->ei_leaf_lo;
                    blockPosition |= ((uint64_t)e->ei_leaf_hi) << 32;

                    bool result;
                    {
                        uint8_t* blockBuffer = new uint8_t[blockSize];
                    
                        device->read(partition.LBAStart + blockPosition * sectorsPerBlock, sectorsPerBlock, blockBuffer);
                        result = travelExtentTree(extentPtr, sector, callback, self); // look for block in sub extent tree
                        
                        delete[] blockBuffer;
                    }

                    if(result) {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

bool EXT4::getSectorInFile(const INode& inode, uint8_t* buffer, uint64_t sector) {
    struct Info{
        uint8_t* buffer;
        Device* device;
    } info {buffer, device};
    return travelExtentTree((uint8_t*)inode.i_block, sector, [](void* self, uint64_t sec){
            Info* info = (Info*)self;
            info->device->read(sec, 1, info->buffer);
        }, &info);
}

bool EXT4::setSectorInFile(const INode& inode, uint8_t* buffer, uint64_t sector) {
    struct Info{
        uint8_t* buffer;
        Device* device;
    } info {buffer, device};

    return travelExtentTree((uint8_t*)inode.i_block, sector, [](void* self, uint64_t sec){
            Info* info = (Info*)self;
            info->device->write(sec, 1, info->buffer);
        }, &info);
}

bool EXT4::setSizeOfFile(uint64_t inodeNum, uint64_t size) {
    if(inodeNum <= 0) {
        return false;
    }
    INode node = getINode(inodeNum);
    node.i_size_lo = (uint32_t)(size & 0xFFFFFFFF);
    node.i_size_high = (uint32_t)(size >> 32);

    writeINode(node, inodeNum);
    
    return true;
}

uint64_t EXT4::getFileSize(const INode& inode) {
    return inode.i_size_lo | (((uint64_t) inode.i_size_high) << 32);
}

bool EXT4::writeGroupDesc(uint64_t groupNum, const GroupDesc& desc) {
    uint64_t offset = groupNum * superBlock->s_desc_size;
    uint64_t subOffset = offset % 512;
    uint8_t sector[512];
    device->read(partition.LBAStart + (1 * sectorsPerBlock) + (offset / 512), 1, sector);

    if(superBlock->s_feature_incompat & 0x80 /*64 bit*/) {
        *(GroupDesc*)(sector + subOffset) = desc;
    } else {
        memcpy(sector + subOffset, &desc, 32);
    }

    device->write(partition.LBAStart + (1 * sectorsPerBlock) + (offset / 512), 1, sector);

    return true;
}

uint64_t EXT4::findFreeINode() {
    uint32_t freeINode = superBlock->s_first_ino;
    
    for(;!isINodeUsed(freeINode);++freeINode);

    uint32_t nextFree = freeINode + 1;
    for(; !isINodeUsed(nextFree); ++nextFree);
    superBlock->s_first_ino = nextFree;
    superBlock->s_inodes_count--;
    superBlock->s_checksum = getSuperblockChecksum();
    device->write(partition.LBAStart + 2, sizeof(SuperBlock) / 512, (uint8_t*)superBlock);

    return freeINode;
}

bool EXT4::useBlocks(uint64_t first, uint64_t count, bool value) {
    uint64_t offset = first % superBlock->s_blocks_per_group;
    uint64_t group = first / superBlock->s_blocks_per_group;
    uint64_t bitmapBufferSize = ((blockBitmapSize - 1) / 512 + 1) * 512;
    uint8_t* bitmap = new uint8_t[bitmapBufferSize];
    
    uint64_t freeBlocksCount = superBlock->s_free_blocks_count_lo | (((uint64_t)superBlock->s_free_blocks_count_hi) << 32);
    freeBlocksCount -= count;
    superBlock->s_free_blocks_count_lo = (uint32_t)(count & 0xFFFFFFFF);
    superBlock->s_free_blocks_count_hi = (uint32_t)(count >> 32);
    superBlock->s_checksum = getSuperblockChecksum();
    device->write(partition.LBAStart + 2, sizeof(SuperBlock) / 512, (uint8_t*)superBlock);

    while(count) {
        GroupDesc desc = getGroupDesc(group);
        uint64_t blockBitmap = desc.bg_block_bitmap_lo | (((uint64_t)desc.bg_block_bitmap_hi) << 32);
        device->read(partition.LBAStart + blockBitmap * sectorsPerBlock, bitmapBufferSize / 512, bitmap);
        
        for(;offset < superBlock->s_blocks_per_group && count; ) {
            //write value in bit at offset
            if(offset % 8 == 0 && count >= 8) {
                //aligned to byte => write byte
                bitmap[offset / 8] = (value ? 0xFF : 0x00);
                count -= 8;
                offset += 8;
            } else {
                if(value) {
                    bitmap[offset / 8] |= 0b1 << (offset % 8);
                } else {
                    bitmap[offset / 8] &= ~(0b1 << (offset % 8));
                }
                --count;
                ++offset;
            }
        }
        device->write(partition.LBAStart + blockBitmap * sectorsPerBlock, bitmapBufferSize / 512, bitmap);
        
        uint32_t myChecksum = crc32c(0, superBlock->s_uuid, sizeof(superBlock->s_uuid));
        myChecksum = crc32c(myChecksum, bitmap, bitmapBufferSize);
        desc.bg_block_bitmap_csum_lo = (uint16_t)((~myChecksum) & 0xFFFF);
        desc.bg_block_bitmap_csum_hi = (uint16_t)(((~myChecksum) >> 16) & 0xFFFF);
        writeGroupDesc(group, desc);
        
        offset = 0;
        group += 1;
    }
    delete[] bitmap;

    return true;
}
bool EXT4::writeINode(const INode& node, uint64_t inodeNum) {
    if(inodeNum <= 0) {
        return false;
    }
    uint64_t blockGroup = (inodeNum - 1) / superBlock->s_inodes_per_group;
    uint64_t index = (inodeNum - 1) % superBlock->s_inodes_per_group;
    uint64_t offset = index * superBlock->s_inode_size;

    GroupDesc desc = getGroupDesc(blockGroup);
    
    uint64_t inodeTableBlock = desc.bg_inode_table_lo;
    inodeTableBlock |= ((uint64_t)desc.bg_inode_table_hi) << 32;
    uint64_t inodeTableSector = inodeTableBlock * sectorsPerBlock;
    
    uint8_t sector[512]{};
    device->read(partition.LBAStart + inodeTableSector + offset / 512, 1, sector);
    
    uint32_t inodeChecksum = generateINodeChecksum(node, inodeNum);

    *((INode*)(sector + (offset % 512))) = node;
    ((INode*)(sector + (offset % 512)))->osd2.linux2.l_i_checksum_lo = (uint16_t)(inodeChecksum & 0xFFFF);
    ((INode*)(sector + (offset % 512)))->i_checksum_hi = (uint16_t)(inodeChecksum >> 16);

    device->write(partition.LBAStart + inodeTableSector + offset / 512, 1, sector);
    return true;
}
bool EXT4::increaseFileSize(uint64_t inodeNum, uint64_t size) {
    if(size == 0)
        return false;

    INode node = getINode(inodeNum);
    uint64_t fileSize = getFileSize(node);
    if(size <= fileSize % blockSize) {
        setSizeOfFile(inodeNum, fileSize + size); // only write size if it is the only changed value.
        return true;
    }

    //update node filesize
    node.i_size_lo = (uint32_t)((fileSize + size) & 0xFFFFFFFF);
    node.i_size_high = (uint32_t)((fileSize + size) >> 32);

    size -= fileSize % blockSize; // insert in existing block
    //size > 0
    uint64_t neededBlockCount = ((size - 1) / blockSize) + 1;

    ExtentHeader* header = (ExtentHeader*)node.i_block;
    if(header->eh_depth != 0) {
        kout << "TODO " << __FILE__ << ':' << (uint64_t)__LINE__ << '\n';
        asm("hlt");
    }
    Extent* ext = (Extent*)(((uint8_t*)header) + header->eh_entries * 12);
    uint64_t start = ext->ee_start_lo | (((uint64_t) ext->ee_start_hi) << 32);
    uint64_t next = start + ext->ee_len;
    bool appendAllocated = false;
    for(; !isBlockUsed(next) && neededBlockCount; ++next) {
        appendAllocated = true;
        --neededBlockCount;
    }
    if(appendAllocated) {
        useBlocks(start + ext->ee_len, next - start + ext->ee_len, true);
        ext->ee_len = next - start;
    }
    
    if(neededBlockCount) {
        if(header->eh_max_entries == header->eh_entries) {
            // no empty entries
            // increase extent depth
            kout << "TODO " << __FILE__ << ':' << (uint64_t)__LINE__ << '\n';
            asm("hlt");
        }
        //find blocks

        uint64_t totalBlockCount = superBlock->s_blocks_count_lo | ((uint64_t)(superBlock->s_blocks_count_hi) << 32);
        uint64_t blockLocation = 0;
        for(uint64_t blockIndex = next; blockIndex != next-1; ++blockIndex) {
            uint64_t sub = 0;
            for(; sub < neededBlockCount; ++sub) {
                if(sub+neededBlockCount > totalBlockCount)
                    return false;
                if(isBlockUsed(sub + neededBlockCount))
                    break;
            }
            if(sub == neededBlockCount) {
                blockLocation = blockIndex;
                break;
            }
        }
        if(blockLocation == 0)
            return false;

        Extent* newExt = (Extent*)(((uint8_t*)header) + header->eh_entries * 12 + 12);
        newExt->ee_block = next;
        newExt->ee_len = neededBlockCount;
        newExt->ee_start_lo = (uint32_t)(blockLocation & 0xFFFFFFFF);
        newExt->ee_start_hi = (uint32_t)(blockLocation >> 32);
        useBlocks(blockLocation + ext->ee_len, next - blockLocation + ext->ee_len, true);
    }
    writeINode(node, inodeNum);
    
    return true;
}
bool EXT4::decreaseFileSize(uint64_t inodeNum, uint64_t size) {

    return false;
}

void EXT4::iterateDirectory(const INode& inode, void(*callback)(const DirEntry&)) {
    if(!(inode.i_mode & 0x4000))
        return;
    
    uint64_t size = getFileSize(inode);
    uint8_t* block = new uint8_t[blockSize];
    for(uint64_t i = 0; i < size; i += blockSize) {
        for(uint64_t j = 0; j < sectorsPerBlock; ++j) {
            if(!getSectorInFile(inode, block + (512 * j), j)) {
                delete[] block;
                return;
            }
        }
        for(uint8_t* ptr = block; ptr < block + blockSize;) {
            DirEntry* entry = (DirEntry*)ptr;
            ptr += entry->size;
            if(entry->inode == 0) {
                continue;
            }
            callback(*entry);
        }
    }
    delete[] block;
}

void EXT4::iterateDirectory(const INode& inode, bool(*callback)(void*, const DirEntry&), void* self) {
    if(!(inode.i_mode & 0x4000))
        return;
    
    uint64_t size = getFileSize(inode);
    uint8_t* block = new uint8_t[blockSize];
    for(uint64_t i = 0; i < size; i += blockSize) {
        for(uint64_t j = 0; j < sectorsPerBlock; ++j) {
            if(!getSectorInFile(inode, block + (512 * j), j)) {
                delete[] block;
                return;
            }
        }
        for(uint8_t* ptr = block; ptr < block + blockSize;) {
            DirEntry* entry = (DirEntry*)ptr;
            ptr += entry->size;
            if(entry->inode == 0) {
                continue;
            }
            if(callback(self, *entry)) {
                delete[] block;
                return;
            }
        }
    }
    delete[] block;
}

EXT4::~EXT4() {
    delete superBlock;
}

bool EXT4::isINodeUsed(uint64_t inodeNum) {
    uint64_t group = inodeNum / superBlock->s_inodes_per_group;
    uint64_t index = inodeNum % superBlock->s_inodes_per_group;
    uint64_t byteIndex = index / 8;
    uint64_t bitIndex = index % 8;
    uint64_t sectorOffset = byteIndex / 512;
    byteIndex %= 512;
    GroupDesc g = getGroupDesc(group);
    uint64_t inodeBitmap = g.bg_inode_bitmap_lo;
    inodeBitmap |= ((uint64_t)g.bg_inode_bitmap_hi) << 32;

    uint8_t buffer[512];
    device->read(partition.LBAStart + inodeBitmap * sectorsPerBlock + sectorOffset, 1, buffer);
    return (buffer[byteIndex] >> bitIndex);
}

uint64_t EXT4::findFileINode(const char* name) {
    uint64_t inodeNum = 2;
    INode node;
    node = getINode(inodeNum);
    for(const char* str = name; *str; ++str) {
        if(*str == '/' || *str == '\\') {
            continue;
        }
        if(node.i_mode & 0x4000) {
            // find next path element
            const char* ptr;
            for(ptr = str; *ptr; ++ptr) {
                if(*ptr == '/' || *ptr == '\\') {
                    break;
                }
            } /*find size of next path element*/
            
            //store information for iterate directory call
            struct Info{
                uint64_t size;
                const char* str;
                uint64_t* inodeNum;
                bool found;
            } info;
            info.size = ptr - str;
            info.str = str;
            info.inodeNum = &inodeNum;
            info.found = false;
            
            /*look for path element*/
            iterateDirectory(node, [](void* _info, const DirEntry& entry) -> bool{
                Info* info = (Info*) _info;
                if(entry.nameSize == info->size && memequ(info->str, entry.name, info->size)) {
                    *info->inodeNum = entry.inode;
                    info->found = true;
                    return true;
                }
                return false;
            }, &info);
            if(!info.found) {
                //not found -> error
                return 0;
            }
            node = getINode(inodeNum); //update node
            str = ptr - 1; // skip read node
        } else {
            // end of name not found but this is a file -> error
            return 0;
        }
    }
    return inodeNum;
}

EXT4::FileType EXT4::getFileType(const INode& inode) {
    if (inode.i_mode & 0x1000)
        return FileType::FIFO;
    else if (inode.i_mode & 0x2000)
        return FileType::CHARACTER_DEVICE_FILE;
    else if (inode.i_mode & 0x4000)
        return FileType::DIRECTORY;
    else if (inode.i_mode & 0x6000)
        return FileType::BLOCK_DEVICE_FILE;
    else if (inode.i_mode & 0x8000)
        return FileType::REGULAR_FILE;
    else if (inode.i_mode & 0xA000)
        return FileType::SYMBOLIC_LINK;
    else if (inode.i_mode & 0xC000)
        return FileType::SOCKET;
    return FileType::UNKNOWN;
}

bool EXT4::isBlockUsed(uint64_t blockNum) {
    uint64_t group = blockNum / superBlock->s_blocks_per_group;
    uint64_t index = blockNum % superBlock->s_blocks_per_group;
    uint64_t byteIndex = index / 8;
    uint64_t bitIndex = index % 8;
    uint64_t sectorOffset = byteIndex / 512;
    byteIndex %= 512;
    GroupDesc g = getGroupDesc(group);
    uint64_t blockBitmap = g.bg_block_bitmap_lo;
    blockBitmap |= ((uint64_t)g.bg_block_bitmap_hi) << 32;

    if(lastBlockBitmap == nullptr) {
        lastBlockBitmap = new uint8_t[512];
        lastBlockBitmapIndex = blockBitmap + 1;
    }
    if(lastBlockBitmapIndex != blockBitmap) {
        lastBlockBitmapIndex = blockBitmap;
        device->read(partition.LBAStart + blockBitmap * sectorsPerBlock + sectorOffset, 1, lastBlockBitmap);
    }
    
    return (lastBlockBitmap[byteIndex] >> bitIndex);
}


EXT4::INode EXT4::getINode(uint64_t inodeNum) {
    if(inodeNum <= 0) {
        return {};
    }
    INode value;
    uint64_t blockGroup = (inodeNum - 1) / superBlock->s_inodes_per_group;
    uint64_t index = (inodeNum - 1) % superBlock->s_inodes_per_group;
    uint64_t offset = index * superBlock->s_inode_size;

    GroupDesc desc = getGroupDesc(blockGroup);
    
    uint64_t inodeTableBlock = desc.bg_inode_table_lo;
    inodeTableBlock |= ((uint64_t)desc.bg_inode_table_hi) << 32;
    uint64_t inodeTableSector = inodeTableBlock * sectorsPerBlock;
    
    uint8_t sector[512]{};
    device->read(partition.LBAStart + inodeTableSector + offset / 512, 1, sector);
    
    value = *(INode*)(sector + (offset % 512));

    return value;
}

EXT4::GroupDesc EXT4::getGroupDesc(uint64_t groupNum) {
    GroupDesc desc{};
    uint64_t offset = groupNum * superBlock->s_desc_size;
    uint64_t subOffset = offset % 512;
    uint8_t sector[512];
    device->read(partition.LBAStart + (1 * sectorsPerBlock) + (offset / 512), 1, sector);

    if(superBlock->s_feature_incompat & 0x80 /*64 bit*/) {
        desc = *(GroupDesc*)(sector + subOffset);
    } else {
        memcpy(&desc, sector + subOffset, 32);
    }

    return desc;
}

}