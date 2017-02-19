#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main(int argc, char** argv)
{
  if(argc<3)
  {
    fprintf(stderr, "Error: Not enough arguments\n");
    exit(1);
  }
  char* from = argv[1];
  char* to = argv[2];
  if(strlen(from)!=strlen(to))
  {
    fprintf(stderr, "Error: Arguments must be the same length\n");
    exit(1);
  }

  int length = strlen(from);
  int i;
  for(i=0; i<length; i++)
  {
    int j;
    for(j=i-1; j>=0; j--)
      if(from[j]==from[i])
      {
	fprintf(stderr, "Error: Cannot have duplicate characters in first argument\n");
	exit(1);
      }
  }

  int c = getchar();
  while(c!=EOF)
  {
    int replaced = 0;
    for(i=0; i<length; i++)
    {
      if(c==from[i])
      {
	putchar(to[i]);
	replaced = 1;
      }
    }
    if(replaced!=1)
      putchar(c); 
    c=getchar();
  }
  exit(0);
}
