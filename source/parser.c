#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include "libmisc.h"


// /////////////////////////////////////////////////////////////////////////////
//    Macro declarations
// /////////////////////////////////////////////////////////////////////////////


// /////////////////////////////////////////////////////////////////////////////
//    Type declarations
// /////////////////////////////////////////////////////////////////////////////


// /////////////////////////////////////////////////////////////////////////////
//    Variables declarations
// /////////////////////////////////////////////////////////////////////////////


// /////////////////////////////////////////////////////////////////////////////
//    Functions
// /////////////////////////////////////////////////////////////////////////////

extern unsigned char hex2dec(char ch);

/**
 * Bypass number of tokens in a string.
 * @param [in]  num      Number of tokens.
 * @param [in]  pString  Input string.
 * @returns  String after bypass number of tokens.
 */
char *bypass_token(int num, char *pString)
{
    char  token[TOKEN_SIZE+1];
    char *pNext = pString;
    int   i;

    for (i=0; i<num; i++)
    {
        pNext = first_token(pNext, token, TOKEN_SIZE);
        if (pNext == NULL) 
        {
            break;
        }
    }

    /* Pass space and tab character */
    for (; *pNext && IS_SPACE(*pNext); pNext++);

    return pNext;
}

/**
 * Get the first token from a string.
 * @param [in]   pString  Input string.
 * @param [out]  pToken   Output token.
 * @param [in]   tsize    Output buffer size.
 * @returns  String after remove the first token.
 */
char *first_token(char *pString, char *pToken, int tsize)
{
    int i = 0;

    if (0x0 == pString[0])
    {
        /* This is a NULL line */
        pToken[0] = 0x0;
        return NULL;
    }

    /* Pass space and tab character */
    for (; *pString && IS_SPACE(*pString); pString++);

    /* Pull out the token */
    for (; *pString && !IS_SPACE(*pString) && i<tsize; pString++, i++)
    {
        *pToken++ = *pString;
    }
    *pToken = 0x0;

    return pString;
}

/**
 * Read one line from a text file.
 * @param [in]   pFile  Input file pointer.
 * @param [out]  pLine  Output line string.
 * @param [in]   lsize  Output buffer size.
 * @returns  Success(>= 0) or failure(-1).
 */
int read_line(FILE *pFile, char *pLine, int lsize)
{
    pLine[0] = 0x0;

    if ( feof(pFile) )
    {
        /* end of file */
        return -1;
    }

    /* char *fgets(                                   */
    /*     char *s,      // character array to store  */
    /*     int   n,      // length to read            */
    /*     FILE *stream  // FILE pointer              */
    /* );                                             */
    fgets(pLine, lsize, pFile);

    /* remove the CR/LF character */
    if ((strlen(pLine) > 0) && (pLine[strlen(pLine)-1] == 0x0a))
    {
        pLine[strlen(pLine)-1] = 0x0;
    }
    if ((strlen(pLine) > 0) && (pLine[strlen(pLine)-1] == 0x0d))
    {
        pLine[strlen(pLine)-1] = 0x0;
    }

    return strlen(pLine);
}

/**
 * Read one block of bytes from a binary file.
 * @param [in]   pFile  Input file pointer.
 * @param [out]  pByte  Output buffer.
 * @param [in]   bsize  Output buffer size.
 * @returns  Block size.
 */
int read_bytes(FILE *pFile, unsigned char *pByte, int bsize)
{
    int rsize = 0;
    int i;

    memset(pByte, 0x00, bsize);

    for (i=0; i<bsize; i++)
    {
        /* size_t fread(
        *      void   *ptr,
        *      size_t  size,
        *      size_t  nmemb,
        *      FILE   *stream
        *  );
        */
        fread(&(pByte[i]), 1, 1, pFile);

        if ( feof(pFile) ) break;

        rsize++;
    }

    return rsize;
}

/**
 * Parse a string into tokens.
 * @param [in]  pString     Input string.
 * @param [in]  pParseFunc  Token parsing callback function.
 * @returns  Token number.
 */
int parse_string_into_token(char *pString, tParseTokenCb pParseFunc)
{
    char  token[TOKEN_SIZE+1];
    char *pNext = pString;
    int   count = 0;
    int   action;

    if (NULL == pString)
    {
        printf("%s: pString is NULL\n", __func__);
        return 0;
    }

    do
    {
        pNext = first_token(pNext, token, TOKEN_SIZE);
        if ( token[0] )
        {
            count++;
            action = pParseFunc(token, strlen( token ), count);

            if (PARSE_STOP == action) break;
        }
    } while ( pNext );

    return count;
}

/**
 * Parse a text file into lines.
 * @param [in]  pFileName   Input file name.
 * @param [in]  pParseFunc  Line parsing callback function.
 * @returns  Line number.
 */
int parse_file_into_line(char *pFileName, tParseLineCb pParseFunc)
{
    char  line[LINE_SIZE+1];
    FILE *pInput = NULL;
    int   count = 0;
    int   action;

    if ((pInput=fopen(pFileName, "r")) == NULL)
    {
        printf("%s: cannot open file %s\n\n", __func__, pFileName);
        return 0;
    }

    /* start reading input file */
    while (read_line(pInput, line, LINE_SIZE) >= 0)
    {
        if ( line[0] )
        {
            count++;
            action = pParseFunc(line, strlen( line ), count);

            if (PARSE_STOP == action) break;
        }
    }

    fclose( pInput );

    return count;
}

/**
 * Parse a binary file into bytes.
 * @param [in]  pFileName   Input file name.
 * @param [in]  pParseFunc  Byte parsing callback function.
 * @param [in]  size        Block size.
 * @returns  File size.
 */
int parse_file_into_byte(char *pFileName, tParseByteCb pParseFunc, int size)
{
    FILE *pInput = NULL;
    unsigned char *pBuf;
    unsigned int   offset = 0;
    int   bytes;
    int   action;

    pBuf = malloc( size );
    if (NULL == pBuf)
    {
        printf("%s: fail to alloc memory (%d)\n\n", __func__, size);
        return 0;
    }

    if ((pInput=fopen(pFileName, "r")) == NULL)
    {
        printf("%s: cannot open file %s\n\n", __func__, pFileName);
        free( pBuf );
        return 0;
    }

    /* start reading input file */
    while ( (bytes = read_bytes(pInput, pBuf, size)) )
    {
        action = pParseFunc(pBuf, bytes, offset);
        offset += bytes;

        if (PARSE_STOP == action) break;
    }

    fclose( pInput );

    free( pBuf );

    return offset;
}

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
)
{
    FILE *pInput = NULL;
    char  line[LINE_SIZE+1];
    char  token[TOKEN_SIZE+1];
    char *pNext;

    unsigned char  nibbleH;
    unsigned char  nibbleL;
    unsigned int   len;
    int  more;


    if ((pInput=fopen(pFileName, "r")) == NULL)
    {
        printf("%s: cannot open file %s\n\n", __func__, pFileName);
        return 0;
    }

    /* start reading input file */
    len = 0;
    do
    {
        more = read_line(pInput, line, LINE_SIZE);
        if (more > 0)
        {
            pNext = line;
            do
            {
                pNext = first_token(pNext, token, TOKEN_SIZE);
                if ((0x0 == token[0]) || ('#' == token[0]))
                {
                    /* ignore the null or comment line */
                    break;
                }

                if (len >= bufSize)
                {
                    printf("%s: un-enough buffer size (%u)\n\n", __func__, bufSize);
                    goto _EXIT_PARSE;
                }

                if (strlen( token ) > 2)
                {
                    printf("%s: wrong HEX token (%s)\n\n", __func__, token);
                    goto _EXIT_PARSE;
                }
                else if (strlen( token ) > 1)
                {
                    nibbleH = hex2dec( token[0] );
                    nibbleL = hex2dec( token[1] );

                    pBuf[len++] = ((nibbleH << 4) | (nibbleL));
                }
                else
                {
                    pBuf[len++] = hex2dec( token[0] );
                }
            } while ( pNext );
        }
    } while (more != -1);

_EXIT_PARSE:
    fclose( pInput );

    return len;
}

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
)
{
    FILE *pInput = NULL;
    char  line[LINE_SIZE+1];
    char *pFirst;
    char *pNext;

    unsigned int  count = 0;
    int  more;
    int  i;


    if ((pInput=fopen(pFileName, "r")) == NULL)
    {
        printf("%s: cannot open file %s\n\n", __func__, pFileName);
        return 0;
    }

    /* start reading input file */
    do
    {
        more = read_line(pInput, line, LINE_SIZE);
        if (more > 0)
        {
            for (pFirst=line; *pFirst && IS_SPACE(*pFirst); pFirst++);

            pNext = NULL;
            for (i=0; i<strlen(pFirst); i++)
            {
                if ( IS_SPACE(pFirst[i]) )
                {
                    pFirst[i] = 0x00;
                    pNext = &(pFirst[i + 1]);
                    break;
                }
            }

            pBuf[count].real = strtod(pFirst, NULL);
            pBuf[count].imag = (pNext ? strtod(pNext, NULL) : 0);

            count++;
            if (count >= bufSize) break;
        }
    } while (more != -1);

    fclose( pInput );

    return count;
}

