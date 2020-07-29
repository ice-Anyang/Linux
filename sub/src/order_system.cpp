#include"order_system.hpp"
int main()
{
  MYSQL *mysql = MysqlInit();
  if(mysql == NULL){
    return -1;
  }
  DishTable dish_tb(mysql);
  OrderTable order_tb(mysql);
  Json::Value dish;
  dish["name"] = "牛肉泡馍";
  dish["price"]=3600;
  dish_tb.Insert(dish);

  MysqlDestory(mysql);
  return 0;
}
