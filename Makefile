CC := g++
LD := g++
AR := ar

CC_FLAGS := -Wall -Wextra -Wpedantic -std=c++17 -fno-rtti -fno-exceptions -MMD -MP -g
CC_INCLUDES := \
	-I./include \
	-I./third-party/imgui \
	-I./third-party/imgui/examples

CC_DEFINES :=

AR_FLAGS := rsc

BIN_DIR := bin

# imgroot
IMGROOT_LIB_DIR := $(BIN_DIR)
IMGROOT_LIB_NAME := imgroot
IMGROOT_LIB := $(IMGROOT_LIB_DIR)/lib$(IMGROOT_LIB_NAME).a

# example
EXAMPLE_DIR := $(BIN_DIR)
EXAMPLE_NAME := example
EXAMPLE := $(EXAMPLE_DIR)/$(EXAMPLE_NAME)

all:
	@$(MAKE) --no-print-directory --jobs 8 $(IMGROOT_LIB) $(EXAMPLE)

###########
# ImGroot #
###########

IMGROOT_LIB_SRC_DIR := src
IMGROOT_LIB_OBJ_DIR := $(IMGROOT_LIB_DIR)/obj/imgroot

IMGROOT_LIB_SRC := $(wildcard $(addsuffix /*.cpp, $(IMGROOT_LIB_SRC_DIR)))
IMGROOT_LIB_OBJ := $(addprefix $(IMGROOT_LIB_OBJ_DIR)/, $(addsuffix .o, $(IMGROOT_LIB_SRC)))

$(IMGROOT_LIB): $(IMGROOT_LIB_OBJ)
	@echo [ LD ] $@
	@mkdir --parent $(@D)
	@$(AR) $(AR_FLAGS) $@ $^

$(IMGROOT_LIB_OBJ_DIR)/%.o: %
	@echo [ CC ] $<
	@mkdir --parent $(@D)
	@$(CC) $(CC_FLAGS) $(CC_INCLUDES) $(CC_DEFINES) -c $< -o $@

###########
# Example #
###########

EXAMPLE_SRC_DIR := example third-party/imgui
EXAMPLE_OBJ_DIR := $(EXAMPLE_DIR)/obj/imgroot

EXAMPLE_SRC := $(wildcard $(addsuffix /*.cpp, $(EXAMPLE_SRC_DIR))) \
	third-party/imgui/examples/imgui_impl_glfw.cpp \
	third-party/imgui/examples/imgui_impl_opengl3.cpp

EXAMPLE_OBJ := $(addprefix $(EXAMPLE_OBJ_DIR)/, $(addsuffix .o, $(EXAMPLE_SRC)))

$(EXAMPLE): $(IMGROOT_LIB) $(EXAMPLE_OBJ)
	@echo [ LD ] $@
	@mkdir --parent $(@D)
	@$(LD) $(EXAMPLE_OBJ) -L$(IMGROOT_LIB_DIR) -l$(IMGROOT_LIB_NAME) -lGLEW -lGL -lglfw -lstdc++fs -o $@

$(EXAMPLE_OBJ_DIR)/%.o: %
	@echo [ CC ] $<
	@mkdir --parent $(@D)
	@$(CC) $(CC_FLAGS) $(CC_INCLUDES) $(CC_DEFINES) -c $< -o $@

run: $(EXAMPLE)
	@./$(EXAMPLE)

clean:
	rm -rf $(BIN_DIR)

-include $(IMGROOT_LIB_OBJ:.o=.d)
-include $(EXAMPLE_OBJ:.o=.d)
