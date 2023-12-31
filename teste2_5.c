#include <stdio.h>
#include "cria_func.h"

#include <stdint.h>
#include <sys/mman.h>
#include <unistd.h>

#define PAGE_START(P) ((intptr_t)(P)&~(pagesize-1))
#define PAGE_END(P) (((intptr_t)(P)+pagesize-1)&~(pagesize-1))

/*
 * The execpage() function shall change the specified memory pages
 * permissions into executable.
 *
 * void *ptr  = pointer to start of memory buff
 * size_t len = memory buff size in bytes
 *
 * The function returns 0 if successful and -1 if any error is encountered.
 * errono may be used to diagnose the error.
 */
int execpage(void *ptr, size_t len) {
	int ret;

	const long pagesize = sysconf(_SC_PAGE_SIZE);
	if (pagesize == -1)
		return -1;

	ret = mprotect((void *)PAGE_START(ptr),
		 PAGE_END((intptr_t)ptr + len) - PAGE_START(ptr),
		 PROT_READ | PROT_WRITE | PROT_EXEC);
	if (ret == -1)
		return -1;

	return 0;
}


typedef int (*func_ptr) ();

char fixa[] = "Era uma vez....\n%s";

unsigned char codigo[120];

int main (void) {
  execpage(codigo, (size_t) 120);
  DescParam params[2];
  func_ptr print;
  char s[] = "Uma historia\n";

  params[0].tipo_val = PTR_PAR; 
  params[0].orig_val = FIX;     
  params[0].valor.v_ptr = fixa;

  params[1].tipo_val = PTR_PAR; 
  params[1].orig_val = IND;   
  params[1].valor.v_ptr = s;
  cria_func (printf, params, 2, codigo);
  print = (func_ptr) codigo;
  print();
  for (int i =0;i < 10;i++)
  {
    s[i] = '.';
    if (i == 3 || i == 7 || i == 10)
    {
      s[i] = '\n';
    }
  }
  s[10] = 'F';
  s[11] = 'I';
  s[12] = 'M';
  s[13] = '\n';
  print();
  return 0;
}


#undef PAGE_START
#undef PAGE_END