#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <math.h>

typedef unsigned char BYTE;
typedef unsigned int  UINT;
typedef UINT MD5_SUB_ARRAY[16];
typedef UINT MD5_TRANSORM_FUNC(UINT,UINT,UINT);
typedef struct
{
    UINT    a;
    UINT    b;
    UINT    c;
    UINT    d;
    MD5_SUB_ARRAY   sub_array;
}MD5_TRANSFORM_PARAM;

const double    MAX_INT = (double)0xFFFFFFFF + 1.0;

const UINT MD5_TRANSFORM_MATRIX[4][16][3] =
{
   
    {
    { 0, 7, 1}, { 1,12, 2}, { 2,17, 3}, { 3,22, 4},
    { 4, 7, 5}, { 5,12, 6}, { 6,17, 7}, { 7,22, 8},
    { 8, 7, 9}, { 9,12,10}, {10,17,11}, {11,22,12},
    {12, 7,13}, {13,12,14}, {14,17,15}, {15,22,16},
    },
   
    {
    { 1, 5,17}, { 6, 9,18}, {11,14,19}, { 0,20,20},
    { 5, 5,21}, {10, 9,22}, {15,14,23}, { 4,20,24},
    { 9, 5,25}, {14, 9,26}, { 3,14,27}, { 8,20,28},
    {13, 5,29}, { 2, 9,30}, { 7,14,31}, {12,20,32},
    },
   
    {
    {5, 4, 33}, { 8,11,34}, {11,16,35},{14, 23,36},
    {1, 4, 37}, { 4,11,38}, { 7,16,39},{10, 23,40},
    {13,4, 41}, { 0,11,42}, { 3,16,43},{ 6, 23,44},
    {9, 4, 45}, {12,11,46}, {15,16,47},{ 2, 23,48},
    },
   
    {
    { 0,6,49},  { 7,10,50}, {14,15,51},{ 5, 21,52},
    {12,6,53},  { 3,10,54}, {10,15,55},{ 1, 21,56},
    { 8,6,57},  {15,10,58}, { 6,15,59},{13, 21,60},
    { 4,6,61},  {11,10,62}, { 2,15,63},{ 9, 21,64},
    },
};

static UINT MD5_TRANSFORM_ARRAY[65];

void MD5_Init()
{
    int x;
    for(x = 1; x <= 64; x++)
    {
        MD5_TRANSFORM_ARRAY[x] = (UINT)(MAX_INT * fabs(sin(x)));
    }
}

UINT F(UINT x,UINT y,UINT z)
{
    return ((x & y) | ((~x) & z));
}

UINT G(UINT x,UINT y,UINT z)
{
    return ((x & z) | (y & (~z)));
}

UINT H(UINT x,UINT y,UINT z)
{
    return (x ^ y ^ z);
}

UINT I(UINT x,UINT y,UINT z)
{
    return (y ^ (x | (~z)));
}



BYTE* MD5_prepare_data(const BYTE* data,int len,int* new_len)
{
    int rest,fill,size;
    BYTE* new_data;
    UINT  bit_len;

    // (1) 字节补齐
    rest = len % 56;
    if (rest <= 56) fill = 56 - rest;
    else fill = (64 - rest) + 56;

    new_data = (BYTE*)malloc(len + fill + 8);
    if (NULL == new_data) return NULL;

    if (len > 0)    memcpy(new_data,data,len);
    if (fill > 0)   memset(new_data + len,0x80,1);
    if (fill > 1)   memset(new_data + len + 1,0,fill - 1);

    size = fill + len;

    // (2) 附加数据的比特长度
    bit_len = len * 8;
    // (64位二进制数表示的)比特长度的低32位
    memset(new_data + size + 0,(bit_len & 0x000000FF),      1);
    memset(new_data + size + 1,(bit_len & 0x0000FF00) >> 8, 1);
    memset(new_data + size + 2,(bit_len & 0x00FF0000) >> 16,1);
    memset(new_data + size + 3,(bit_len & 0xFF000000) >> 24,1);
    // 不考虑比特长度超出32位无符号数表示范围,所以高32位总是0
    memset(new_data + size + 4,0,4);

    *new_len = size + 8;

    return new_data;
}

void MD5_transform(MD5_TRANSFORM_PARAM* param,int ring,MD5_TRANSORM_FUNC func)
{
    UINT a,b,c,d,s,k,i;
    UINT abcd[4];
    UINT *X,*T;
    int index;

    abcd[0] = param->a;
    abcd[1] = param->b;
    abcd[2] = param->c;
    abcd[3] = param->d;
    X = param->sub_array;
    T = MD5_TRANSFORM_ARRAY;

    for(index = 0; index < 16; index++)
    {
        a = abcd[(3 * index + 0) % 4];
        b = abcd[(3 * index + 1) % 4];
        c = abcd[(3 * index + 2) % 4];
        d = abcd[(3 * index + 3) % 4];

        k = MD5_TRANSFORM_MATRIX[ring][index][0];
        s = MD5_TRANSFORM_MATRIX[ring][index][1];
        i = MD5_TRANSFORM_MATRIX[ring][index][2];

        a = a + func(b,c,d) + X[k] + T[i];
        a = ( a << s) | ( a >> (32 - s)); // 循环左移
        a = a + b;

        abcd[(3 * index + 0) % 4] = a;
    }

    param->a = abcd[0];
    param->b = abcd[1];
    param->c = abcd[2];
    param->d = abcd[3];
}

extern "C" char* MyMD5(const BYTE* data,int len)
{
    int  x,y,new_len;
    MD5_TRANSFORM_PARAM param;
    UINT AA,BB,CC,DD;
    BYTE* buf;
    static char ret_md5[32];

    if(len <=0 || data==NULL)return NULL;
    
    MD5_Init();

    buf = MD5_prepare_data(data,len,&new_len);
    if (buf == NULL) return NULL;

    AA = 0x67452301;
    BB = 0xefcdab89;
    CC = 0x98badcfe;
    DD = 0x10325476;

    for(x = 0; x < new_len / 64; x++)
    {
        param.a = AA;
        param.b = BB;
        param.c = CC;
        param.d = DD;

        for(y = 0; y < 16; y++)
        {
            param.sub_array[y]  = buf[64 * x + 4 * y + 0];
            param.sub_array[y] += buf[64 * x + 4 * y + 1] << 8;
            param.sub_array[y] += buf[64 * x + 4 * y + 2] << 16;
            param.sub_array[y] += buf[64 * x + 4 * y + 3] << 24;
        }
        MD5_transform(&param,0,F);
        MD5_transform(&param,1,G);
        MD5_transform(&param,2,H);
        MD5_transform(&param,3,I);

        AA += param.a;
        BB += param.b;
        CC += param.c;
        DD += param.d;
    }

#if 0
    printf("MD5(\"%s\")=",data);

    printf("%02X%02X%02X%02X",
        (AA & 0x000000FF),
        (AA & 0x0000FF00) >> 8,
        (AA & 0x00FF0000) >> 16,
        (AA & 0xFF000000) >> 24);

    printf("%02X%02X%02X%02X",
        (BB & 0x000000FF),
        (BB & 0x0000FF00) >> 8,
        (BB & 0x00FF0000) >> 16,
        (BB & 0xFF000000) >> 24);

    printf("%02X%02X%02X%02X",
        (CC & 0x000000FF),
        (CC & 0x0000FF00) >> 8,
        (CC & 0x00FF0000) >> 16,
        (CC & 0xFF000000) >> 24);

    printf("%02X%02X%02X%02X",
        (DD & 0x000000FF),
        (DD & 0x0000FF00) >> 8,
        (DD & 0x00FF0000) >> 16,
        (DD & 0xFF000000) >> 24);

    printf("\n");
#else
    memcpy(ret_md5,&AA,4);
    memcpy(ret_md5+4,&BB,4);
    memcpy(ret_md5+8,&CC,4);
    memcpy(ret_md5+12,&DD,4);
#endif

    return ret_md5;
}

