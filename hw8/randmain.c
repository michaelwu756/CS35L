#include "randcpuid.h"
#include <dlfcn.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

static bool
writebytes (unsigned long long x, int nbytes)
{
  int ndigits = nbytes * 2;
  do
    {
      if (putchar ("0123456789abcdef"[x & 0xf]) < 0)
	return false;
      x >>= 4;
      ndigits--;
    }
  while (0 < ndigits);

  return 0 <= putchar ('\n');
}

/* Main program, which outputs N bytes of random data.  */
int
main (int argc, char **argv)
{
  /* Check arguments.  */
  bool valid = false;
  long long nbytes;
  if (argc == 2)
    {
      char *endptr;
      errno = 0;
      nbytes = strtoll (argv[1], &endptr, 10);
      if (errno)
	perror (argv[1]);
      else
	valid = !*endptr && 0 <= nbytes;
    }
  if (!valid)
    {
      fprintf (stderr, "%s: usage: %s NBYTES\n", argv[0], argv[0]);
      return 1;
    }

  /* If there's no work to do, don't worry about which library to use.  */
  if (nbytes == 0)
    return 0;

  char *lib;
  if(rdrand_supported())
    lib="randlibhw.so";
  else
    lib="randlibsw.so";

  /* Open the shared object */
  void *dl_handle = dlopen( lib, RTLD_LAZY );
  if (dl_handle==NULL) {
    printf( "Error: %s\n", dlerror() );
    return 1;
  }
 
  /* Resolve the symbol (method) from the object */
  unsigned long long (*rand64)(void) = dlsym( dl_handle, "rand64" );
  char *error = dlerror();
  if (error != NULL) {
    printf( "Error: %s\n", error );
    return 1;
  }

  int wordsize = sizeof rand64 ();
  int output_errno = 0;

  do
    {
      unsigned long long x = rand64 ();
      int outbytes = nbytes < wordsize ? nbytes : wordsize;
      if (!writebytes (x, outbytes))
	{
	  output_errno = errno;
	  break;
	}
      nbytes -= outbytes;
    }
  while (0 < nbytes);

  if (fclose (stdout) != 0)
    output_errno = errno;

  dlclose(dl_handle);
  error = dlerror();

  if (error != NULL) {
    printf( "Error: %s\n", error );
    return 1;
  }

  if (output_errno)
    {
      errno = output_errno;
      perror ("output");
      return 1;
    }
  
  return 0;
}
