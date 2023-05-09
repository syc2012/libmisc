include ../build.include

SRC_DIR = $(BUILD_DIR)/source
INC_DIR = $(BUILD_DIR)/include
LIB_DIR = $(BUILD_DIR)/lib


############
#  Source
############

SRC += $(SRC_DIR)/dump.c
SRC += $(SRC_DIR)/file.c
SRC += $(SRC_DIR)/input.c
SRC += $(SRC_DIR)/log.c
SRC += $(SRC_DIR)/network.c
SRC += $(SRC_DIR)/parser.c
SRC += $(SRC_DIR)/string.c
SRC += $(SRC_DIR)/system.c
SRC += $(SRC_DIR)/time.c

INC += -I$(INC_DIR)

OBJ = $(SRC:.c=.o)


############
#  CFLAGS
############

CFLAGS += -fPIC
CFLAGS += -Wall
CFLAGS += -Wno-unused-result
CFLAGS += -O2
CFLAGS += $(INC)


############
#  LDFLAGS
############

LDFLAGS += -shared


############
#  Build
############

all: $(LIB_DIR)/libmisc.so $(LIB_DIR)/libmisc.a
	$(STRIP) $^

$(LIB_DIR)/libmisc.so: $(OBJ)
	$(CC) $(OBJ) $(LDFLAGS) -o $@

$(LIB_DIR)/libmisc.a: $(OBJ)
	$(AR) rcs $@ $(OBJ)

%.o: %.c $(INC_DIR)/libmisc.h
	$(CC) $(CFLAGS) -c $< -o $@

install:
#	sudo cp -a $(LIB_DIR)/libmisc.so /usr/local/lib/
#	sudo cp    $(INC_DIR)/libmisc.h  /usr/local/include/

clean:
	@rm -f $(LIB_DIR)/libmisc.so $(LIB_DIR)/libmisc.a
	@rm -f $(OBJ)

.PHONY: all clean install
