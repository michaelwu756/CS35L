#include<stdio.h>
#include<stdlib.h>

int frobcmphelp(void const* a, void const* b)
{
  return frobcmp(*(char const**)a,*(char const**)b);
}

int frobcmp(char const* a, char const* b)
{
  int i=0;
  int key = 42;
  while(a[i]!=' ' && b[i]!=' ')
  {
    if((a[i]^key)-(b[i]^key)!=0)
      return (a[i]^key)-(b[i]^key);
    i++;
  }
  if(a[i]==' ' && b[i]==' ')
    return 0;
  else if (a[i]==' ')
    return -1;
  else
    return 1;
}

void checkWidths(char*** lines, int x, int y, int* w)
{
   if(y==*w)
   {
      *w+=*w;
      int i;
      for(i=0;i<x+1;i++)
      {
	char* newMem = realloc((*lines)[i],*w+1);
	if(newMem==NULL)
	  exit(1);
	(*lines)[i]=newMem;
      }
    }
}

void addString(char*** lines, int x, int w)
{
  char** newMem = realloc(*lines,(x+1)*sizeof(char*));
  char* newMem2 = malloc(w+1);
  if(newMem==NULL || newMem2 == NULL)
    exit(1);
  *lines = newMem;
  (*lines)[x] = newMem2;
}

char** parseInput(size_t *numElements)
{
  char **lines=malloc(sizeof(char*));
  if(lines==NULL)
    exit(1);
  int outerIndex = 0;
  int innerIndex = 0;
  int widths = 1;

  int c=getchar();
  if(c==EOF)
  {
    *numElements=0;
    return lines;
  }

  addString(&lines, outerIndex, widths);
  while(c!=EOF)
  {
    checkWidths(&lines, outerIndex, innerIndex, &widths);
    lines[outerIndex][innerIndex]=c;
    innerIndex++;
    if(c==' ')
    {
      lines[outerIndex][innerIndex]='\0';
      innerIndex=0;
      outerIndex++;
      addString(&lines,outerIndex,widths);
    }
    c=getchar();
  }
  if(innerIndex!=0)
  {
    checkWidths(&lines, outerIndex, innerIndex, &widths);
    lines[outerIndex][innerIndex]=' ';
    lines[outerIndex][innerIndex+1]='\0';
    outerIndex++;
  }
  *numElements=(size_t)(outerIndex);
  return lines;
}

main()
{
  size_t num=0;
  char** lines = parseInput(&num);
  qsort(lines, num, sizeof(char*), frobcmphelp);
  int i;
  for(i=0;i<num;i++)
  {
    int j=0;
    while(lines[i][j]!=' ')
    {
      putchar(lines[i][j]);
      j++;
    }
    putchar(lines[i][j]);
    free(lines[i]);
  }
  free(lines);
  exit(0);
}
