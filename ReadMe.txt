[ Directory Structure ]

$(BUILD_DIR)
  |-- include (include header path)
  |-- lib     (link library path)
  |-- source  (source code)
  `-- test    (example programs)


[ Source Code ]

dump.c
  void mem_dump(char *pDesc, void *pAddr, unsigned int size, int ascii);
  void cplx_dump(char *pDesc, tComplex *pSeq, unsigned int len);
  void stack_dump(char *pDesc);

file.c
  char *current_dir(void);
  int list_dir_files(
      char       *pDir,
      int         recursive,
      tListDirCb  pListFunc,
      void       *pArg
  );
  int file_exist(char *pName);
  long file_size(char *pName);
  void file_path_name(char *pInput, char *pPath, char *pName);
  int file_scanf(char *pName, char *pStr);
  int file_printf(char *pName, char *pStr);
  int file_read(char *pName, void *pBuf, int bufLen, int offset);
  int file_write(char *pName, void *pData, int dataLen, int offset);

input.c
  int getch(void);
  void stop(void);

log.c
  void log_option(int mask);
  void log_print(int mask, char *pFormat, ...);

network.c
  int netdev_getIpAddr(char *pDev, char *pIp);
  int netdev_getMacAddr(char *pDev, unsigned char *pMac);
  int netdev_getMtu(char *pDev, int *pMtu);

memory.c
  void fmem_init(tFlexMem *pMem);
  void fmem_uninit(tFlexMem *pMem);
  void *fmem_resize(tFlexMem *pMem, unsigned int bytes);

parser.c
  char *bypass_token(int num, char *pString);
  char *first_token(char *pString, char *pToken, int tsize);
  int parse_string_into_token(char *pString, tParseTokenCb pParseFunc);
  int parse_file_into_line(char *pFileName, tParseLineCb pParseFunc);
  int parse_file_into_byte(char *pFileName, tParseByteCb pParseFunc, int size);
  unsigned int parse_hex_string_file(
      char          *pFileName,
      unsigned char *pBuf,
      unsigned int   bufSize
  );
  unsigned int parse_complex_string_file(
      char         *pFileName,
      tComplex     *pBuf,
      unsigned int  bufSize
  );

string.c
  int ip2str(int ver, void *pAddr, char *pBuf, int bufSize);
  int str2ip(int ver, char *pStr, void *pAddr);
  int hex2str(void *pHex, int hexSize, char *pBuf, int bufSize);
  int str2hex(char *pStr, unsigned char *pBuf, int bufSize);
  int plmn2str(void *pPlmn, int plmnSize, char *pBuf, int bufSize);
  int str2plmn(char *pStr, unsigned char *pBuf, int bufSize);

system.c
  int rand_num(int min, int max);
  int endian(void);
  int bitsize(void);

time.c
  void msleep(unsigned int ms);
  char *time_stamp(void);
  void time_markA(int id);
  void time_markB(int id);
  long time_elapse(int id);


[ Build Command ]

$ source setup.gcc
$ make

