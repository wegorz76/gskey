/* S/Key can use MD5 now, if defined... */
#include <md5.h>

#define	MDXFinal	MD5Final
#define	MDXInit		MD5Init
#define	MDXUpdate	MD5Update
#define	MDX_CTX		MD5_CTX
