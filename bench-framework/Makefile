CPP=g++
STD=-std=c++20
CFLAGS=-Wall -O3 $(STD)
# Removed deprecated compute_70 and added Wno-deprecated-gpu-targets
NVCCFLAGS=-ccbin $(CPP) $(STD) -Wno-deprecated-gpu-targets -gencode=arch=compute_75,code=sm_75 -gencode=arch=compute_80,code=sm_80 -gencode=arch=compute_89,code=sm_89 -gencode=arch=compute_90,code=sm_90
# Added ./include to INCLUDE paths
INCLUDE=/usr/local/cuda/include ./src ./include
LDFLAGS=
LIBS=
LIBDIRS=/usr/local/cuda/lib64
HEADERS=$(shell find ./src -name '*.hpp' -o -name '*.cuh')
KERNELS=$(shell find ./src -name '*.cu')
# Changed output paths for object files to use bin directory
KERNEL_OBJS=$(patsubst ./src/%.cu,./bin/%.obj,$(KERNELS))
CPP_SOURCES=$(shell find ./src -name '*.cpp')
CPP_OBJS=$(patsubst ./src/%.cpp,./bin/%.obj,$(CPP_SOURCES))
# Move the target to the bin directory
TARGET=$(BIN_DIR)/gol_bench
# Directory for build artifacts
BIN_DIR=./bin
# Default empty value for command-line arguments
ARGS ?=

.PHONY: all clear clean purge run mkdirs

# Added mkdirs as a dependency to ensure bin directory exists
all: mkdirs $(TARGET)

# Create necessary directories
mkdirs:
	@mkdir -p $(BIN_DIR)
	@for file in $(CPP_SOURCES) $(KERNELS); do \
		dir=$$(dirname $${file#./src/}); \
		mkdir -p $(BIN_DIR)/$$dir; \
	done

# Building Targets
$(TARGET): $(CPP_OBJS) $(KERNEL_OBJS)
	@echo Compiling and linking executable "$@" ...
	@$(CPP) $(CFLAGS) $(addprefix -I,$(INCLUDE)) $(LDFLAGS) $(addprefix -L,$(LIBDIRS)) $(addprefix -l,$(LIBS)) -lcudart $^ -o $@

# Updated paths for C++ object files with directory creation
$(BIN_DIR)/%.obj: ./src/%.cpp $(HEADERS)
	@echo Compiling "$<" ...
	@mkdir -p $(dir $@)
	@$(CPP) $(CFLAGS) $(addprefix -I,$(INCLUDE)) -c $< -o $@

# Updated paths for CUDA object files with directory creation
$(BIN_DIR)/%.obj: ./src/%.cu
	@echo Compiling CUDA kernels in "$<" ...
	@mkdir -p $(dir $@)
	@nvcc $(NVCCFLAGS) $(addprefix -I,$(INCLUDE)) --compile -cudart static $< -o $@

run: $(TARGET)
	@$(TARGET) $(ARGS)

# Cleaning Stuff
clear:
	@echo Removing object files ...
	-@rm -rf $(BIN_DIR)

clean: clear

purge: clear
	@echo Removing executable ...
	-@rm -f $(TARGET)
