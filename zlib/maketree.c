/* maketree.c -- make inffixed.h table for decoding fixed codes
 * Copyright (C) 1998 Mark Adler
 * For conditions of distribution and use, see copyright notice in zlib.h 
 */

/* WARNING: this file should *not* be used by applications. It is
   part of the implementation of the compression library and is
   subject to change. Applications should only use zlib.h.
 */

/* This program is included in the distribution for completeness.
   You do not need to compile or run this program since inffixed.h
   is already included in the distribution.  To use this program
   you need to compile zlib with BUILDFIXED defined and then compile
   and link this program with the zlib library.  Then the output of
   this program can be piped to inffixed.h. */

#include <stdio.h>
#include <stdlib.h>
#include "zutil.h"
#include "inftrees.h"

/* simplify the use of the inflate_huft type with some defines */
#define exop word.what.Exop
#define bits word.what.Bits

/* showtree is only used for debugging purposes */
void showtree(uInt b, inflate_huft *t, int d)
{
  int i, e;
  char *p=_alloca(2*d+1);

  for (i = 0; i < 2*d; i++)
    p[i] = ' ';
  p[i] = 0;
  printf("%s[%d]\n", p, 1<<b);
  for (i = 0; i < (1<<b); i++)
  {
    e = t[i].exop;
    if (e == 0)                 /* simple code */
      printf("%s%d(%d): literal=%d\n", p, i, t[i].bits, t[i].base);
    else if (e & 16)            /* length */
      printf("%s%d(%d): length/distance=%d+(%d)\n",
                p, i, t[i].bits, t[i].base, e & 15);
    else if ((e & 64) == 0)     /* next table */
    {
      printf("%s%d(%d): *sub table*\n", p, i, t[i].bits);
      showtree(e, t + t[i].base, d + 1);
    }
    else if (e & 32)            /* end of block */
      printf("%s%d(%d): end of block\n", p, i, t[i].bits);
    else                        /* bad code */
      printf("%s%d: bad code\n", p, i);
  }
}

/* generate initialization table for an inflate_huft structure array */
void maketree(uInt b, inflate_huft *t)
{
  int i, e;

  i = 0;
  while (1)
  {
    e = t[i].exop;
    if (e && (e & (16+64)) == 0)        /* table pointer */
    {
      fprintf(stderr, "maketree: cannot initialize sub-tables!\n");
      exit(1);
    }
    if (i % 5 == 0)
      printf("\n   ");
    printf(" {{%u,%u},%u}", t[i].exop, t[i].bits, t[i].base);
    if (++i == (1<<b))
      break;
    putchar(',');
  }
  puts("");
}

/* create the fixed tables in C initialization syntax */
void main(void)
{
  int r;
  uInt bl, bd;
  inflate_huft *tl, *td;
  z_stream z;

  z.zalloc = zcalloc;
  z.opaque = (voidpf)0;
  z.zfree = zcfree;
  r = inflate_trees_fixed(&bl, &bd, &tl, &td, &z);
  if (r)
  {
    fprintf(stderr, "inflate_trees_fixed error %d\n", r);
    return;
  }
  /* puts("Literal/Length Tree:");
     showtree(bl, tl, 1);
     puts("Distance Tree:");
     showtree(bd, td, 1); */
  puts("/* inffixed.h -- table for decoding fixed codes");
  puts(" * Generated automatically by the maketree.c program");
  puts(" */");
  puts("");
  puts("/* WARNING: this file should *not* be used by applications. It is");
  puts("   part of the implementation of the compression library and is");
  puts("   subject to change. Applications should only use zlib.h.");
  puts(" */");
  puts("");
  printf("local uInt fixed_bl = %d;\n", bl);
  printf("local uInt fixed_bd = %d;\n", bd);
  printf("local inflate_huft fixed_tl[] = {");
  maketree(bl, tl);
  puts("  };");
  printf("local inflate_huft fixed_td[] = {");
  maketree(bd, td);
  puts("  };");
}
