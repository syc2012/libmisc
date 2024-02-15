#ifndef __LIBMISC_H__
#define __LIBMISC_H__

#include <stdio.h>
#include <dirent.h>


// /////////////////////////////////////////////////////////////////////////////
//    Macro declarations
// /////////////////////////////////////////////////////////////////////////////

#define LOGINFO( a... )    log_print(L_INFO, ##a)
#define LOGWARN( a... )    log_print(L_WARN, ##a)
#define LOGERROR( a... )   log_print(L_ERROR, ##a)
#define LOGDEBUG( a... )   log_print(L_DEBUG, ##a)
#define LOGTRACE()         log_print(L_TRACE, "%s %u\n", __func__, __LINE__)
#define LOGDUMP(var, len)  mem_dump(#var, var, len, 0)
#define LOGASCII(var, len) mem_dump(#var, var, len, 1)


#define IS_UPPER_ALHPABET(ch) ((ch >= 'A') && (ch <= 'Z'))
#define IS_LOWER_ALHPABET(ch) ((ch >= 'a') && (ch <= 'z'))
#define IS_NUMBER(ch)         ((ch >= '0') && (ch <= '9'))
#define IS_SPACE(ch)          ((ch == ' ') || (ch == '\t'))
#define LINE_SIZE             (1023)
#define TOKEN_SIZE            (255)

#define SKIP_SPACE(in, out)   for (out=in; *out && IS_SPACE(*out); out++)


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


#define DIV_CEIL(x, y)  ceil((double)(x) / (double)(y))
#define DIV_FLOOR(x, y) floor((double)(x) / (double)(y))
#define DIV_MOD(x, y)   ((x) - ((y) * DIV_FLOOR(x, y)))
#define DIV_REM(x, y)   ((x) % (y))


#define CPLX_ADD(ina, inb, outc) \
{ \
    outc.real = (ina.real + inb.real); \
    outc.imag = (ina.imag + inb.imag); \
}

#define CPLX_SUB(ina, inb, outc) \
{ \
    outc.real = (ina.real - inb.real); \
    outc.imag = (ina.imag - inb.imag); \
}

#define CPLX_MUL(ina, inb, outc) \
{ \
    double real, imag; \
    real = ((ina.real * inb.real) - (ina.imag * inb.imag)); \
    imag = ((ina.imag * inb.real) + (ina.real * inb.imag)); \
    outc.real = real; \
    outc.imag = imag; \
}

#define CPLX_DIV(ina, inb, outc) \
{ \
    double real, imag; \
    double temp = ((inb.real * inb.real) + (inb.imag * inb.imag)); \
    real = ((ina.real * inb.real) + (ina.imag * inb.imag)) / temp; \
    imag = ((ina.imag * inb.real) - (ina.real * inb.imag)) / temp; \
    outc.real = real; \
    outc.imag = imag; \
}

#define CPLX_SQUARE(in, out) \
{ \
    double real, imag; \
    real = (pow(in.real, 2) - pow(in.imag, 2)); \
    imag = (2 * in.real * in.imag); \
    out.real = real; \
    out.imag = imag; \
}

#define CPLX_POW( in ) \
    (pow(in.real, 2) + pow(in.imag, 2))

#define CPLX_ABS( in ) \
    sqrt(pow(in.real, 2) + pow(in.imag, 2))

#define CPLX_ANGLE( in ) \
    atan2(in.imag, in.real)


// /////////////////////////////////////////////////////////////////////////////
//    Type declarations
// /////////////////////////////////////////////////////////////////////////////

typedef enum
{
    L_NONE  = 0x00,
    L_INFO  = 0x01,
    L_WARN  = 0x02,
    L_ERROR = 0x04,
    L_DEBUG = 0x08,
    L_TRACE = 0x10,
    L_ALL   = 0x1F
} tLogMask;

typedef enum
{
    PARSE_CONTINUE = 0,
    PARSE_STOP
} tParseAction;

typedef int (*tParseByteCb)(unsigned char *pBuf, int size, unsigned int offset);
typedef int (*tParseLineCb)(char *pStr, int len, int count);
typedef int (*tParseTokenCb)(char *pStr, int len, int count);
typedef void (*tListDirCb)(char *pDir, char *pName, int type, void *pArg);

typedef struct _tComplex
{
    double  real;
    double  imag;
} tComplex;

typedef struct _tFlexMem
{
    unsigned char *pBuf;
    unsigned int   len;
    unsigned int   lenMax;
} tFlexMem;


// /////////////////////////////////////////////////////////////////////////////
//    Variables declarations
// /////////////////////////////////////////////////////////////////////////////


// /////////////////////////////////////////////////////////////////////////////
//    Functions
// /////////////////////////////////////////////////////////////////////////////

/**
 * Log output option.
 * @param [in]  mask  A @ref tLogMask enumeration.
 */
void log_option(int mask);

/**
 * Log print function.
 * @param [in]  mask     A @ref tLogMask enumeration.
 * @param [in]  pFormat  Print format.
 */
void log_print(int mask, char *pFormat, ...);


/**
 * Dump memory.
 * @param [in]  pDesc  Description string.
 * @param [in]  pAddr  Memory address.
 * @param [in]  size   Memory size.
 * @param [in]  ascii  Print ASCII (boolean).
 */
void mem_dump(char *pDesc, void *pAddr, unsigned int size, int ascii);

/**
 * Print complex number sequence.
 * @param [in]  pDesc  Description string.
 * @param [in]  pSeq   Complex number sequence.
 * @param [in]  len    Complex number sequence length.
 * @param [in]  eForm  Scientific notation.
*/
void cplx_dump(char *pDesc, tComplex *pSeq, unsigned int len, int eForm);


/**
 * Generate a random between min. and max. value.
 * @param [in]  min  Min. value.
 * @param [in]  max  Max. value.
 * @returns  Random number.
 */
int rand_num(int min, int max);

/**
 * Test system endian.
 * @returns  Big(1) or litte(0) endian.
 */
int endian(void);

/**
 * Test system bit number.
 * @returns  Bits.
*/
int bitsize(void);


/**
 * Sleep in milli-second(s).
 * @param [in]  ms  milli-second.
 */
void msleep(unsigned int ms);

/**
 * Get time stamp.
 * @returns  Time stamp string.
 */
char *time_stamp(void);

/**
 * Set time mark point A.
 * @param [in]  id  Mark ID (0 ~ 15).
 */
void time_markA(int id);

/**
 * Set time mark point B.
 * @param [in]  id  Mark ID (0 ~ 15).
 */
void time_markB(int id);

/**
 * Get time elapsed from point A to B.
 * @param [in]  id  Mark ID (0 ~ 15).
 * @returns  Time elapsed in nsec.
 */
long time_elapse(int id);


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
 * Extract file path and name.
 * @param [in]  pInput  File path and name.
 * @param [out] pPath.  File path.
 * @param [out] pName.  File name.
 */
void file_path_name(char *pInput, char *pPath, char *pName);

/**
 * Text file input.
 * @param [in]  pName  File name.
 * @param [out] pStr   String buffer.
 * @returns  String length.
 */
int file_scanf(char *pName, char *pStr);

/**
 * Text file output.
 * @param [in]  pName  File name.
 * @param [in]  pStr   String buffer.
 * @returns  String length.
 */
int file_printf(char *pName, char *pStr);

/**
 * File read.
 * @param [in]  pName   File name.
 * @param [in]  pBuf    Read buffer.
 * @param [in]  bufLen  Read length.
 * @param [in]  offset  Read offset.
 * @returns  Data length.
 */
int file_read(char *pName, void *pBuf, int bufLen, int offset);

/**
 * File write.
 * @param [out]  pName    File name.
 * @param [in]   pData    Write buffer.
 * @param [in]   dataLen  Write length.
 * @param [in]   offset   Write offset.
 * @returns  Data length.
 */
int file_write(char *pName, void *pData, int dataLen, int offset);


/**
 * Remove CR, LF in the end of string.
 * @param [in,out]  pStr  String buffer.
 */
void removeCRLF(char *pStr);

/**
 * Convert HEX to DEC.
 * @param [in]  ch  HEX character (0 ~ F).
 * @returns  Integer 0 ~ 15.
 */
unsigned char hex2dec(char ch);


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
 * Get network device IP address.
 * @param [in]   pDev  Network device name.
 * @param [out]  pIp   IP address (string).
 * @returns  Success(0) or failure(-1).
 */
int netdev_getIpAddr(char *pDev, char *pIp);

/**
 * Get network device MAC address.
 * @param [in]   pDev  Network device name.
 * @param [out]  pMac  MAC address (6 bytes).
 * @returns  Success(0) or failure(-1).
 */
int netdev_getMacAddr(char *pDev, unsigned char *pMac);

/**
 * Get network device MTU.
 * @param [in]   pDev  Network device name.
 * @param [out]  pMtu  MTU size.
 * @returns  Success(0) or failure(-1).
 */
int netdev_getMtu(char *pDev, int *pMtu);


/**
* Flexible memory initial.
* @param [in]  pMem  A @ref tFlexMem object.
*/
void fmem_init(tFlexMem *pMem);

/**
* Flexible memory un-initial.
* @param [in]  pMem  A @ref tFlexMem object.
*/
void fmem_uninit(tFlexMem *pMem);

/**
* Flexible memory resize.
* @param [in]  pMem   A @ref tFlexMem object.
* @param [in]  bytes  Memory buffer size.
* @returns  Memory buffer.
*/
void *fmem_resize(tFlexMem *pMem, unsigned int bytes);


#endif  /* __LIBMISC_H__ */
