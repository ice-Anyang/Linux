// 使用封装的TcpSocket类实例化对象tcp客户端程序

#include<iostream>
#include<stdlib.h>
#include<signal.h>
#include"tcpsocket.hpp"

void sigcb(int signo){
  printf("连接已经断开，继续发送数据触发异常SIGPIPE信号\n");
}
int main(int argc,char*argv[])
{
  if (argc != 3) {
    printf("em:./tcp_cli 172.26.32.242 9000--服务绑定的地址\n");
    return -1;

  }
  signal(SIGPIPE, sigcb);
  std::string ip = argv[1];
  uint16_t port = atoi(argv[2]);
  TcpSocket cli_sock;
  // 创建套接字
  CHECK_RET(cli_sock.Socket());
  // 绑定地址信息（不推荐）
  //向服务端发起请求
  CHECK_RET(cli_sock.Connect(ip,port));
  //循环收发数据
  while(1){
    printf("client say: %s");
    fflush(stdout);
    std::string buf;
    std::cin>>buf;
    //因为客户端不存在多种套接字的文件，因此一旦当前套接字出错直接退出就行
    //进程退出就会释放资源，关闭套接字
    
    cli_sock.Send(buf);

    buf.clear();
    cli_sock.Recv(&buf);
    printf("server say:%s\n",buf.c_str());
  }
  cli_sock.Close();
  return 0;
}
