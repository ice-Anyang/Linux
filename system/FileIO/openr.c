#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<fcntl.h>

int main()
{

  int fd = open("myfile", O_RDONLY);
  if(fd < 0){
    perror("open");
    return 1;

  }
  const char *msg = "hello bit!\n";
  char buf[1024];
  while(1){
    ssize_t s = read(fd, buf, strlen(msg));//类比write
    if(s > 0){
      printf("%s", buf);

    }else{
      break;

    }

  }
  close(fd);
  return 0;
}
