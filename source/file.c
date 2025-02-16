#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
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

/**
 * Current work directory.
 * @returns  Directory path.
 */
char *current_dir(void)
{
    static char _path[PATH_MAX];

    memset(_path, 0, PATH_MAX);
    getcwd(_path, PATH_MAX);

    return _path;
}

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
)
{
#define DEBUG_LIST_DIR (0)
    DIR *pInput;
    struct dirent *pFile;
    char  fullName[1024];
    int   num = 0;
    int   len;

    if ((pInput=opendir( pDir )) == NULL)
    {
        perror( "opendir" );
        printf("%s\n\n", pDir);
        return 0;
    }

    len = strlen( pDir );
    if ((len > 0) && ('/' == pDir[len - 1]))
    {
        pDir[len - 1] = 0x0;
    }

    while ((pFile=readdir( pInput )) != NULL)
    {
        if (!strcmp(pFile->d_name, "." ) ||
            !strcmp(pFile->d_name, ".."))
        {
            continue;
        }

        sprintf(fullName, "%s/%s", pDir, pFile->d_name);
        #if (DEBUG_LIST_DIR)
        printf("%s\n", fullName);
        #endif
        num++;

        if ( pListFunc )
        {
            pListFunc(pDir, pFile->d_name, pFile->d_type, pArg);
        }

        if (( recursive ) &&
            (DT_DIR == pFile->d_type))
        {
            num += list_dir_files(fullName, 1, pListFunc, pArg);
        }
    }

    closedir( pInput );

    return num;
}


/**
 * File existence test.
 * @param [in]  pName  File path name.
 * @returns  True(1) or false(0).
 */
int file_exist(char *pName)
{
    if (-1 == access(pName, F_OK))
    {
        /* file does not exist */
        return 0;
    }

    return 1;
}

/**
 * Get file size.
 * @param [in]  pName  File path name.
 * @returns  Size in bytes.
 */
long file_size(char *pName)
{
    struct stat  statBuf;

    if ( !stat(pName, &statBuf) )
    {
        return statBuf.st_size;
    }

    return 0;
}

/**
 * Extract file path and name.
 * @param [in]  pInput  File path and name.
 * @param [out] pPath.  File path.
 * @param [out] pName.  File name.
 */
void file_path_name(char *pInput, char *pPath, char *pName)
{
    int i = 0;
    int j = -1;
    int len;

    if ((len = strlen(pInput)) > 0)
    {
        for (i=0, j=(len-1); i<len; i++, j--)
        {
            if ('/' == pInput[j])
            {
                pInput[j] = 0x00;
                break;
            }
        }
    }

    if (j < 0)
    {
        if ( pPath ) pPath[0] = 0x00;
        if ( pName ) strcpy(pName, &(pInput[0]));
    }
    else if (j == 0)
    {
        if ( pPath ) sprintf(pPath, "/");
        if ( pName ) strcpy(pName, &(pInput[1]));
    }
    else
    {
        if ( pPath ) sprintf(pPath, "%s/", &(pInput[0]));
        if ( pName ) strcpy(pName, &(pInput[j+1]));
    }
}

/**
 * Text file input.
 * @param [in]  pName  File name.
 * @param [out] pStr   String buffer.
 * @returns  String length.
 */
int file_scanf(char *pName, char *pStr)
{
    FILE *pFile;
    int len = 0;

    pFile = fopen(pName, "r");
    if ( pFile )
    {
        *pStr = 0x00;
        fscanf(pFile, "%s", pStr);
        fclose( pFile );
        len = strlen( pStr );
    }

    return len;
}

/**
 * Text file output.
 * @param [in]  pName  File name.
 * @param [in]  pStr   String buffer.
 * @returns  String length.
 */
int file_printf(char *pName, char *pStr)
{
    FILE *pFile;
    int len = 0;

    pFile = fopen(pName, "w");
    if ( pFile )
    {
        fprintf(pFile, "%s\n", pStr);
        fclose( pFile );
        len = strlen( pStr );
    }

    return len;
}

/**
 * File read.
 * @param [in]  pName   File name.
 * @param [in]  pBuf    Read buffer.
 * @param [in]  bufLen  Read length.
 * @param [in]  offset  Read offset.
 * @returns  Data length.
 */
int file_read(char *pName, void *pBuf, int bufLen, int offset)
{
    FILE *pFile;
    unsigned char *pData = pBuf;
    int i;

    pFile = fopen(pName, "r");
    if (NULL == pFile)
    {
        perror( "fopen" );
        printf("%s\n\n", pName);
        return 0;
    }

    if (offset > 0)
    {
        if (0 != fseek(pFile, offset, SEEK_SET))
        {
            perror( "fseek" );
            printf("offset: %d\n\n", offset);
            fclose( pFile );
            return 0;
        }
    }

    for (i=0; i<bufLen; i++)
    {
        if ( feof( pFile ) ) break;
        if (1 != fread(&(pData[i]), 1, 1, pFile)) break;
    }

    fclose( pFile );

    return i;
}

/**
 * File write.
 * @param [out]  pName    File name.
 * @param [in]   pData    Write buffer.
 * @param [in]   dataLen  Write length.
 * @param [in]   offset   Write offset.
 * @returns  Data length.
 */
int file_write(char *pName, void *pData, int dataLen, int offset)
{
    FILE *pFile;
    unsigned char empty = 0;
    int i;

    pFile = fopen(pName, "w");
    if (NULL == pFile)
    {
        perror( "fopen" );
        printf("%s\n\n", pName);
        return 0;
    }

    for (i=0; i<offset; i++)
    {
        if (1 != fwrite(&empty, 1, 1, pFile))
        {
            perror( "fwrite" );
            printf("offset: %d/%d\n\n", i, offset);
            break;
        }
    }

    if (1 != fwrite(pData, dataLen, 1, pFile))
    {
        perror( "fwrite" );
        printf("dataLen: %d\n\n", dataLen);
    }

    fclose( pFile );

    return (dataLen + i);
}

