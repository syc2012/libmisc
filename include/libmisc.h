#ifndef __LIBMISC_H__
#define __LIBMISC_H__

#include <stdio.h>
#include <dirent.h>


// /////////////////////////////////////////////////////////////////////////////
//    Macro declarations
// /////////////////////////////////////////////////////////////////////////////

#define IS_UPPER_ALHPABET(ch) ((ch >= 'A') && (ch <= 'Z'))
#define IS_LOWER_ALHPABET(ch) ((ch >= 'a') && (ch <= 'z'))
#define IS_NUMBER(ch)         ((ch >= '0') && (ch <= '9'))
#define IS_SPACE(ch)          ((ch == ' ') || (ch == '\t'))
#define LINE_SIZE             (1023)
#define TOKEN_SIZE            (255)


#define DIR_UNKNOWN_FILE_TYPE  DT_UNKNOWN
#define DIR_NAMED_PIPE         DT_FIFO
#define DIR_CHARACTER_DEVICE   DT_CHR
#define DIR_DIRECTORY          DT_DIR
#define DIR_BLOCK_DEVICE       DT_BLK
#define DIR_REGULAR_FILE       DT_REG
#define DIR_SYMBOLIC_LINK      DT_LNK
#define DIR_UNIX_DOMAIN_SOCKET DT_SOCK
#define DIR_WHITEOUT_INODE     DT_WHT


#define BYTES_TO_U16(pBuf, val) \
    do { \
        val  = (*((unsigned char *)pBuf    ) << 8); \
        val |= (*((unsigned char *)pBuf + 1)     ); \
    } while (0)

#define U16_TO_BYTES(val, pBuf) \
    do { \
        *((unsigned char *)pBuf    ) = ((val >> 8) & 0xFF); \
        *((unsigned char *)pBuf + 1) = ((val     ) & 0xFF); \
    } while (0)

#define BYTES_TO_U32(pBuf, val) \
    do { \
        val  = (*((unsigned char *)pBuf    ) << 24); \
        val |= (*((unsigned char *)pBuf + 1) << 16); \
        val |= (*((unsigned char *)pBuf + 2) <<  8); \
        val |= (*((unsigned char *)pBuf + 3)      ); \
    } while (0)

#define U32_TO_BYTES(val, pBuf) \
    do { \
        *((unsigned char *)pBuf    ) = ((val >> 24) & 0xFF); \
        *((unsigned char *)pBuf + 1) = ((val >> 16) & 0xFF); \
        *((unsigned char *)pBuf + 2) = ((val >>  8) & 0xFF); \
        *((unsigned char *)pBuf + 3) = ((val      ) & 0xFF); \
    } while (0)


#define DIV_CEIL(x, y)  (((x) + ((y) - 1)) / (y))
#define DIV_FLOOR(x, y) ((x) / (y))


// /////////////////////////////////////////////////////////////////////////////
//    Type declarations
// /////////////////////////////////////////////////////////////////////////////

typedef enum
{
    PARSE_CONTINUE = 0,
    PARSE_STOP
} tParseAction;

typedef struct _tComplex
{
    double  real;
    double  imag;
} tComplex;

typedef int (*tParseByteCb)(unsigned char *pBuf, int size, unsigned int offset);
typedef int (*tParseLineCb)(char *pStr, int len, int count);
typedef int (*tParseTokenCb)(char *pStr, int len, int count);
typedef void (*tListDirCb)(char *pDir, char *pName, int type, void *pArg);


// /////////////////////////////////////////////////////////////////////////////
//    Variables declarations
// /////////////////////////////////////////////////////////////////////////////


// /////////////////////////////////////////////////////////////////////////////
//    Functions
// /////////////////////////////////////////////////////////////////////////////

/**
 * Generate a random between min. and max. value.
 * @param [in]  min  Min. value.
 * @param [in]  max  Max. value.
 * @returns  Random number.
 */
int rand_num(int min, int max);

/**
 * Get time stamp.
 * @returns  Time stamp string.
 */
char *time_stamp(void);


/**
 * Get one character from keyboard input.
 * @returns  Character value.
 */
int getch(void);

/**
 * Wait for a key board event.
 */
void stop(void);


/**
 * Bypass number of tokens in a string.
 * @param [in]  num      Number of tokens.
 * @param [in]  pString  Input string.
 * @returns  String after bypass number of tokens.
 */
char *bypass_token(int num, char *pString);

/**
 * Get the first token from a string.
 * @param [in]   pString  Input string.
 * @param [out]  pToken   Output token.
 * @param [in]   tsize    Output buffer size.
 * @returns  String after remove the first token.
 */
char *first_token(char *pString, char *pToken, int tsize);

/**
 * Parse a string into tokens.
 * @param [in]  pString     Input string.
 * @param [in]  pParseFunc  Token parsing callback function.
 * @returns  Token number.
 */
int parse_string_into_token(char *pString, tParseTokenCb pParseFunc);

/**
 * Parse a text file into lines.
 * @param [in]  pFileName   Input file name.
 * @param [in]  pParseFunc  Line parsing callback function.
 * @returns  Line number.
 */
int parse_file_into_line(char *pFileName, tParseLineCb pParseFunc);

/**
 * Parse a binary file into bytes.
 * @param [in]  pFileName   Input file name.
 * @param [in]  pParseFunc  Byte parsing callback function.
 * @param [in]  size        Block size.
 * @returns  File size.
 */
int parse_file_into_byte(char *pFileName, tParseByteCb pParseFunc, int size);

/**
 * Parse a HEX string file to byte array.
 * @param [in]   pFileName  Input file name.
 * @param [out]  pBuf       Byte array buffer.
 * @param [in]   bufSize    Byte array buffer size.
 * @returns  Data length.
 */
unsigned int parse_hex_string_file(
    char          *pFileName,
    unsigned char *pBuf,
    unsigned int   bufSize
);

/**
 * Parse a complex string file to array.
 * @param [in]   pFileName  Input file name.
 * @param [out]  pBuf       Complex array buffer.
 * @param [in]   bufSize    Complex array buffer size.
 * @returns  Data count.
 */
unsigned int parse_complex_string_file(
    char         *pFileName,
    tComplex     *pBuf,
    unsigned int  bufSize
);


/**
 * List all files in a directory.
 * @param [in]  pDir       Directory path.
 * @param [in]  recursive  Boolean.
 * @param [in]  pListFunc  List directory callback function.
 * @param [in]  pArg       Argument for callback function.
 * @returns  Number of files.
 */
int list_dir_files(
    char       *pDir,
    int         recursive,
    tListDirCb  pListFunc,
    void       *pArg
);


/**
 * Convert IP address data to string.
 * @param [in]   ver      IP version (4 or 6).
 * @param [in]   pAddr    IP address data (byte array).
 * @param [out]  pBuf     IP address string buffer.
 * @param [in]   bufSize  IP address string buffer size.
 * @returns  String length.
 */
int ip2str(int ver, void *pAddr, char *pBuf, int bufSize);

/**
 * Convert IP address string to byte array.
 * @param [in]   ver    IP version (4 or 6).
 * @param [in]   pStr   IP address string.
 * @param [out]  pAddr  IP address data buffer (byte array).
 * @returns  Data length.
 */
int str2ip(int ver, char *pStr, void *pAddr);


/**
 * Convert byte array data to HEX string.
 * @param [in]   pHex     Byte array data.
 * @param [in]   hexSize  Byte array data size.
 * @param [out]  pBuf     String buffer.
 * @param [in]   bufSize  String buffer size.
 * @returns  String length.
 */
int hex2str(void *pHex, int hexSize, char *pBuf, int bufSize);

/**
 * Convert HEX string to byte array.
 * @param [in]   pStr     HEX string.
 * @param [out]  pBuf     Byte array buffer.
 * @param [in]   bufSize  Byte array buffer size.
 * @returns  Data length.
 */
int str2hex(char *pStr, unsigned char *pBuf, int bufSize);


/**
 * Convert byte array data to PLMN ID string.
 * @param [in]   pPlmn     Byte array data.
 * @param [in]   plmnSize  Byte array data size.
 * @param [out]  pBuf      String buffer.
 * @param [in]   bufSize   String buffer size.
 * @returns  String length.
 */
int plmn2str(void *pPlmn, int plmnSize, char *pBuf, int bufSize);

/**
 * Convert PLMN ID string to byte array.
 * @param [in]   pStr     PLMN ID string.
 * @param [out]  pBuf     Byte array buffer.
 * @param [in]   bufSize  Byte array buffer size.
 * @returns  Data length.
 */
int str2plmn(char *pStr, unsigned char *pBuf, int bufSize);


/**
 * File existence test.
 * @param [in]  pName  File path name.
 * @returns  True(1) or false(0).
 */
int file_exist(char *pName);

/**
 * Get file size.
 * @param [in]  pName  File path name.
 * @returns  Size in bytes.
 */
long file_size(char *pName);


/**
 * Dump memory.
 * @param [in]  pDesc  Description string.
 * @param [in]  pAddr  Memory address.
 * @param [in]  size   Memory size.
 */
void mem_dump(char *pDesc, void *pAddr, unsigned int size);


#endif  /* __LIBMISC_H__ */
