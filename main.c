/**************************************************************************
**
**	The author disclaims copyright to this source code.
** 	In place of a legal notice, here is a bless in:
**
**	May you do good and not evil.
**	May you find forgiveness for yourself and forgive others.
**	May you share freely, never taking more than you give.
**
*************************************************************************/

/*
 * File:   Main.h
 * Author: CAI
 * Created on 2018/5/11, 10:00pm
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "rscode.h"

#define SOFTWARE_VERSION    "1.0.01"
#define SOFTWARE_Name     "ReedSolomon"

void PrintUsage(int argc, char * *argv)
{
  char DTChar[100] =
  {
    0x00
  };

  if (argc == 2)
  {
    strcpy(DTChar, argv[1]);

    if (!strcasecmp(DTChar, "--help"))
    {
      printf("Stuff specific to the 8-bit symbol version of the general purpose RS codecs\r\n");
	  printf("Supported characters would be [%s]\r\n", BB_CHARACTERS);
      printf("Usage: %s --encode [16-bit data]\r\n", SOFTWARE_Name);
      printf("Usage: %s --decode [28-bit data]\r\n", SOFTWARE_Name);
      exit(1);
    }
  }
}


void PrintHelp()
{
  printf("Invalid argument, optional parameters:\r\n");
  printf("--help Usage Information\r\n");
  printf("--version Version Information\r\n");
  exit(1);
}


void PrintVersionInfo(int argc, char * *argv)
{
  char DTChar[100] = {0x00};

  if (argc == 2)
  {
    strcpy(DTChar, argv[1]);

    if (!strcasecmp(DTChar, "--version"))
    {
      char date[32] = __DATE__;

      struct tm t;
      memset(&t, 0, sizeof(t));
      strptime(date, "%b %d %Y", &t);
      t.tm_mon      += 1;
      printf("Application Name: %s\r\n"
             "Application Version: %s\r\n"
             "Compile Date: %04d-%02d-%2d %s\r\n",
             SOFTWARE_Name, SOFTWARE_VERSION, t.tm_year + 1900, t.tm_mon, t.tm_mday, __TIME__);
      exit(0);
    }
  }
}

int char_to_num(char c, unsigned int * n)
{
  if (n == NULL)
  {
    return - 1;
  }

  *n = 0;

  if (c >= 48 && c <= 57)
  {
    *n = c - 48;
    return 0;
  }

  else if (c >= 65 && c <= 90)
  {
    *n = c - 65;
    return 0;
  }

  else if (c >= 97 && c <= 122)
  {
    *n = c - 87;
    return 0;
  }

  return 1;
}


int num_to_char(int n, char * c)
{
  if (c != NULL && n >= 0 && n < 64)
  {
    *c = BB_CHARACTERS[n];
    return 0;
  }

  return 1;
}


int Encode(const char *src)
{
  char result_with_rs[RS_TOTAL_LEN + 1];

  unsigned char data[RS_TOTAL_LEN];

  for (int i = 0; i < RS_TOTAL_LEN; i++)
  {
    if (i < RS_DATA_LEN)
    {
      char_to_num(src[i], (unsigned int *)(data + i));
    }
    else
    {
      data[i] = 0x00;
    }
  }

  unsigned char * code = data + RS_DATA_LEN;

  RS *rs  = init_rs(RS_SYMSIZE, RS_GFPOLY, RS_FCR, RS_PRIM, RS_NROOTS, RS_PAD);

  if (rs == NULL)
  {
    printf("Rs code init failed.\r\n");
    return 1;
  }

  encode_rs_char(rs, data, code);

  for (int i = 0; i < RS_TOTAL_LEN; i++)
  {
    num_to_char(data[i], result_with_rs + i);
  }

  result_with_rs[RS_TOTAL_LEN] = '\0';

  printf("Result with Rscode [%s]\r\n", result_with_rs);

  return 0;
}


int Decode(const char *src)
{

  unsigned char msgBuf[40] = {0x00};

  for (int i = 0; i < RS_TOTAL_LEN; i++)
  {
    int res_int;
    char_to_num(src[i], &res_int);
    msgBuf[i] = res_int;
  }

  RS *rs  = init_rs(RS_SYMSIZE, RS_GFPOLY, RS_FCR, RS_PRIM, RS_NROOTS, RS_PAD);

  if (rs == NULL)
  {
    printf("Rs code init failed.\r\n");
    return 1;
  }

  int eras_pos[RS_TOTAL_LEN] = {0};

  int count = decode_rs_char(rs, (unsigned char *) msgBuf, eras_pos, 0);

  printf("Rscode decoder res is [%d]\r\n", count);

  char finalResult[RS_DATA_LEN+1] = {0x00};

  for (int i = 0; i < RS_DATA_LEN; i++)
  {
    char res_char;
    num_to_char(msgBuf[i], &res_char);
    finalResult[i] = res_char;
  }

  printf("Final Result: [");

  for (int i = 0; i < RS_DATA_LEN; i++)
  {
    printf("%c", finalResult[i]);
  }

  printf("]\r\n");

  return 0;
}


int main(int argc, char * *argv)
{

  switch (argc)
  {
    case 2:
    {
      PrintVersionInfo(argc, argv);
      PrintUsage(argc, argv);
      printf("Wrong Instruction\r\n");
      break;
    }
    case 3:
    {
      int ret = 0;

	  printf("%s %s %d\r\n", argv[1], argv[2], strlen(argv[2]));
	  	
      if (!strcmp(argv[1], "--encode") && strlen(argv[2])==16)
      {
        ret = Encode(argv[2]);
		return ret;
      }
	  
      if (!strcmp(argv[1], "--decode") && strlen(argv[2])==28)
      {
        ret = Decode(argv[2]);
		return ret;
      }
      printf("Wrong Param\r\n");
      break;
    }
    default:
    {
      PrintHelp();
      break;
    }
  }
  return 0;
}





















