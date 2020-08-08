/*
 * 演示json的基本使用，序列化与反序列化
 *
 */

#include<cstdio>
#include<iostream>
#include<string>
#include<jsoncpp/json/json.h>

int main()
{

  std::string name = "小明";
  std::string sex = "男";
  int age = 26;

  Json::Value value; //value 数据串
  value["姓名"] = name;
  value["性别"] = sex;
  value["年龄"] = age;
  value["成绩"].append(88);
  value["成绩"].append(89);
  value["成绩"].append(90);

  Json::StyledWriter writer;// json 序列化
  std::string json_str = writer.write(value);
  printf("json_str:[%s]\n",json_str.c_str());
  Json::FastWriter fwriter; // json 序列化对象
  json_str = fwriter.write(value);
  printf("json_str:[%s]\n",json_str.c_str());
  
  Json::Value val2;
  Json::Reader reader;//json 反序列化
  // 将数据串进行解析得到的数据放到json::val2 中
  reader.parse(json_str,val2);

  std::cout<<"name:"<<val2["姓名"]<<std::endl;
  std::cout<<"sex:"<<val2["性别"]<<std::endl;
  std::cout<<"age:"<<val2["年龄"]<<std::endl;
  std::cout<<"score:"<<val2["成绩"]<<std::endl;
  for(auto it : val2["成绩"]){
    std::cout<<"ch score :"<<it<<std::endl;
  }


  return 0;
}
