// socket编程udp客服端编写
//       1. 创建套接字
//       2. 为套接字绑定地址信息
//       3. 接收数据     
//       4. 发送数据
//       5. 关闭套接字

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>//sockaddr 结构体 / IPPRPTP_UDP
#include<netinet/in.h> //包含一些字节序转换接口
#include<sys/socket.h>// 套接字接口头文件

int main(int argc,char *argv[])
{
  //argc表示参数个数，通过argv像程序员传递端口信息
  if(argc != 3){
    printf("./srv_udp ip port  em: ./srv_udp 172.26.32.242  1500\n");
    return -1;
  }
  const char* ip_addr = argv[1];
  uint16_t port_addr = atoi (argv[2]);
  //socket(地址域，套接字类型，协议类型)
  int sockfd = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
  //bind(套接字描述符，地址结构，地址长度)
  //struck sockaddr_in ipv4地址结构
  // struct in_addr{uint32_t s_addr}
  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  // htons -- 将两个字节的主机字节序装换为网络字节序
  addr.sin_port = htons(port_addr);
  // inet_addr
  addr.sin_addr.s_addr = inet_addr(ip_addr);
  socklen_t len = sizeof(struct sockaddr_in); //获取地址信息长度
  int ret = bind(sockfd,(struct sockaddr*)&addr,len);
  if(ret<0){
    perror("bind error");
    close(sockfd);
    return -1;
  }
  while(1){
    char buf[1024]={0};
    struct sockaddr_in cliaddr;
    socklen_t len = sizeof(struct sockaddr_in);
    //recvfrom(描述符，缓冲区，长度，参数，客户端地址信息)
    int ret = recvfrom(sockfd,buf,1023,0,(struct sockaddr*)&cliaddr,&len);
    if(ret<0){
      perror("recvfrom error");
      close(sockfd);
      return -1;
    }
    printf("client say:%s\n",buf);

    memset(buf,0x00,1024);//清空buf
    printf("server say: ");
    fflush(stdout);
    scanf("%s",buf);
    // 通过sockfd将buf中的数据发送到cliaddr客户端
    ret = sendto(sockfd,buf,strlen(buf),0,(struct sockaddr*)&cliaddr,len);
    if(ret<0){
      perror("sendto error");
      close(sockfd);
      return -1;
    }
  }
  close(sockfd);
  return 0;
}
