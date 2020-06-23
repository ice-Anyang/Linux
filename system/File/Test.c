#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main()
{
  FILE* fd = fopen("myfile","w");
  if(!fd)
  {
    printf("fopen error!\n");
  }

  const char* msg = "hello world\n";

  int count = 6;
  while(count--)
  {
    fwrite(msg,strlen(msg),1,fd);
  }

  fclose(fd);

  return 0;
}
