#include <stdio.h>
#include <stdlib.h>

#include "rscode.h"

//#define DATA "00pd7ae0agvllcquu0hqlid37u5h"

#define DATA "00pd7kk0agvllcquu0hkkid37kkh"



int main(void)
{
	unsigned char msgBuf[40] = {0x00};

	sprintf((char *)msgBuf,  DATA);
	
	RS *rs = init_rs(RS_SYMSIZE, RS_GFPOLY, RS_FCR, RS_PRIM, RS_NROOTS, RS_PAD);

	printf("%d %d %d %d %d %d\r\n", RS_SYMSIZE, RS_GFPOLY, RS_FCR, RS_PRIM, RS_NROOTS, RS_PAD);
	
	if(rs == NULL)
	{
		return -2;
	}

	int eras_pos[RS_TOTAL_LEN] = {0};

	int count = decode_rs_char(rs, (unsigned char*)msgBuf, eras_pos, 0);

	printf("Result is [%s], Rscode decoder res is [%d]\r\n", msgBuf, count);

	return 0;
}





















