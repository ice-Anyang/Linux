#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>

int main()
{
  while(1)
  {
    sleep(1);
    printf("你好，晚安！\n");
  }
  return 0;
}
