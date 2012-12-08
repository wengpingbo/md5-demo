/*
 *file:md5.c
 *author:WEN Pingbo in LZUOSS
 *Email:wengpingbo@gmail.com
 *Date:2012/12/04
 *Notice:This is a md5 demo,give a input string from \
 *main entrace,and a md5 hashing will return.The program \
 *assume the "unsigned long have 4 bytes and usigned char have \
 *1byte",if the condition did't hold,the program may in trouble. \
 *The data in md5 are storaged by follows:
 *1.group of eight bits:MSB
 *2.group of word(four bytes):LSB
 *Copyright:the program is under GNU license, Just use it for free.
 *
 */
#include "md5.h"

//table T value
const UINT32 T[] = 
{
	0xd76aa478,	 /* T1 */
	0xe8c7b756,	 /* T2 */
	0x242070db,	 /* T3 */
	0xc1bdceee,	 /* T4 */
	0xf57c0faf,	 /* T5 */
	0x4787c62a,	 /* T6 */
	0xa8304613,	 /* T7 */
	0xfd469501,	 /* T8 */
	0x698098d8,	 /* T9 */
	0x8b44f7af,	 /* T10 */
	0xffff5bb1,	 /* T11 */
	0x895cd7be,	 /* T12 */
	0x6b901122,	 /* T13 */
	0xfd987193,	 /* T14 */
	0xa679438e,	 /* T15 */
	0x49b40821,	 /* T16 */
	0xf61e2562,	 /* T17 */
	0xc040b340,	 /* T18 */
	0x265e5a51,	 /* T19 */
	0xe9b6c7aa,	 /* T20 */
	0xd62f105d,	 /* T21 */
	0x02441453,	 /* T22 */
	0xd8a1e681,	 /* T23 */
	0xe7d3fbc8,	 /* T24 */
	0x21e1cde6,	 /* T25 */
	0xc33707d6,	 /* T26 */
	0xf4d50d87,	 /* T27 */
	0x455a14ed,	 /* T28 */
	0xa9e3e905,	 /* T29 */
	0xfcefa3f8,	 /* T30 */
	0x676f02d9,	 /* T31 */
	0x8d2a4c8a,	 /* T32 */
	0xfffa3942,	 /* T33 */
	0x8771f681,	 /* T34 */
	0x6d9d6122,	 /* T35 */
	0xfde5380c,	 /* T36 */
	0xa4beea44,	 /* T37 */
	0x4bdecfa9,	 /* T38 */
	0xf6bb4b60,	 /* T39 */
	0xbebfbc70,	 /* T40 */
	0x289b7ec6,	 /* T41 */
	0xeaa127fa,	 /* T42 */
	0xd4ef3085,	 /* T43 */
	0x04881d05,	 /* T44 */
	0xd9d4d039,	 /* T45 */
	0xe6db99e5,	 /* T46 */
	0x1fa27cf8,	 /* T47 */
	0xc4ac5665,	 /* T48 */
	0xf4292244,	 /* T49 */
	0x432aff97,	 /* T50 */
	0xab9423a7,	 /* T51 */
	0xfc93a039,	 /* T52 */
	0x655b59c3,	 /* T53 */
	0x8f0ccc92,	 /* T54 */
	0xffeff47d,	 /* T55 */
	0x85845dd1,	 /* T56 */
	0x6fa87e4f,	 /* T57 */
	0xfe2ce6e0,	 /* T58 */
	0xa3014314,	 /* T59 */
	0x4e0811a1,	 /* T60 */
	0xf7537e82,	 /* T61 */
	0xbd3af235,	 /* T62 */
	0x2ad7d2bb,	 /* T63 */
	0xeb86d391,	 /* T64 */
};

const UINT32 shift_num[]={
	7,12,17,22,
	5,9,14,20,
	4,11,16,23,
	6,10,15,21
};

int main(int argc,char *argv[])
{
	if(argc<=1) 
	{
		fprintf(stderr,"w_md5:No data found,abort!\n");
		return 1;
	}
	//calculate the length of text after append
	int len_origin_8=strlen(argv[1]);
	int len_512=(len_origin_8*8)/512;
	if((len_origin_8-64*len_512<56)&&(len_origin_8!=56)) len_512++;
	else len_512+=2;
	UINT8 input_data[512*len_512/8];
	strcpy(input_data,argv[1]);  //copy input data
	//append padding bits
	input_data[len_origin_8]=128;
	for(int i=len_origin_8+1;i<56+64*(len_512-1);i++)
		input_data[i]=0x0;	
	//append length
	UINT64 len_origin_bit=len_origin_8*8;
	for(int i=0;i<8;i++)
	{
		input_data[len_512*64-1-i]=len_origin_bit&0xff;
		len_origin_bit>>=8;
	}
	for(int i=0,tmp;i<4;i++) //invert the length of bits,lsb
	{
		tmp=input_data[len_512*64-8+i];
		input_data[len_512*64-8+i]=input_data[len_512*64-1-i];
		input_data[len_512*64-1-i]=tmp;	
	}
	//initialize MD cache
	UINT32 cache_a=0x67452301,
		   cache_b=0xefcdab89,
		   cache_c=0x98badcfe,
		   cache_d=0x10325476;
	UINT32 cache_tmp,
		   cache_bak_a,
		   cache_bak_b,
		   cache_bak_c,
		   cache_bak_d;
	//hashing the input data
	for(int i=0;i<len_512;i++)
	{
		cache_bak_a=cache_a;
		cache_bak_b=cache_b;
		cache_bak_c=cache_c;
		cache_bak_d=cache_d;
		for(int j=0;j<4;j++)
		{
			for(int k=0;k<16;k++)
			{
				cache_a=cache_a+
					g(cache_b,cache_c,cache_d,j+1)+
					T[j*16+k]+
					merge_8to32(input_data,i,k,j+1);
				cache_a=shift_left(cache_a,shift_num[j*4+k%4]);
				cache_a+=cache_b;
				cache_tmp=cache_d;
				cache_d=cache_c;
				cache_c=cache_b;
				cache_b=cache_a;
				cache_a=cache_tmp;
			}
		}
		cache_a+=cache_bak_a;
		cache_b+=cache_bak_b;
		cache_c+=cache_bak_c;
		cache_d+=cache_bak_d;
	}
	//invert the digest in lsb
	UINT8 digest[16];
	digest[0]=cache_a&0xff;
	digest[1]=(cache_a>>8)&0xff;
	digest[2]=(cache_a>>16)&0xff;
	digest[3]=(cache_a>>24)&0xff;
	digest[4]=cache_b&0xff;
	digest[5]=(cache_b>>8)&0xff;
	digest[6]=(cache_b>>16)&0xff;
	digest[7]=(cache_b>>24)&0xff;
	digest[8]=cache_c&0xff;
	digest[9]=(cache_c>>8)&0xff;
	digest[10]=(cache_c>>16)&0xff;
	digest[11]=(cache_c>>24)&0xff;
	digest[12]=cache_d&0xff;
	digest[13]=(cache_d>>8)&0xff;
	digest[14]=(cache_d>>16)&0xff;
	digest[15]=(cache_d>>24)&0xff;

	for(int i=0;i<16;i++)
		fprintf(stdout,"%02x",digest[i]);
	fprintf(stdout,"\n");
	return 0;
}

UINT32 shift_left(UINT32 data,UINT32 shift_num)
{
	return (data>>(32-shift_num))+(data<<shift_num);
}

UINT32 merge_8to32(UINT8 *data,
		int num512,int num32,int type)
{
	UINT32 temp=0,result=0x0;
	UINT32 num32_tmp;
	if(type==1) num32_tmp=num32;
	else if(type==2) num32_tmp=(num32*5+1)%16;
	else if(type==3) num32_tmp=(num32*3+5)%16;
	else if(type==4) num32_tmp=(num32*7)%16;
	for(int i=3;i>=0;i--) //lsb in result
	{
		temp=data[num512*64+num32_tmp*4+i];
		temp=temp&0xff;
		result<<=8;
		result+=temp;
	}
	return result;
}

UINT32 g(UINT32 b,UINT32 c,UINT32 d,int type)
{
	UINT32 result;
	if(type==1)
		result=(b&c)|((~b)&d);
	else if(type==2)
		result=(b&d)|(c&(~d));
	else if(type==3)
		result=b^c^d;
	else if(type==4)
		result=c^(b|(~d));
	return result;
}
