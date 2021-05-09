.DEFAULT_GOAL := build

cpp_source_files := $(shell find src -name *.cpp)
cpp_object_files := $(patsubst src/%.cpp, build/cpp_object_files/%.o, $(cpp_source_files))

program_cpp_files := $(shell find $(program_dir)/src -name *.cpp)
program_object_files := $(patsubst $(program_dir)/src/%.cpp, $(program_dir)/build/cpp_object_files/%.o, $(program_cpp_files))

object_files := $(cpp_object_files) $(program_object_files)

$(program_object_files): $(program_dir)/build/cpp_object_files/%.o : $(program_dir)/src/%.cpp
	mkdir -p $(dir $@) && \
	x86_64-elf-g++ -c -I header -Wno-literal-suffix -std=c++17 -fno-exceptions -mgeneral-regs-only -mno-red-zone -mgeneral-regs-only -ffreestanding $(patsubst $(program_dir)/build/cpp_object_files/%.o, $(program_dir)/src/%.cpp, $@) -o $@

$(cpp_object_files): build/cpp_object_files/%.o : src/%.cpp
	mkdir -p $(dir $@) && \
	x86_64-elf-g++ -c -I header -Wno-literal-suffix -std=c++17 -fno-exceptions -mgeneral-regs-only -mno-red-zone -mgeneral-regs-only -ffreestanding $(patsubst build/cpp_object_files/%.o, src/%.cpp, $@) -o $@

.PHONY: build
build: $(object_files)
	x86_64-elf-ld -n -o $(program_dir)/$(notdir $(program_dir)).elf -T linker.ld $(object_files)