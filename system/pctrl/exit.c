#include<stdio.h>
#include<unistd.h>
#include<sys/wait.h>
#include<stdlib.h>
int main()
{
  printf("晚上天气好热、、、\n");
  sleep(2);
  _exit(1);


}
