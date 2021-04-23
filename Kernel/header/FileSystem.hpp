#pragma once

#include "stdint.h"
#include "IDE.hpp"
#include "Partition.hpp"

namespace Kernel {

class EXT4 {
public:
    struct SuperBlock {
        uint32_t s_inodes_count;            //Total inode count.
        uint32_t s_blocks_count_lo;         //Total block count.
        uint32_t s_r_blocks_count_lo;       //This number of blocks can only be allocated by the super-user.
        uint32_t s_free_blocks_count_lo;    //Free block count.
        uint32_t s_free_inodes_count;       //Free inode count.
        uint32_t s_first_data_block;        //First data block. This must be at least 1 for 1k-block filesystems and is typically 0 for all other block sizes.
        uint32_t s_log_block_size;          //Block size is 2 ^ (10 + s_log_block_size).
        uint32_t s_log_cluster_size;        //Cluster size is (2 ^ s_log_cluster_size) blocks if bigalloc is enabled. Otherwise s_log_cluster_size must equal s_log_block_size.
        uint32_t s_blocks_per_group;        //Blocks per group.
        uint32_t s_clusters_per_group;      //Clusters per group, if bigalloc is enabled. Otherwise s_clusters_per_group must equal s_blocks_per_group.
        uint32_t s_inodes_per_group;        //Inodes per group.
        uint32_t s_mtime;                   //Mount time, in seconds since the epoch.
        uint32_t s_wtime;                   //Write time, in seconds since the epoch.
        uint16_t s_mnt_count;               //Number of mounts since the last fsck.
        uint16_t s_max_mnt_count;           //Number of mounts beyond which a fsck is needed.
        uint16_t s_magic;                   //Magic signature, 0xEF53
        uint16_t s_state;                   //File system state. Valid values are:
                                            //1 Cleanly umounted
                                            //2 Errors detected
                                            //4 Orphans being recovered
        uint16_t s_errors;                  //Behaviour when detecting errors. One of:
                                            //1 Continue
                                            //2 Remount read-only
                                            //3 Panic
        uint16_t s_minor_rev_level;         //Minor revision level.
        uint32_t s_lastcheck;               //Time of last check, in seconds since the epoch.
        uint32_t s_checkinterval;           //Maximum time between checks, in seconds.
        uint32_t s_creator_os;              //OS. One of:
                                            //0 Linux
                                            //1 Hurd
                                            //2 Masix
                                            //3 FreeBSD
                                            //4 Lites
        uint32_t s_rev_level;               //Revision level. One of:
                                            //0 Original format
                                            //1 v2 format w/ dynamic inode sizes
        uint16_t s_def_resuid;              //Default uid for reserved blocks.
        uint16_t s_def_resgid;              //Default gid for reserved blocks.
        uint32_t s_first_ino;               //First non-reserved inode.
        uint16_t s_inode_size;              //Size of inode structure, in bytes.
        uint16_t s_block_group_nr;          //Block group # of this superblock.
        uint32_t s_feature_compat;          //Compatible feature set flags. Kernel can still read/write this fs even if it doesn't understand a flag; e2fsck will not attempt to fix a filesystem with any unknown COMPAT flags. Any of:
                                            //0x1 	Directory preallocation (COMPAT_DIR_PREALLOC).
                                            //0x2 	"imagic inodes". Used by AFS to indicate inodes that are not linked into the directory namespace. Inodes marked with this flag will not be added to lost+found by e2fsck. (COMPAT_IMAGIC_INODES).
                                            //0x4 	Has a journal (COMPAT_HAS_JOURNAL).
                                            //0x8 	Supports extended attributes (COMPAT_EXT_ATTR).
                                            //0x10 	Has reserved GDT blocks for filesystem expansion. Requires RO_COMPAT_SPARSE_SUPER. (COMPAT_RESIZE_INODE).
                                            //0x20 	Has indexed directories. (COMPAT_DIR_INDEX).
                                            //0x40 	"Lazy BG". Not in Linux kernel, seems to have been for uninitialized block groups? (COMPAT_LAZY_BG).
                                            //0x80 	"Exclude inode". Intended for filesystem snapshot feature, but not used. (COMPAT_EXCLUDE_INODE).
                                            //0x100 	"Exclude bitmap". Seems to be used to indicate the presence of snapshot-related exclude bitmaps? Not defined in kernel or used in e2fsprogs. (COMPAT_EXCLUDE_BITMAP).
                                            //0x200 	Sparse Super Block, v2. If this flag is set, the SB field s_backup_bgs points to the two block groups that contain backup superblocks. (COMPAT_SPARSE_SUPER2).
        uint32_t s_feature_incompat;        //Incompatible feature set. If the kernel or e2fsck doesn't understand one of these bits, it will refuse to mount or attempt to repair the filesystem. Any of:
                                            //0x1 	Compression. Not implemented. (INCOMPAT_COMPRESSION).
                                            //0x2 	Directory entries record the file type. See ext4_dir_entry_2 below. (INCOMPAT_FILETYPE).
                                            //0x4 	Filesystem needs journal recovery. (INCOMPAT_RECOVER).
                                            //0x8 	Filesystem has a separate journal device. (INCOMPAT_JOURNAL_DEV).
                                            //0x10 	Meta block groups. See the earlier discussion of this feature. (INCOMPAT_META_BG).
                                            //0x40 	Files in this filesystem use extents. (INCOMPAT_EXTENTS).
                                            //0x80 	Enable a filesystem size over 2^32 blocks. (INCOMPAT_64BIT).
                                            //0x100 	Multiple mount protection. Prevent multiple hosts from mounting the filesystem concurrently by updating a reserved block periodically while mounted and checking this at mount time to determine if the filesystem is in use on another host. (INCOMPAT_MMP).
                                            //0x200 	Flexible block groups. See the earlier discussion of this feature. (INCOMPAT_FLEX_BG).
                                            //0x400 	Inodes can be used to store large extended attribute values (INCOMPAT_EA_INODE).
                                            //0x1000 	Data in directory entry. Allow additional data fields to be stored in each dirent, after struct ext4_dirent. The presence of extra data is indicated by flags in the high bits of ext4_dirent file type flags (above EXT4_FT_MAX). The flag EXT4_DIRENT_LUFID = 0x10 is used to store a 128-bit File Identifier for Lustre. The flag EXT4_DIRENT_IO64 = 0x20 is used to store the high word of 64-bit inode numbers. Feature still in development. (INCOMPAT_DIRDATA).
                                            //0x2000 	Metadata checksum seed is stored in the superblock. This feature enables the administrator to change the UUID of a metadata_csum filesystem while the filesystem is mounted; without it, the checksum definition requires all metadata blocks to be rewritten. (INCOMPAT_CSUM_SEED).
                                            //0x4000 	Large directory >2GB or 3-level htree. Prior to this feature, directories could not be larger than 4GiB and could not have an htree more than 2 levels deep. If this feature is enabled, directories can be larger than 4GiB and have a maximum htree depth of 3. (INCOMPAT_LARGEDIR).
                                            //0x8000 	Data in inode. Small files or directories are stored directly in the inode i_blocks and/or xattr space. (INCOMPAT_INLINE_DATA).
                                            //0x10000 	Encrypted inodes are present on the filesystem (INCOMPAT_ENCRYPT).
        uint32_t s_feature_ro_compat;       //Readonly-compatible feature set. If the kernel doesn't understand one of these bits, it can still mount read-only, but e2fsck will refuse to modify the filesystem. Any of:
                                            //0x1 	Sparse superblocks. See the earlier discussion of this feature. (RO_COMPAT_SPARSE_SUPER).
                                            //0x2 	Allow storing files larger than 2GiB (RO_COMPAT_LARGE_FILE).
                                            //0x4 	Was intended for use with htree directories, but was not needed. Not used in kernel or e2fsprogs (RO_COMPAT_BTREE_DIR).
                                            //0x8 	This filesystem has files whose space usage is stored in i_blocks in units of filesystem blocks, not 512-byte sectors. Inodes using this feature will be marked with EXT4_INODE_HUGE_FILE. (RO_COMPAT_HUGE_FILE)
                                            //0x10 	Group descriptors have checksums. In addition to detecting corruption, this is useful for lazy formatting with uninitialized groups (RO_COMPAT_GDT_CSUM).
                                            //0x20 	Indicates that the old ext3 32,000 subdirectory limit no longer applies. A directory's i_links_count will be set to 1 if it is incremented past 64,999. (RO_COMPAT_DIR_NLINK).
                                            //0x40 	Indicates that large inodes exist on this filesystem, storing extra fields after EXT2_GOOD_OLD_INODE_SIZE. (RO_COMPAT_EXTRA_ISIZE).
                                            //0x80 	This filesystem has a snapshot. Not implemented in ext4. (RO_COMPAT_HAS_SNAPSHOT).
                                            //0x100 	Quota is handled transactionally with the journal (RO_COMPAT_QUOTA).
                                            //0x200 	This filesystem supports "bigalloc", which means that filesystem block allocation bitmaps are tracked in units of clusters (of blocks) instead of blocks (RO_COMPAT_BIGALLOC).
                                            //0x400 	This filesystem supports metadata checksumming. (RO_COMPAT_METADATA_CSUM; implies RO_COMPAT_GDT_CSUM, though GDT_CSUM must not be set)
                                            //0x800 	Filesystem supports replicas. This feature is neither in the kernel nor e2fsprogs. (RO_COMPAT_REPLICA).
                                            //0x1000 	Read-only filesystem image; the kernel will not mount this image read-write and most tools will refuse to write to the image. (RO_COMPAT_READONLY).
                                            //0x2000 	Filesystem tracks project quotas. (RO_COMPAT_PROJECT)
        uint8_t s_uuid[16];                 //128-bit UUID for volume.
        char s_volume_name[16];             //Volume label.
        char s_last_mounted[64];            //Directory where filesystem was last mounted.
        uint32_t s_algorithm_usage_bitmap;  //For compression (Not used in e2fsprogs/Linux)
                                            //Performance hints. Directory preallocation should only happen if the EXT4_FEATURE_COMPAT_DIR_PREALLOC flag is on.
        uint8_t s_prealloc_blocks;          //# of blocks to try to preallocate for ... files? (Not used in e2fsprogs/Linux)
        uint8_t s_prealloc_dir_blocks;      //# of blocks to preallocate for directories. (Not used in e2fsprogs/Linux)
        uint16_t s_reserved_gdt_blocks;     //Number of reserved GDT entries for future filesystem expansion.
                                            //Journaling support valid if EXT4_FEATURE_COMPAT_HAS_JOURNAL set.
        uint8_t s_journal_uuid[16];         //UUID of journal superblock
        uint32_t s_journal_inum;            //inode number of journal file.
        uint32_t s_journal_dev;             //Device number of journal file, if the external journal feature flag is set.
        uint32_t s_last_orphan;             //Start of list of orphaned inodes to delete.
        uint32_t s_hash_seed[4];            //HTREE hash seed.
        uint8_t s_def_hash_version;         //Default hash algorithm to use for directory hashes. One of:
                                            // 0x0 	Legacy.
                                            // 0x1 	Half MD4.
                                            // 0x2 	Tea.
                                            // 0x3 	Legacy, unsigned.
                                            // 0x4 	Half MD4, unsigned.
                                            // 0x5 	Tea, unsigned.
        uint8_t s_jnl_backup_type;          //If this value is 0 or EXT3_JNL_BACKUP_BLOCKS (1), then the s_jnl_blocks field contains a duplicate copy of the inode's i_block[] array and i_size.
        uint16_t s_desc_size;               //Size of group descriptors, in bytes, if the 64bit incompat feature flag is set.
        uint32_t s_default_mount_opts;      //Default mount options. Any of:
                                            // 0x0001 	Print debugging info upon (re)mount. (EXT4_DEFM_DEBUG)
                                            // 0x0002 	New files take the gid of the containing directory (instead of the fsgid of the current process). (EXT4_DEFM_BSDGROUPS)
                                            // 0x0004 	Support userspace-provided extended attributes. (EXT4_DEFM_XATTR_USER)
                                            // 0x0008 	Support POSIX access control lists (ACLs). (EXT4_DEFM_ACL)
                                            // 0x0010 	Do not support 32-bit UIDs. (EXT4_DEFM_UID16)
                                            // 0x0020 	All data and metadata are commited to the journal. (EXT4_DEFM_JMODE_DATA)
                                            // 0x0040 	All data are flushed to the disk before metadata are committed to the journal. (EXT4_DEFM_JMODE_ORDERED)
                                            // 0x0060 	Data ordering is not preserved; data may be written after the metadata has been written. (EXT4_DEFM_JMODE_WBACK)
                                            // 0x0100 	Disable write flushes. (EXT4_DEFM_NOBARRIER)
                                            // 0x0200 	Track which blocks in a filesystem are metadata and therefore should not be used as data blocks. This option will be enabled by default on 3.18, hopefully. (EXT4_DEFM_BLOCK_VALIDITY)
                                            // 0x0400 	Enable DISCARD support, where the storage device is told about blocks becoming unused. (EXT4_DEFM_DISCARD)
                                            // 0x0800 	Disable delayed allocation. (EXT4_DEFM_NODELALLOC)
        uint32_t s_first_meta_bg;           //First metablock block group, if the meta_bg feature is enabled.
        uint32_t s_mkfs_time;               //When the filesystem was created, in seconds since the epoch.
        uint32_t s_jnl_blocks[17];          //Backup copy of the journal inode's i_block[] array in the first 15 elements and i_size_high and i_size in the 16th and 17th elements, respectively.
                                            // 64bit support valid if EXT4_FEATURE_COMPAT_64BIT
        uint32_t s_blocks_count_hi;         //High 32-bits of the block count.
        uint32_t s_r_blocks_count_hi;       //High 32-bits of the reserved block count.
        uint32_t s_free_blocks_count_hi;    //High 32-bits of the free block count.
        uint16_t s_min_extra_isize;         //All inodes have at least # bytes.
        uint16_t s_want_extra_isize;        //New inodes should reserve # bytes.
        uint32_t s_flags;                   //Miscellaneous flags. Any of:
                                            // 0x0001 	Signed directory hash in use.
                                            // 0x0002 	Unsigned directory hash in use.
                                            // 0x0004 	To test development code.
        uint16_t s_raid_stride;             //RAID stride. This is the number of logical blocks read from or written to the disk before moving to the next disk. This affects the placement of filesystem metadata, which will hopefully make RAID storage faster.
        uint16_t s_mmp_interval;            //# seconds to wait in multi-mount prevention (MMP) checking. In theory, MMP is a mechanism to record in the superblock which host and device have mounted the filesystem, in order to prevent multiple mounts. This feature does not seem to be implemented...
        uint64_t s_mmp_block;               //Block # for multi-mount protection data.
        uint32_t s_raid_stripe_width;       //RAID stripe width. This is the number of logical blocks read from or written to the disk before coming back to the current disk. This is used by the block allocator to try to reduce the number of read-modify-write operations in a RAID5/6.
        uint8_t s_log_groups_per_flex;      //Size of a flexible block group is 2 ^ s_log_groups_per_flex.
        uint8_t s_checksum_type;            //Metadata checksum algorithm type. The only valid value is 1 (crc32c).
        uint16_t s_reserved_pad;            //
        uint64_t s_kbytes_written;          //Number of KiB written to this filesystem over its lifetime.
        uint32_t s_snapshot_inum;           //inode number of active snapshot. (Not used in e2fsprogs/Linux.)
        uint32_t s_snapshot_id;             //Sequential ID of active snapshot. (Not used in e2fsprogs/Linux.)
        uint64_t s_snapshot_r_blocks_count; //Number of blocks reserved for active snapshot's future use. (Not used in e2fsprogs/Linux.)
        uint32_t s_snapshot_list;           //inode number of the head of the on-disk snapshot list. (Not used in e2fsprogs/Linux.)
        uint32_t s_error_count;             //Number of errors seen.
        uint32_t s_first_error_time;        //First time an error happened, in seconds since the epoch.
        uint32_t s_first_error_ino;         //inode involved in first error.
        uint64_t s_first_error_block;       //Number of block involved of first error.
        uint8_t s_first_error_func[32];     //Name of function where the error happened.
        uint32_t s_first_error_line;        //Line number where error happened.
        uint32_t s_last_error_time;         //Time of most recent error, in seconds since the epoch.
        uint32_t s_last_error_ino;          //inode involved in most recent error.
        uint32_t s_last_error_line;         //Line number where most recent error happened.
        uint64_t s_last_error_block;        //Number of block involved in most recent error.
        uint8_t s_last_error_func[32];      //Name of function where the most recent error happened.
        uint8_t s_mount_opts[64];           //ASCIIZ string of mount options.
        uint32_t s_usr_quota_inum;          //Inode number of user quota file.
        uint32_t s_grp_quota_inum;          //Inode number of group quota file.
        uint32_t s_overhead_blocks;         //Overhead blocks/clusters in fs. (Huh? This field is always zero, which means that the kernel calculates it dynamically.)
        uint32_t s_backup_bgs[2];           //Block groups containing superblock backups (if sparse_super2)
        uint8_t s_encrypt_algos[4];         //Encryption algorithms in use. There can be up to four algorithms in use at any time; valid algorithm codes are given below:
                                            // 0 	Invalid algorithm (ENCRYPTION_MODE_INVALID).
                                            // 1 	256-bit AES in XTS mode (ENCRYPTION_MODE_AES_256_XTS).
                                            // 2 	256-bit AES in GCM mode (ENCRYPTION_MODE_AES_256_GCM).
                                            // 3 	256-bit AES in CBC mode (ENCRYPTION_MODE_AES_256_CBC).
        uint8_t s_encrypt_pw_salt[16];      //Salt for the string2key algorithm for encryption.
        uint32_t s_lpf_ino;                 //Inode number of lost+found
        uint32_t s_prj_quota_inum;          //Inode that tracks project quotas.
        uint32_t s_checksum_seed;           //Checksum seed used for metadata_csum calculations. This value is crc32c(~0, $orig_fs_uuid).
        uint32_t s_reserved[98];            //Padding to the end of the block.
        uint32_t s_checksum;                //Superblock checksum.
    } __attribute__((packed));

    struct GroupDesc {
        uint32_t bg_block_bitmap_lo;        //Lower 32-bits of location of block bitmap.
        uint32_t bg_inode_bitmap_lo;        //Lower 32-bits of location of inode bitmap.
        uint32_t bg_inode_table_lo;         //Lower 32-bits of location of inode table.
        uint16_t bg_free_blocks_count_lo;   //Lower 16-bits of free block count.
        uint16_t bg_free_inodes_count_lo;   //Lower 16-bits of free inode count.
        uint16_t bg_used_dirs_count_lo;     //Lower 16-bits of directory count.
        uint16_t bg_flags;                  //Block group flags. Any of:
                                            //0x1 	inode table and bitmap are not initialized (EXT4_BG_INODE_UNINIT).
                                            //0x2 	block bitmap is not initialized (EXT4_BG_BLOCK_UNINIT).
                                            //0x4 	inode table is zeroed (EXT4_BG_INODE_ZEROED).
        uint32_t bg_exclude_bitmap_lo;      //Lower 32-bits of location of snapshot exclusion bitmap.
        uint16_t bg_block_bitmap_csum_lo;   //Lower 16-bits of the block bitmap checksum.
        uint16_t bg_inode_bitmap_csum_lo;   //Lower 16-bits of the inode bitmap checksum.
        uint16_t bg_itable_unused_lo;       //Lower 16-bits of unused inode count. If set, we needn't scan past the (sb.s_inodes_per_group - gdt.bg_itable_unused)th entry in the inode table for this group.
        uint16_t bg_checksum;               //Group descriptor checksum; crc16(sb_uuid+group+desc) if the RO_COMPAT_GDT_CSUM feature is set, or crc32c(sb_uuid+group_desc) & 0xFFFF if the RO_COMPAT_METADATA_CSUM feature is set.
        uint32_t bg_block_bitmap_hi;        //Upper 32-bits of location of block bitmap.
        uint32_t bg_inode_bitmap_hi;        //Upper 32-bits of location of inodes bitmap.
        uint32_t bg_inode_table_hi;         //Upper 32-bits of location of inodes table.
        uint16_t bg_free_blocks_count_hi;   //Upper 16-bits of free block count.
        uint16_t bg_free_inodes_count_hi;   //Upper 16-bits of free inode count.
        uint16_t bg_used_dirs_count_hi;     //Upper 16-bits of directory count.
        uint16_t bg_itable_unused_hi;       //Upper 16-bits of unused inode count.
        uint32_t bg_exclude_bitmap_hi;      //Upper 32-bits of location of snapshot exclusion bitmap.
        uint16_t bg_block_bitmap_csum_hi;   //Upper 16-bits of the block bitmap checksum.
        uint16_t bg_inode_bitmap_csum_hi;   //Upper 16-bits of the inode bitmap checksum.
        uint32_t bg_reserved;               //Padding to 64 bytes.
    } __attribute__((packed));

    struct INode {
        uint16_t i_mode;         //File mode. Any of:
                                 //0x1 	S_IXOTH (Others may execute)
                                 //0x2 	S_IWOTH (Others may write)
                                 //0x4 	S_IROTH (Others may read)
                                 //0x8 	S_IXGRP (Group members may execute)
                                 //0x10 	S_IWGRP (Group members may write)
                                 //0x20 	S_IRGRP (Group members may read)
                                 //0x40 	S_IXUSR (Owner may execute)
                                 //0x80 	S_IWUSR (Owner may write)
                                 //0x100 	S_IRUSR (Owner may read)
                                 //0x200 	S_ISVTX (Sticky bit)
                                 //0x400 	S_ISGID (Set GID)
                                 //0x800 	S_ISUID (Set UID)
                                 //These are mutually-exclusive file types:
                                 //0x1000 	S_IFIFO (FIFO)
                                 //0x2000 	S_IFCHR (Character device)
                                 //0x4000 	S_IFDIR (Directory)
                                 //0x6000 	S_IFBLK (Block device)
                                 //0x8000 	S_IFREG (Regular file)
                                 //0xA000 	S_IFLNK (Symbolic link)
                                 //0xC000 	S_IFSOCK (Socket)
        uint16_t i_uid;          //lower 16-bits of Owner UID.
        uint32_t i_size_lo;      //Lower 32-bits of size in bytes. 
        uint32_t i_atime;        //Last access time, in seconds since the epoch. However, if the EA_INODE inode flag is set, this inode stores an extended attribute value and this field contains the checksum of the value. 
        uint32_t i_ctime;        //Last inode change time, in seconds since the epoch. However, if the EA_INODE inode flag is set, this inode stores an extended attribute value and this field contains the lower 32 bits of the attribute value's reference count. 
        uint32_t i_mtime;        //Last data modification time, in seconds since the epoch. However, if the EA_INODE inode flag is set, this inode stores an extended attribute value and this field contains the number of the inode that owns the extended attribute. 
        uint32_t i_dtime;        //Deletion Time, in seconds since the epoch.
        uint16_t i_gid;          //Lower 16-bits of GID. 
        uint16_t i_links_count;  //Hard link count. Normally, ext4 does not permit an inode to have more than 65,000 hard links. This applies to files as well as directories, which means that there cannot be more than 64,998 subdirectories in a directory (each subdirectory's '..' entry counts as a hard link, as does the '.' entry in the directory itself). With the DIR_NLINK feature enabled, ext4 supports more than 64,998 subdirectories by setting this field to 1 to indicate that the number of hard links is not known. 
        uint32_t i_blocks_lo;    //Lower 32-bits of "block" count. If the huge_file feature flag is not set on the filesystem, the file consumes i_blocks_lo 512-byte blocks on disk. If huge_file is set and EXT4_HUGE_FILE_FL is NOT set in inode.i_flags, then the file consumes i_blocks_lo + (i_blocks_hi << 32) 512-byte blocks on disk. If huge_file is set and EXT4_HUGE_FILE_FL IS set in inode.i_flags, then this file consumes (i_blocks_lo + i_blocks_hi << 32) filesystem blocks on disk. 
        uint32_t i_flags;        //Inode flags. Any of:
                                 //0x1 	This file requires secure deletion (EXT4_SECRM_FL). (not implemented)
                                 //0x2 	This file should be preserved, should undeletion be desired (EXT4_UNRM_FL). (not implemented)
                                 //0x4 	File is compressed (EXT4_COMPR_FL). (not really implemented)
                                 //0x8 	All writes to the file must be synchronous (EXT4_SYNC_FL).
                                 //0x10 	File is immutable (EXT4_IMMUTABLE_FL).
                                 //0x20 	File can only be appended (EXT4_APPEND_FL).
                                 //0x40 	The dump(1) utility should not dump this file (EXT4_NODUMP_FL).
                                 //0x80 	Do not update access time (EXT4_NOATIME_FL).
                                 //0x100 	Dirty compressed file (EXT4_DIRTY_FL). (not used)
                                 //0x200 	File has one or more compressed clusters (EXT4_COMPRBLK_FL). (not used)
                                 //0x400 	Do not compress file (EXT4_NOCOMPR_FL). (not used)
                                 //0x800 	Encrypted inode (EXT4_ENCRYPT_FL). This bit value previously was EXT4_ECOMPR_FL (compression error), which was never used.
                                 //0x1000 	Directory has hashed indexes (EXT4_INDEX_FL).
                                 //0x2000 	AFS magic directory (EXT4_IMAGIC_FL).
                                 //0x4000 	File data must always be written through the journal (EXT4_JOURNAL_DATA_FL).
                                 //0x8000 	File tail should not be merged (EXT4_NOTAIL_FL). (not used by ext4)
                                 //0x10000 	All directory entry data should be written synchronously (see dirsync) (EXT4_DIRSYNC_FL).
                                 //0x20000 	Top of directory hierarchy (EXT4_TOPDIR_FL).
                                 //0x40000 	This is a huge file (EXT4_HUGE_FILE_FL).
                                 //0x80000 	Inode uses extents (EXT4_EXTENTS_FL).
                                 //0x200000 	Inode stores a large extended attribute value in its data blocks (EXT4_EA_INODE_FL).
                                 //0x400000 	This file has blocks allocated past EOF (EXT4_EOFBLOCKS_FL). (deprecated)
                                 //0x01000000 	Inode is a snapshot (EXT4_SNAPFILE_FL). (not in mainline)
                                 //0x04000000 	Snapshot is being deleted (EXT4_SNAPFILE_DELETED_FL). (not in mainline)
                                 //0x08000000 	Snapshot shrink has completed (EXT4_SNAPFILE_SHRUNK_FL). (not in mainline)
                                 //0x10000000 	Inode has inline data (EXT4_INLINE_DATA_FL).
                                 //0x20000000 	Create children with the same project ID (EXT4_PROJINHERIT_FL).
                                 //0x80000000 	Reserved for ext4 library (EXT4_RESERVED_FL).
                                 //Aggregate flags:
                                 //0x4BDFFF 	User-visible flags.
                                 //0x4B80FF 	User-modifiable flags. Note that while EXT4_JOURNAL_DATA_FL and EXT4_EXTENTS_FL can be set with setattr, they are not in the kernel's EXT4_FL_USER_MODIFIABLE mask, since it needs to handle the setting of these flags in a special manner and they are masked out of the set of flags that are saved directly to i_flags. 
        uint8_t osd1[4];         //Union osd1
        uint8_t i_block[60];     //Block map or extent tree. See the section "The Contents of inode.i_block".
        uint32_t i_generation;   //File version (for NFS).
        uint32_t i_file_acl_lo;  //Lower 32-bits of extended attribute block. ACLs are of course one of many possible extended attributes; I think the name of this field is a result of the first use of extended attributes being for ACLs. 
        uint32_t i_size_high;    //Upper 32-bits of file/directory size. In ext2/3 this field was named i_dir_acl, though it was usually set to zero and never used. 
        uint32_t i_obso_faddr;   //(Obsolete) fragment address. 
        union {
            struct {
                uint16_t l_i_blocks_high; /* were l_i_reserved1 */
                uint16_t l_i_file_acl_high;
                uint16_t l_i_uid_high;    /* these 2 fields */
                uint16_t l_i_gid_high;    /* were reserved2[0] */
                uint16_t l_i_checksum_lo; /* crc32c(uuid+inum+inode) LE */
                uint16_t l_i_reserved;
            } linux2;
            struct {
                uint16_t h_i_reserved1; /* Obsoleted fragment number/size which are removed in ext4 */
                uint16_t h_i_mode_high;
                uint16_t h_i_uid_high;
                uint16_t h_i_gid_high;
                uint32_t h_i_author;
            } hurd2;
            struct {
                uint16_t h_i_reserved1; /* Obsoleted fragment number/size which are removed in ext4 */
                uint16_t m_i_file_acl_high;
                uint32_t m_i_reserved2[2];
            } masix2;
        } osd2;	
        uint16_t i_extra_isize;  //Size of this inode - 128. Alternately, the size of the extended inode fields beyond the original ext2 inode, including this field.
        uint16_t i_checksum_hi;  //Upper 16-bits of the inode checksum.
        uint32_t i_ctime_extra;  //Extra change time bits. This provides sub-second precision. See Inode Timestamps section.
        uint32_t i_mtime_extra;  //Extra modification time bits. This provides sub-second precision.
        uint32_t i_atime_extra;  //Extra access time bits. This provides sub-second precision.
        uint32_t i_crtime;       //File creation time, in seconds since the epoch.
        uint32_t i_crtime_extra; //Extra file creation time bits. This provides sub-second precision.
        uint32_t i_version_hi;   //Upper 32-bits for version number.
        uint32_t i_projid;       //Project ID.
    } __attribute__((packed));

    struct ExtentHeader {
        uint16_t eh_magic; // must equal 0xF30A
        uint16_t eh_entries; // number of entries
        uint16_t eh_max_entries; // max number of entries
        uint16_t eh_depth; // depth
        uint32_t unused;
    } __attribute__((packed));

    struct Extent {
        uint32_t ee_block; // first file block number that this extent covers
        uint16_t ee_len; //Number of blocks covered by extent. If the value of this field is <= 32768, the extent is initialized. If the value of the field is > 32768, the extent is uninitialized and the actual extent length is ee_len - 32768. Therefore, the maximum length of a initialized extent is 32768 blocks, and the maximum length of an uninitialized extent is 32767. 
        uint16_t ee_start_hi;
        uint32_t ee_start_lo;
    } __attribute__((packed));

    struct ExtentIdx {
        uint32_t ei_block;
        uint32_t ei_leaf_lo;
        uint16_t ei_leaf_hi;
        uint16_t ei_unused;
    } __attribute__((packed));

    struct DirEntry {
        uint32_t inode;
        uint16_t size;
        uint8_t nameSize;
        uint8_t fileType;
        char name[];
    } __attribute__((packed));
public:
    enum class FileType {
        UNKNOWN,
        REGULAR_FILE,
        DIRECTORY,
        CHARACTER_DEVICE_FILE,
        BLOCK_DEVICE_FILE,
        FIFO,
        SOCKET,
        SYMBOLIC_LINK
    };

    EXT4() = default;
    EXT4(ATA::Device& device, uint8_t partition);
    ~EXT4();
    EXT4(const EXT4&) = delete;
    EXT4& operator=(const EXT4&) = delete;
    EXT4(EXT4&&) = delete;
    EXT4& operator=(EXT4&&) = delete;

    INode getINode(uint64_t inodeNum);
    uint64_t findFileINode(const char* name);

    void iterateDirectory(const INode& inode, void(*callback)(const DirEntry&));
    void iterateDirectory(const INode& inode, bool(*callback)(void*, const DirEntry&), void* self); ///< return `true` to stop iterating
    GroupDesc getGroupDesc(uint64_t groupNum);
    bool writeGroupDesc(uint64_t groupNum, const GroupDesc& desc);

    bool getSectorInFile(const INode& inode, uint8_t* buffer, uint64_t sector);

    bool setSectorInFile(const INode& inode, uint8_t* buffer, uint64_t sector);
    bool setSizeOfFile(uint64_t inodeNum, uint64_t size);

    bool increaseFileSize(uint64_t inodeNum, uint64_t size);
    bool decreaseFileSize(uint64_t inodeNum, uint64_t size);

    uint64_t findFreeINode();

    bool useBlocks(uint64_t first, uint64_t count, bool value);

    uint32_t getSuperblockChecksum();
    uint16_t getGroupDescChecksum(uint32_t groupNum);

    uint64_t getFileSize(const INode& inode);
    FileType getFileType(const INode& inode);

    inline uint64_t getInodeBitmapSize() const {return inodeBitmapSize;}
    inline uint64_t getBlockBitmapSize() const {return blockBitmapSize;}

    uint32_t generateINodeChecksum(INode, uint64_t inodeNum);

    bool isINodeUsed(uint64_t inodeNum);
    bool isBlockUsed(uint64_t blockNum);
    void setINodeUsed(uint64_t blockNum, bool val);
    void setBlockUsed(uint64_t blockNum, bool val);

    bool writeINode(const INode& node, uint64_t inodeNum);
private:
    bool travelExtentTree(const uint8_t* extentPtr, uint64_t sector, void(*callback)(void*, uint64_t), void* self);

    SuperBlock sb;
    ATA::Device device;
    uint8_t partitionID;
    Partition partition;
    SuperBlock* superBlock;
    uint64_t sectorsPerBlock;
    uint64_t blockSize;
    uint64_t inodeBitmapSize;
    uint64_t blockBitmapSize;
    uint64_t lastBlockBitmapIndex;
    uint8_t* lastBlockBitmap;

    static constexpr uint32_t supportedIncompatFeatures = 0x200 | 0x2 | 0x400;
    static constexpr uint32_t supportedReadOnlycompatFeatures = 0x400 | 0x40 | 0x20 | 0x8 | 0x2 | 0x1;
};

}