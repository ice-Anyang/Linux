#include"order_system.hpp"
#include"httplib.h"

Ice::DishTable* dish_table;
Ice::OrderTable* order_table;
void InsertDish(const httplib::Request& req,httplib::Response& rsp){
  // req.body 中保存正文
  Json::Value dish_val;

  Json::Reader reader;
  //reader.parse(json数据串，解析后的Json::val对象) 
  bool ret = reader.parse(req.body,dish_val);
  if(ret == false){
    std::cout<<"Insertdish parse json data failed!\n";
    rsp.status = 400;
  }
  dish_table->Insert(dish_val);// 将菜品信息上传到dish_table中;
  rsp.status = 200;
  return ;
}

void DeleteDish(const httplib::Request& req,httplib::Response& rsp){
  // path="/dish/100"; /dish/(\d+)
  // 匹配一个/dish/数字 这个格式字符串
  int dish_id = std::stoi(req.matches[1]);// matches 中保存所有匹配的指定字符串
  bool ret = dish_table->Delete(dish_id);
  if(ret == false){
    rsp.status = 500;
    return ;
  }

  rsp.status = 200;
  return;
}
void UpdateDish(const httplib::Request& req,httplib::Response &rsp){
  //新的菜品信息保存在请求的正文中
  Json::Value dish_val;
  Json::Reader reader;

  bool ret = reader.parse(req.body,dish_val);
  if(ret == false){
    rsp.status = 400;
    return;
  }
  //更新数据库信息qian，应该对信息进行服务器端检验
  ret = dish_table->Update(dish_val);
  if(ret == false){
    rsp.status = 500;
    return ;
  }
  rsp.status = 200;
  return;
}

void SelectOneDish(const httplib::Request& req,httplib::Response& rsp)
{
  int dish_id = std::stoi(req.matches[1]);
  // 从数据库中获取指定id的彩屏信息
  Json::Value dish_val;
  bool ret = dish_table->SelectOne(dish_id,&dish_val);
  if(ret == false)
  {
    rsp.status = 500;
    return;
  }
  // 获取成功，将dish_val 数据对象，序列化为json数据串。
  Json::FastWriter writer;
  std::string body = writer.write(dish_val);

  rsp.set_content(body.c_str(),body.size(),"application/json");
  rsp.status = 200;
  return;
}

void SelectAllDish(const httplib::Request& req, httplib::Response& rsp){
  Json::Value dishes_val;
  bool ret = dish_table->SelectAll(&dishes_val);
  if(ret == false){
    rsp.status = 500;
    return;
  }
  std::string body;
  Json::FastWriter writer;
  body = writer.write(dishes_val);

  rsp.set_content(body.c_str(),body.size(),"application/json");
  rsp.status = 200;
  return;
}
void InsertOrder(const httplib::Request& req,httplib::Response& rsp){
  Json::Value order_val;
  Json::Reader reader;
  bool ret = reader.parse(req.body,order_val);
  if(ret  == false){
    std::cout<<"InsertOrder parse json failed!!\n";
    rsp.status = 400;
    return;
  }
  ret = order_table->Insert(order_val);
  if(ret == false){
    std::cout<<"InsertOrder insert data to db failed!!\n";
    rsp.status = 500;
    return;
  }
  rsp.status = 200;
  return;
}
void DeleteOrder(const httplib::Request& req,httplib::Response& rsp){
  int order_id = std::stoi(req.matches[1]);
  bool ret = order_table->Delete(order_id);
  if(ret == false){
    std::cout<<"DeleteOrder delect order failed!\n";
    rsp.status = 500;
    return;
  }
  rsp.status = 200;
  return;
}
void UpdateOrder(const httplib::Request& req,httplib::Response& rsp){
  Json::Value order_val;
  Json::Reader reader;

  bool ret = reader.parse(req.body,order_val);
  if(ret == false){
    std::cout<<"UpdateOrder parse json failed!!\n";
    rsp.status = 400;
    return;
  }
  ret = order_table->Update(order_val);
  if(ret == false){
    std::cout <<"UpdateOrder update data to db failed\n";
    rsp.status = 400;
    return;
  }
  rsp.status = 200;
  return;
}

void SelectOneOrder(const httplib::Request& req,httplib::Response& rsp){
  int order_id = std::stoi(req.matches[1]);
  Json::Value order_val;
  bool ret = order_table->SelectOne(order_id,&order_val);
  if(ret == false){
    std::cout << "SelectOneOrder get order info from db failed!\n";
    return ;
  }
  std::string body;
  Json::FastWriter writer;
  body = writer.write(order_val);
  rsp.set_content(body.c_str(),body.size(),"application/json");
  rsp.status = 200;
  return ;
} 

void SelectAllOrder(const httplib::Request& req,httplib::Response& rsp){
  Json::Value order_val;
  bool ret = order_table->SelectAll(&order_val);
  if(ret == false){
    std::cout<<"SelectAllOrder get order iinfo from db failed!\n";
    return;
  }

  std::string body;
  Json::FastWriter writer;
  body = writer.write(order_val);
  rsp.set_content(body.c_str(),body.size(),"application/json");
  rsp.status = 200;
  return;
}


int main()
{
  MYSQL*mysql = Ice::MysqlInit();
  dish_table = new Ice::DishTable(mysql);
  order_table = new Ice::OrderTable(mysql);

  httplib::Server server;
  //插入菜品
  server.Post("/dish",InsertDish);
  //删除菜品
  server.Delete(R"(/dish/(\d+))",DeleteDish);
  //更新菜品
  server.Put("/dish",UpdateDish);
  //查询单个
  server.Get(R"(/dish/(\d+))",SelectOneDish);
  //查询全部
  server.Get("/dish",SelectAllDish);
  server.Post("/order",InsertOrder);
  server.Delete(R"(/order/(\d+))",DeleteOrder);
  server.Put("/order",UpdateOrder);
  server.Get(R"(/order/(\d+))",SelectOneOrder);
  server.Get("/order",SelectAllOrder);
  server.listen("0.0.0.0",9000);

  Ice::MysqlDestory(mysql);
  return 0;
}
