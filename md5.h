/*
 *
 *file:md5.h
 *email:wengpingbo@gmail.com
 *date:2012/12/08 in lzuoss
 *
 */

#ifndef MD5_H
#define MD5_H 1

#include <stdio.h>
#include <string.h>

typedef unsigned long int UINT32;
typedef unsigned char UINT8;
typedef unsigned long long UINT64;
//table T value
extern const UINT32 T[]; 
extern const UINT32 shift_num[];

UINT32 merge_8to32(UINT8 *,int,int,int);
UINT32 g(UINT32,UINT32,UINT32,int type);
UINT32 shift_left(UINT32,UINT32);

#endif
