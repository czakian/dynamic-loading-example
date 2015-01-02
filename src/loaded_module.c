#include <stdio.h>

int fib(int n) 
{
  int x, y;

  if (n <= 1) {
    printf("fib recursive completed\n");
    return 1;
  } else {
    x = fib(n - 1);
    y = fib(n - 2);
    return x + y;
  }
}

int fib_accum(int n, int accum1, int accum2) 
{
  if (n <= 1) {
    printf("fib accum completed\n");
    return accum2;
  }

  return fib_accum((n-1), accum2, accum1 + accum2);
}

/**
 * You can dynamically change and recompile this module (via make) 
 * and the changes will get picked up in the main executable (dynamic_loader.exe)
 */
int __dynamic_fn_entry_point__(void *args, void *result) 
{
  int fib_result;
  //fib_result = fib(*(int *) args);
  fib_result = fib_accum(*(int *) args, 1, 1);
  *(int *) result = fib_result;
  return 0;
}

