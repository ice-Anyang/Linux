// 利用TcpSocket类编写tcp服务器

#include<iostream>
#include<stdlib.h>
#include"tcpsocket.hpp"

int main(int argc ,char* argv[])
{
  if (argc != 3) {
    printf("em:./tcp_srv 172.26.32.242 9000\n");
    return -1;

  }
  std::string ip = argv[1];
  uint16_t port = atoi(argv[2]);

  TcpSocket lst_sock;
  CHECK_RET(lst_sock.Socket());//创建套接字

  CHECK_RET(lst_sock.Bind(ip,port));//绑定地址信息
  CHECK_RET(lst_sock.Listen()); //开始监听
  while(1){
    TcpSocket cli_sock;
    std::string cli_ip;
    uint16_t cli_port;
    // Accept类成员函数，使用的私有成员就是lst_sock 的私有成员 
    bool ret = lst_sock.Accept(&cli_sock,&cli_ip,&cli_port);
    if(ret==false){
      //获取新连接失败
      continue;
    }
    printf("new connect:[%s:%d]\n", cli_ip.c_str(), cli_port);
     std::string buf;
    //通过新获取的套接字与客户端进行通信
    if(cli_sock.Recv(&buf)==false){
      cli_sock.Close();
      continue;
    }
    printf("client:[%s:%d] say:%s\n", &cli_ip[0], cli_port, &buf[0]);

    std::cout << "server say:";
    fflush(stdout);
    buf.clear();
    std::cin >> buf;

    if (cli_sock.Send(buf) == false) {
      cli_sock.Close();
      continue;
    }
  }
  lst_sock.Close();
  return 0;
}
