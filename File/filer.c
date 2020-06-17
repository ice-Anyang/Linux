#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main()
{
  FILE* fd = fopen("myfile","r");
  if(!fd)
  {
    printf("fopen error!\n");
  }
  
  char buf[1024];
  const char* msg = "hello world\n";
  while(1)
  {
    ssize_t s = fwrite(buf,1,strlen(msg),fd);
    if(s>0)
    {
      buf[s]=0;
      printf("%s",buf);
    }
    if(feof(fd))//判断文件是否为空
      break;

  }
  fclose(fd);
  return 0;
}
