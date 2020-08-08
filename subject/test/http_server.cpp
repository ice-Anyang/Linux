#include<iostream>
#include"httplib.h"
void HelloWorld(const httplib::Request& req,httplib::Response& rsp)
{
  // req--- 接收到并解析完毕的请求信息，更具这个信息完成业务处理
  //rsp --- 要求用户进行填充响应信息
  std::cout<<"method:"<<req.method<<std::endl;
  std::cout<<"path:"<<req.path<<std::endl;
  std::cout<<"body:"<<req.body<<std::endl;
  rsp.status = 200;
  rsp.body = "<html><body><h1>Hell0World</h1></body></html>";
  rsp.set_header("Content-Type","text/html");

  //std::string body ="<html><body><h1>Hell0World</h1></body></html>";
  // rsp.set_content(body,bode.size(),"text/html");
  return ;
}

int main()
{
  int a = 10;
  httplib::Server server;
  server.Get("/helloworld",HelloWorld);
  server.Get("/hi",[&](const httplib::Request& req,httplib::Response& rsp)
      {
        std::cout<<"a:"<<a<<std::endl;
        std::string body = "<html><body><h1>hi</h1></body></html>";
        rsp.set_content(body.c_str(),body.size(),"text/html");
      });
  server.listen("0.0.0.0",9000);
  return 0;
}
