#include <stdio.h>
#include <stdlib.h>

#include "rscode.h"

//#define DATA "00pd7ae0agvllcquu0hqlid37u5h"

#define DATA "1234567890abcdef0q1duje5ddu2"

//#define DATA 					"1234567890abcdef"

#define BB_CHARACTERS   		"0123456789abcdefghijklmnopqrstuv"

#define BB_THRESHOLD            16


int char_to_num(char c, unsigned int *n)
{
	if (n == NULL)
	{
		return  - 1;
	}

	*n = 0;

	if (c >= 48 && c <= 57)
	{
		*n = c - 48;
		return 0;
	}
	else if (c >= 97 && c <= 122)
	{
		*n = c - 87;
		return 0;
	}
	return	1;
}


int num_to_char(int n, char *c)
{
	if (c != NULL && n >= 0 && n < 64)
	{
		*c = BB_CHARACTERS[n];
		return 0;
	}
	return	1;
}


int main(void)
{
#if 0
	unsigned char src[64] = {0x00};

	char result_with_rs[RS_TOTAL_LEN + 1];

	sprintf(src, DATA);

	unsigned char data[RS_TOTAL_LEN];

	for (int i = 0; i < RS_TOTAL_LEN; i++)
	{
		if (i < RS_DATA_LEN)
		{
			char_to_num(src[i], (unsigned int *) (data + i));
		}
		else
		{
			data[i] = 0;
		}
	}

	unsigned char *code = data + RS_DATA_LEN;

	RS *rs = init_rs(RS_SYMSIZE, RS_GFPOLY, RS_FCR, RS_PRIM, RS_NROOTS, RS_PAD);

    encode_rs_char(rs, data, code);

	for (int i = 0; i < RS_TOTAL_LEN; i++)
	{
		num_to_char(data[i], result_with_rs + i);
	}

	result_with_rs[RS_TOTAL_LEN] = '\0';

	printf("%s\r\n", result_with_rs);
#endif

#if 1
	unsigned char msgBuf[40] = {0x00};

	char tmp[64] = {0x00};
	
	sprintf(tmp,  DATA);

	for(int i = 0; i < strlen(tmp); i++)
	{
		int res_int;
		char_to_num(tmp[i], &res_int);
		msgBuf[i] = res_int;
	}
	
	RS *rs = init_rs(RS_SYMSIZE, RS_GFPOLY, RS_FCR, RS_PRIM, RS_NROOTS, RS_PAD);

	if(rs == NULL)
	{
		printf("Rs code init failed.\r\n");
		return 1;
	}

	int eras_pos[RS_TOTAL_LEN] = {0};

	int count = decode_rs_char(rs, (unsigned char*)msgBuf, eras_pos, 0);

	for(int i = 0; i < RS_TOTAL_LEN; i++)
	{
		//printf("eras_pos[%d] = %d\r\n", i, eras_pos[i]);
	}

	printf("Rscode decoder res is [%d]\r\n", count);

	char finalResult[17] = {0x00};

	for (int i = 0; i < 16; i ++)
	{
		char res_char;
		num_to_char(msgBuf[i], &res_char);
		finalResult[i] = res_char;
	}

	printf("Final Result: [");
	for(int i = 0; i < 16; i++)
	{
		printf("%c", finalResult[i]);
	}
	printf("]\r\n");
	
#endif

	return 0;
}





















