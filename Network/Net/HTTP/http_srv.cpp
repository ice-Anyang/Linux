#include<iostream>
#include<sstream>
#include<stdlib.h>
#include"tcpsocket.hpp"

int main(int argc,char* argv[])
{
  if(argc!=3){
    printf("em:./tcp_srv 172.26.32.242 9000");
    return -1;
  }
  std::string ip = argv[1];
  uint16_t port = atoi(argv[2]);

  TcpSocket lst_sock;
  CHECK_RET(lst_sock.Socket());//创建监听套接字
  CHECK_RET(lst_sock.Bind(ip,port));
  CHECK_RET(lst_sock.Listen());
  while(1){
    TcpSocket cli_sock;
    bool ret = lst_sock.Accept(&cli_sock);
    if(ret == false){
      continue;
    }

    std::string http_req;
    cli_sock.Recv(&http_req);
    printf("req:[%s]\n",http_req.c_str());

    //响应—首行(版本/状态码/描述)-头部(Content-Length)-空行-正文
    std::string body = "<html><body><h1>hellow word</h1></body></html>";
    std::string blank = "\r\n";
    std::stringstream header;
    header<<"Content-length:"<<body.size()<<"\r\n";
    header<<"Content-Type:text/html\r\n";
    header<<"Location:http://baidu.com/\r\n";
    std::string first_line = "HTTP/1.1 500 Inernal Server Error\r\n";
    cli_sock.Send(first_line);
    cli_sock.Send(header.str());
    cli_sock.Send(blank);
    cli_sock.Send(body);
    cli_sock.Close();
  }

  lst_sock.Close();

  return 0;
}
