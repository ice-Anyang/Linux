// 实现客户端，向外提供简单接口实现套接字编程
#include<iostream>
#include<cstdio>// stdio.h
#include<string>
#include<unistd.h>//close接口
#include<stdlib.h>//atoi接口
#include<netinet/in.h> // 地址结构定义
#include<arpa/inet.h> // 字节序转换接口
#include<sys/socket.h> // 套接字接口

class UdpSocket
{
  public:
    UdpSocket():_sockfd(-1){

    }
    //1.创建套接字
    bool Socket(){
      //socket(地址域，套接字类型，协议类型)
      //AF_INET--这是IPV4通信，并且提供的是数据报服务，使用udp协议。
      _sockfd = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
      if(_sockfd < 0){
        perror("socket error");
        return false;
      }
      return true;
    }
    // 2.为套接字绑定地址信息
    bool Bind(const std::string &ip,uint32_t port){
      //①定义ipv4地质结构
      struct sockaddr_in addr;
      addr.sin_family = AF_INET;
      addr.sin_port = htons(port);
      addr.sin_addr.s_addr = inet_addr(ip.c_str());
      //②绑定地址信息
      socklen_t len = sizeof(struct sockaddr_in);
      int ret = bind(_sockfd,(struct sockaddr*)&addr,len);
      if(ret < 0){
        perror("bind error");
        return false;
      }
      return true;
    }
    // 3.发送数据
    bool Send(const std::string &data,const std::string &ip,uint16_t port){
      //sendto(描述符，数据，长度，选项，对端地址，地址长度)
      //①定义虽短地址的ipv4地质结构
      struct sockaddr_in addr;
      addr.sin_family = AF_INET;
      addr.sin_port = htons(port);
      addr.sin_addr.s_addr = inet_addr(ip.c_str());
      //②像这个地址发送数据
      int ret;
      socklen_t len = sizeof(struct sockaddr_in);
      ret = sendto(_sockfd,data.c_str(),data.size(),0,(struct sockaddr*)&addr,len);
      if(ret < 0){
        perror("sendto error");
        return false;
      }
      return true;
    }
    // 4.接收数据
    bool Recv(std::string *buf,std::string *ip = NULL,uint16_t *port = NULL){
      //recvfrom(描述符，缓冲区，数据长度，选项，对端地址，地址长度)
      struct sockaddr_in addr;//用于获取发送端地址信息
      socklen_t len = sizeof(struct sockaddr_in);
      int ret;
      char tmp[4096]={0};
      ret = recvfrom(_sockfd,tmp,4096,0,(struct sockaddr*)&addr,&len);
      if(ret < 0){
        perror("recvfrom error");
        return -1;
      }
      buf->assign(tmp,ret);
      if(ip!= NULL){
        *ip = inet_ntoa(addr.sin_addr);
      }
      if(port != NULL){
        *port = ntohs(addr.sin_port);
      }
      return true;
    }
    //5.关闭套接字
    void Close()
    {
      close(_sockfd);
      _sockfd = -1;
      return;
    }

  private:
    int _sockfd;
};
#define CHECK_RET(q) if((q)==false){return -1;}
//客户端要给服务端发送数据，那麽久需要知道服务端的地址信息。
int main(int argc ,char *argv[])
{
  if(argc != 3){
    printf("em: ./udp_cli 172.26.32.242 1500\n");
    return -1;
  }
  // argv[0] = ./cli_udp
  // argv[1]=172.26.32.242
  // argv[2]=1500
  std::string ip_addr = argv[1];//服务端地址信息
  uint16_t port_addr = atoi(argv[2]);
  UdpSocket sock;
  CHECK_RET(sock.Socket());
  //CHECK_RET(sock.Bind());
  while(1){
    std:: cout<<"client say: ";
    fflush(stdout);
    std::string buf;
    std::cin>>buf;
    sock.Send(buf,ip_addr,port_addr);

    buf.clear();//清空buf缓冲区
    sock.Recv(&buf);
    std::cout<<"server say: "<<buf<<std::endl;
  }
  sock.Close();
  return 0;
}

