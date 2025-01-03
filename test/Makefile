include ../build.include

SRC_DIR = $(BUILD_DIR)/test
INC_DIR = $(BUILD_DIR)/include
LIB_DIR = $(BUILD_DIR)/lib


############
#  CFLAGS
############

CFLAGS += -Wall
CFLAGS += -I$(INC_DIR)


############
#  LDFLAGS
############

LDFLAGS += -L$(LIB_DIR)
LDFLAGS += -lmisc


############
#  Build
############

APPS += parse_byte
APPS += parse_line
APPS += parse_token
APPS += parse_hex
APPS += parse_complex
APPS += parse_shell

all: $(APPS)
	$(STRIP) $^

parse_byte: parse_byte.o
	$(CC) $< $(LDFLAGS) -o $@

parse_line: parse_line.o
	$(CC) $< $(LDFLAGS) -o $@

parse_token: parse_token.o
	$(CC) $< $(LDFLAGS) -o $@

parse_hex: parse_hex.o
	$(CC) $< $(LDFLAGS) -o $@

parse_complex: parse_complex.o
	$(CC) $< $(LDFLAGS) -o $@

parse_shell: parse_shell.o
	$(CC) $< $(LDFLAGS) -o $@

%.o: %.c $(INC_DIR)/libmisc.h
	$(CC) $(CFLAGS) -c $<

clean:
	@rm -f $(APPS) *.o

.PHONY: all clean
