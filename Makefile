# Set parent folder name as target elf
obj-elf := $(shell basename $(shell pwd))

# Set the folder that includes your source files
SRCDIR=./src

### Simplified CFLAGS
# Includes will be prefixed with -I automatically
INCLUDES ?= src 
# Defines will be prefixed with -D automatically
DEFINES ?=

### Simplified LDFLAGS
# Libpaths will be prefixed with -L automatically
LIBPATHS ?= 
# Libpaths will be prefixed with -l automatically
# Add libraries like pthread to LLINK directly
LIBRARIES ?= SDL2 SDL2_image SDL2_ttf m

### Linked libraries
LLINK := $(LIBRARIES:%=-l%)

### CFLAGS
CFLAGS := -Wextra -Wall -Og -g
CFLAGS += $(addprefix -I, $(INCLUDES))
CFLAGS += $(addprefix -D, $(DEFINES))

CXX_FLAGS = $(CFLAGS) -std=c++17

### LDFLAGS
LDFLAGS ?= -Wl,--start-group $(addprefix -l, $(DEFINES)) -Wl,--end-group

obj-c := $(addsuffix .o, $(basename $(shell find $(SRCDIR) -name *.c)))
obj-cpp := $(addsuffix .o, $(basename $(shell find $(SRCDIR) -name *.cpp)))
obj-asm := $(addsuffix .o, $(basename $(shell find $(SRCDIR) -name *.s)))

### Include makefile-subdirs
# -include src/subdir.mk

obj-o := $(obj-c:%.c=%.o)
obj-o += $(obj-cpp:%.cpp=%.o)
obj-o += $(obj-asm:%.s=%.o)

PHONY += all
all: $(obj-elf)

%.o: %.cpp
	$(CXX) $(CXX_FLAGS) -c $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# g++ -o YourProgName YourSource.cpp -lX11 -lGL -lpthread -lpng -lstdc++fs -std=c++17
$(obj-elf): $(obj-o)
	$(CXX) $(LDFLAGS) $^ -o $@ $(LIBPATHS) $(LLINK)

PHONY += clean
clean:
	$(RM) -Rf $(obj-elf) $(obj-o)

.PHONY: $(PHONY)
