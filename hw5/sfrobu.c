#include<stdlib.h>
#include<stdio.h>
#include<ctype.h>
#include<unistd.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<limits.h>

int comparisons=0;

int frobcmphelp(void const* a, void const* b)
{
  return frobcmp(*(char const**)a,*(char const**)b,0);
}

int frobcmphelpupper(void const* a, void const* b)
{
  return frobcmp(*(char const**)a,*(char const**)b,1);
}

int frobcmp(char const* a, char const* b, int upper)
{
  comparisons++;
  int i=0;
  int key = 42;
  while(a[i]!=' ' && b[i]!=' ')
  {
    int firstval = a[i]^key;
    int secondval = b[i]^key;
    if(upper==1)
    {
      if((a[i]^key>0 && a[i]^key<UCHAR_MAX) || a[i]^key==EOF)
	firstval=toupper(a[i]^key);
      if((b[i]^key>0 && b[i]^key<UCHAR_MAX) || b[i]^key==EOF)
	secondval=toupper(b[i]^key);
    }
    if(firstval-secondval!=0)
      return firstval-secondval;
    i++;
  }
  if(a[i]==' ' && b[i]==' ')
    return 0;
  else if (a[i]==' ')
    return -1;
  else
    return 1;
}

void writeError(char* str)
{
  fprintf(stderr, "%s", str);
  exit(1);
}

int addString(char*** lines, int size)
{
  char** newMem = realloc(*lines,size*sizeof(char*));
  if(newMem==NULL)
    writeError("Error: Memory error\n");    
  *lines = newMem;
  return size;
}

int makeBuffer(char** buffer, int size, int new)
{
  char* newMem;
  if(new==1)
    newMem = malloc(size);
  else
    newMem = realloc(*buffer,size);
  if(newMem==NULL)
    writeError("Error: Memory error\n");
  *buffer = newMem;
  return size;
}

char* readStdin(int* bufferSize)
{
  struct stat fileInfo;
  if(fstat(STDIN_FILENO,&fileInfo) < 0)    
    writeError("Error: Cannot get stdin file info\n");

  char* buffer;
  int size;
  if(fileInfo.st_size>=1)
    size = makeBuffer(&buffer,fileInfo.st_size,1);
  else
    size = makeBuffer(&buffer,1,1);

  int i=0;
  while(read(STDIN_FILENO,(buffer+i),1)!=0)
  {
    i++;
    if(i>=size)
      size = makeBuffer(&buffer,2*size,0);
  }
  buffer[i]=' ';
  *bufferSize = i;
  if(i==0)
  {
    free(buffer);
    return NULL;
  }
  return buffer;
}

char** parseInput(char* buffer, int bufferSize, size_t *numElements)
{
  char** lines=malloc(sizeof(char*));
  if(lines==NULL)
    writeError("Error: Memory error\n");
  
  if(buffer==NULL)
  {
    *numElements=0;
    return lines;
  }  

  int numStrings=1;
  lines[0]=buffer;
  int c;
  for(c=0;c<bufferSize-1; c++)
  {
    if(buffer[c]==' ')
    {
      numStrings=addString(&lines,numStrings+1);
      lines[numStrings-1]=(buffer+c+1);
    }
  }
  *numElements=(size_t)numStrings;
  return lines;
}

int main(int argc, char** argv)
{
  int ignoreCase=0;
  if(argc==2 && strcmp(argv[1],"-f")==0)
    ignoreCase=1;
  else if(argc>2 || (argc==2 && strcmp(argv[1],"-f")!=0))
    writeError("Error: Bad arguments\n");
  
  int bufferSize;
  char* buffer = readStdin(&bufferSize);
 
  size_t num=0;
  char** lines = parseInput(buffer,bufferSize,&num);
  if(ignoreCase==1)
    qsort(lines, num, sizeof(char*), frobcmphelpupper);
  else
    qsort(lines, num, sizeof(char*), frobcmphelp);
  int i;
  for(i=0;i<num;i++)
  {
    int j = 0;
    while(lines[i][j]!=' ')
    {
      write(STDOUT_FILENO, lines[i]+j, 1);
      j++;
    }
    write(STDOUT_FILENO, lines[i]+j, 1);
  }
  printf("\n%i\n",comparisons);
  free(buffer);
  free(lines);
  exit(0);
}
