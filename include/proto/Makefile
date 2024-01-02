# Makefile for proto directory

# Define the source directory
PROTO_SRC_DIR := proto

# Define the output directories
CONF_OUTPUT_DIR := $(PROTO_SRC_DIR)/conf
MSG_OUTPUT_DIR := $(PROTO_SRC_DIR)/msg

# Define the source files
CONF_PROTO_FILES := $(PROTO_SRC_DIR)/confproto/person.proto
MSG_PROTO_FILES := $(PROTO_SRC_DIR)/msgproto/request.proto

# Define the output C files
CONF_C_FILES := $(patsubst $(PROTO_SRC_DIR)/%.proto,$(CONF_OUTPUT_DIR)/%.pb-c.c,$(CONF_PROTO_FILES))
CONF_H_FILES := $(patsubst $(PROTO_SRC_DIR)/%.proto,$(CONF_OUTPUT_DIR)/%.pb-c.h,$(CONF_PROTO_FILES))

MSG_C_FILES := $(patsubst $(PROTO_SRC_DIR)/%.proto,$(MSG_OUTPUT_DIR)/%.pb-c.c,$(MSG_PROTO_FILES))
MSG_H_FILES := $(patsubst $(PROTO_SRC_DIR)/%.proto,$(MSG_OUTPUT_DIR)/%.pb-c.h,$(MSG_PROTO_FILES))

# Define the protobuf compiler
PROTOC := protoc

# Define flags for the protobuf compiler
PROTOC_FLAGS := --c_out=$(PROTO_SRC_DIR)

# Define the rule to generate C files from proto files
$(CONF_OUTPUT_DIR)/%.pb-c.c $(CONF_OUTPUT_DIR)/%.pb-c.h: $(PROTO_SRC_DIR)/%.proto
	@mkdir -p $(CONF_OUTPUT_DIR)
	$(PROTOC) $(PROTOC_FLAGS) --proto_path=$(PROTO_SRC_DIR) $<

$(MSG_OUTPUT_DIR)/%.pb-c.c $(MSG_OUTPUT_DIR)/%.pb-c.h: $(PROTO_SRC_DIR)/%.proto
	@mkdir -p $(MSG_OUTPUT_DIR)
	$(PROTOC) $(PROTOC_FLAGS) --proto_path=$(PROTO_SRC_DIR) $<

# Define the default target
all: conf msg

# Define the target to generate conf files
conf: $(CONF_C_FILES) $(CONF_H_FILES)

# Define the target to generate msg files
msg: $(MSG_C_FILES) $(MSG_H_FILES)

# Define the clean target
clean:
	rm -rf $(CONF_OUTPUT_DIR) $(MSG_OUTPUT_DIR)
