c_files := $(shell find src -name *.c)
c_object_files := $(patsubst src/%.c, build/c_object_files/%.o, $(c_files))


cpp_files := $(shell find src -name *.cpp)
cpp_object_files := $(patsubst src/%.cpp, build/cpp_object_files/%.o, $(cpp_files))

asm_source_files := $(shell find src -name *.asm)
asm_object_files := $(patsubst src/%.asm, build/asm_object_files/%.o, $(asm_source_files))

object_files := $(asm_object_files) $(cpp_object_files) $(c_object_files)

$(c_object_files): build/c_object_files/%.o : src/%.c
	mkdir -p $(dir $@) && \
	x86_64-elf-gcc -c -I header -ffreestanding $(patsubst build/c_object_files/%.o, src/%.c, $@) -o $@

$(cpp_object_files): build/cpp_object_files/%.o : src/%.cpp
	mkdir -p $(dir $@) && \
	x86_64-elf-g++ -c -I header -std=c++17 -Wno-multichar -Wno-literal-suffix -fno-exceptions -fno-rtti -fno-common -mno-red-zone -mgeneral-regs-only -ffreestanding $(patsubst build/cpp_object_files/%.o, src/%.cpp, $@) -o $@

$(asm_object_files): build/asm_object_files/%.o : src/%.asm
	mkdir -p $(dir $@) && \
	nasm -f elf64 $(patsubst build/asm_object_files/%.o, src/%.asm, $@) -o $@

.PHONY: build-x86_64
build-x86_64: $(object_files)
	mkdir -p dist && \
	x86_64-elf-ld -n -o dist/kernel.bin -T targets/linker.ld $(object_files) && \
	objdump -dC dist/kernel.bin > dist/kernel.asm && \
	cp dist/kernel.bin targets/iso/boot/kernel.bin && \
	grub-mkrescue /usr/lib/grub/i386-pc -o dist/kernel.iso targets/iso