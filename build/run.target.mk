# This file is generated by gyp; do not edit.

TOOLSET := target
TARGET := run
DEFS_Debug := \
	'-DNODE_GYP_MODULE_NAME=run' \
	'-DUSING_UV_SHARED=1' \
	'-DUSING_V8_SHARED=1' \
	'-DV8_DEPRECATION_WARNINGS=1' \
	'-D_LARGEFILE_SOURCE' \
	'-D_FILE_OFFSET_BITS=64' \
	'-DBUILDING_NODE_EXTENSION' \
	'-DDEBUG' \
	'-D_DEBUG'

# Flags passed to all source files.
CFLAGS_Debug := \
	-fPIC \
	-pthread \
	-Wall \
	-Wextra \
	-Wno-unused-parameter \
	-m64 \
	-std=c++11 \
	-std=c++11 \
	-g \
	-O0

# Flags passed to only C files.
CFLAGS_C_Debug := \
	-std=c++11

# Flags passed to only C++ files.
CFLAGS_CC_Debug := \
	-fno-rtti \
	-std=gnu++0x \
	-std=c++11

INCS_Debug := \
	-I/home/oleg/.node-gyp/4.2.6/include/node \
	-I/home/oleg/.node-gyp/4.2.6/src \
	-I/home/oleg/.node-gyp/4.2.6/deps/uv/include \
	-I/home/oleg/.node-gyp/4.2.6/deps/v8/include \
	-I$(srcdir)/node_modules/nan

DEFS_Release := \
	'-DNODE_GYP_MODULE_NAME=run' \
	'-DUSING_UV_SHARED=1' \
	'-DUSING_V8_SHARED=1' \
	'-DV8_DEPRECATION_WARNINGS=1' \
	'-D_LARGEFILE_SOURCE' \
	'-D_FILE_OFFSET_BITS=64' \
	'-DBUILDING_NODE_EXTENSION'

# Flags passed to all source files.
CFLAGS_Release := \
	-fPIC \
	-pthread \
	-Wall \
	-Wextra \
	-Wno-unused-parameter \
	-m64 \
	-std=c++11 \
	-std=c++11 \
	-O3 \
	-ffunction-sections \
	-fdata-sections \
	-fno-omit-frame-pointer

# Flags passed to only C files.
CFLAGS_C_Release := \
	-std=c++11

# Flags passed to only C++ files.
CFLAGS_CC_Release := \
	-fno-rtti \
	-std=gnu++0x \
	-std=c++11

INCS_Release := \
	-I/home/oleg/.node-gyp/4.2.6/include/node \
	-I/home/oleg/.node-gyp/4.2.6/src \
	-I/home/oleg/.node-gyp/4.2.6/deps/uv/include \
	-I/home/oleg/.node-gyp/4.2.6/deps/v8/include \
	-I$(srcdir)/node_modules/nan

OBJS := \
	$(obj).target/$(TARGET)/run.o

# Add to the list of files we specially track dependencies for.
all_deps += $(OBJS)

# CFLAGS et al overrides must be target-local.
# See "Target-specific Variable Values" in the GNU Make manual.
$(OBJS): TOOLSET := $(TOOLSET)
$(OBJS): GYP_CFLAGS := $(DEFS_$(BUILDTYPE)) $(INCS_$(BUILDTYPE))  $(CFLAGS_$(BUILDTYPE)) $(CFLAGS_C_$(BUILDTYPE))
$(OBJS): GYP_CXXFLAGS := $(DEFS_$(BUILDTYPE)) $(INCS_$(BUILDTYPE))  $(CFLAGS_$(BUILDTYPE)) $(CFLAGS_CC_$(BUILDTYPE))

# Suffix rules, putting all outputs into $(obj).

$(obj).$(TOOLSET)/$(TARGET)/%.o: $(srcdir)/%.cpp FORCE_DO_CMD
	@$(call do_cmd,cxx,1)

# Try building from generated source, too.

$(obj).$(TOOLSET)/$(TARGET)/%.o: $(obj).$(TOOLSET)/%.cpp FORCE_DO_CMD
	@$(call do_cmd,cxx,1)

$(obj).$(TOOLSET)/$(TARGET)/%.o: $(obj)/%.cpp FORCE_DO_CMD
	@$(call do_cmd,cxx,1)

# End of this set of suffix rules
### Rules for final target.
LDFLAGS_Debug := \
	-pthread \
	-rdynamic \
	-m64

LDFLAGS_Release := \
	-pthread \
	-rdynamic \
	-m64

LIBS :=

$(obj).target/run.node: GYP_LDFLAGS := $(LDFLAGS_$(BUILDTYPE))
$(obj).target/run.node: LIBS := $(LIBS)
$(obj).target/run.node: TOOLSET := $(TOOLSET)
$(obj).target/run.node: $(OBJS) FORCE_DO_CMD
	$(call do_cmd,solink_module)

all_deps += $(obj).target/run.node
# Add target alias
.PHONY: run
run: $(builddir)/run.node

# Copy this to the executable output path.
$(builddir)/run.node: TOOLSET := $(TOOLSET)
$(builddir)/run.node: $(obj).target/run.node FORCE_DO_CMD
	$(call do_cmd,copy)

all_deps += $(builddir)/run.node
# Short alias for building this executable.
.PHONY: run.node
run.node: $(obj).target/run.node $(builddir)/run.node

# Add executable to "all" target.
.PHONY: all
all: $(builddir)/run.node

