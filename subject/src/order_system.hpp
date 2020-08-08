/*
 *基于mysql封装一个数据对象操作类
 *菜品信息表操作类
 *订单信息表操作类
 *通过josin通信格式完成菜品信息/订单信息
 */

#include<cstdio>
#include<iostream>
#include<string>
#include<vector>
#include<mysql/mysql.h>
#include<jsoncpp/json/json.h>

namespace Ice 
{

#define MYSQL_SERVER "127.0.0.1"
#define MYSQL_USER "root"
#define MYSQL_PASSWD "123"
#define MYSQL_DB "db_order_sys"

  void MysqlDestory(MYSQL* mysql);
  MYSQL* MysqlInit();
  bool MysqlQuery(MYSQL* mysql,const std::string &sql);
  //封装mysql数据库的基本操作，作为静态函数
  //1.mysql初始
  MYSQL* MysqlInit(){
    MYSQL*mysql = NULL;
    // 初始化数据库操作句柄
    mysql = mysql_init(NULL);
    if(mysql == NULL){
      printf("mysql init error\n");
      return NULL;
    }
    // 连接服务器
    if(mysql_real_connect(mysql, MYSQL_SERVER, MYSQL_USER, MYSQL_PASSWD, MYSQL_DB, 0, NULL, 0)==NULL){
      printf("connect mysql server failed:%s\n",mysql_error(mysql));
      MysqlDestory(mysql);
      return NULL;
    }
    //设置字符编码集
    if(mysql_set_character_set(mysql,"utf8")!=0){
      printf("set mysql client character failed:%s\n",mysql_error(mysql));
      MysqlDestory(mysql);
      return NULL;
    }
    return mysql;
  }
  //2.mysql销毁
  void MysqlDestroy(MYSQL* mysql){
    if(mysql!=NULL){
      mysql_close(mysql);
    }
    return;
  }
  //3.mysql执行语句

  bool MysqlQuery(MYSQL* mysql,const std::string &sql){
    int ret = mysql_query(mysql,sql.c_str()); 
    if(ret!= 0){
      printf("sql:[%s] query failed:%s\n",sql.c_str(),mysql_error(mysql));
      return false;
    }
    return true;
  }

  class DishTable //数据库中的菜品信息
  {
    public:
      DishTable(MYSQL* mysql):_mysql(mysql){
      }
      //上传菜品
      bool Insert(const Json::Value &dish){
#define DISH_INSERT "insert into tb_dishes(id,name,price,uptime)value(null,'%s',%d,now());"
        char buf[4096]={0};
        sprintf(buf,DISH_INSERT,dish["name"].asCString(),dish["price"].asInt());
        if(MysqlQuery(_mysql,buf)==false){
          return false;
        }
        return true;
      }
      // 获得一个彩品信息
      bool SelectOne(const int dish_id,Json::Value* dish){
#define DISH_SELECTONE "select name,price,uptime from tb_dishes where id = %d;"
        char buf[4096]={0};
        sprintf(buf,DISH_SELECTONE,dish_id);
        if(MysqlQuery(_mysql,buf)==false){
          return false;
        }
        MYSQL_RES* res = mysql_store_result(_mysql);
        if(res == NULL){
          printf("store one dish result failed:%s",mysql_error(_mysql));
          return false;
        }
        int num_row =mysql_num_rows(res);//获取结果条数；
        if(num_row != 1){
          printf("one dish result error\n");
          return false;
        }
        for(int i = 0;i < num_row;i++){
          MYSQL_ROW row = mysql_fetch_row(res);
          (*dish)["id"] = dish_id;
          (*dish)["name"] = row[0];
          (*dish)["uptime"] = row[2];
        }
        mysql_free_result(res);// 销毁结果集
        return true;
      }
      bool SelectAll(Json::Value* dishes){//返回所有菜品信息
#define DISH_SELECTONEALL "select id,name,price,uptime from tb_dishes;"
        if(MysqlQuery(_mysql,DISH_SELECTONEALL)==false){
          return false;
        }
        MYSQL_RES* res = mysql_store_result(_mysql);
        if(res == NULL){
          printf("store all dish result failed:%s\n",mysql_error(_mysql));
          return false;
        }
        int num_row = mysql_num_rows(res);
        for(int i = 0;i < num_row ;++i){
          MYSQL_ROW row = mysql_fetch_row(res);
          Json::Value dish_val;
          dish_val["id"] = std::stoi(row[0]);
          dish_val["name"]=row[1];
          dish_val["price"]=std::stoi(row[2]);
          dish_val["uptime"]=row[3];
          dishes->append(dish_val);
        }
        mysql_free_result(res);
        return true;
      }
      bool Update(const Json::Value &dish){
#define DISH_UPDATE "update tb_dishes set name ='%s',price = %d,uptime = now() where id = %d;"
        char buf[4096]={0};
        sprintf(buf,DISH_UPDATE,dish["name"].asCString(),dish["price"].asInt(),dish["id"].asInt());
        if(MysqlQuery(_mysql,buf)==false){
          return false;
        }
        return true;
      }
      bool Delete(const int dish_id){
#define DISH_DELETE "delete from tb_dishes where id = %d;"
        char buf[4096]={0};
        sprintf(buf,DISH_DELETE,dish_id);
        if(MysqlQuery(_mysql,buf)==false){
          return false;
        }
        return true;
      }
      ~DishTable(){
      }
    private:
      MYSQL* _mysql;
  };

  class OrderTable // 数据库中订单信息
  {
    public:
      OrderTable(MYSQL* mysql):_mysql(mysql){
      }
      bool Insert(const Json::Value &order){
#define ORDER_INSERT "insert into tb_orders(id,table_id,dishes,status,uptime)value(null,%d,'%s',%d,now());"
        char buf[4096]={0};
        Json::FastWriter writer;
        std::string dishes = writer.write(order["dishes"]);
        sprintf(buf,ORDER_INSERT,order["table_id"].asInt(),dishes.c_str(),order["status"].asInt());
        if(MysqlQuery(_mysql,buf)==false){
          return false;
        }
        return true;
      }
      bool SelectOne(const int order_id,Json::Value* order){
        //获得一个菜品信息、
#define ORDER_SELECTONE "select table_id,dishes,status,uptime from tb_orders where id=%d;"
        char buf[4096]={0};
        sprintf(buf,ORDER_SELECTONE,order_id);
        if(MysqlQuery(_mysql,buf)==false){
          return false;
        }
        MYSQL_RES* res = mysql_store_result(_mysql);
        if(res == NULL){
          printf("store one order result failed:%s\n",mysql_error(_mysql));
          return false;
        }
        int num_rows = mysql_num_rows(res);//获取结果条数
        if(num_rows != 1){
          printf("one order result error\n");
          return false;
        }
        for(int i = 0;i < num_rows;i++ )
        {
          MYSQL_ROW row = mysql_fetch_row(res);// 遍历获取每一条数据
          (*order)["id"] = order_id;
          (*order)["table_id"] =std::stoi(row[0]);
          Json::Value dishes;
          Json::Reader reader;
          reader.parse(row[1],dishes);
          (*order)["dishes"] = dishes;//订单彩屏信息
          (*order)["status"] = std::stoi(row[2]);//订单状态
          (*order)["uptime"] = row[3];//订单更新时间
        }
        mysql_free_result(res);
        return true;
      }
      bool SelectAll(Json::Value* orders){
        //返回所有菜品信息
#define ORDER_SELECTALL "select id,table_id,dishes,status,uptime from tb_orders;"
        if(MysqlQuery(_mysql,ORDER_SELECTALL)==false){
          return false;
        }
        MYSQL_RES* res=mysql_store_result(_mysql);//获取结果集
        if(res==NULL){
          printf("store one order result failed:%s\n",mysql_error(_mysql));
          return false;
        }
        int num_rows =mysql_num_rows(res);
        for(int i=0;i < num_rows;i++){
          MYSQL_ROW row = mysql_fetch_row(res);
          Json::Value order_val;
          order_val["id"]=std::stoi(row[0]);
          order_val["table_id"]=std::stoi(row[1]);
          Json::Value dishes;
          Json::Reader reader;

          reader.parse(row[2],dishes);
          order_val["dishes"]=dishes;
          order_val["status"]=std::stoi(row[3]);
          order_val["uptime"]=row[4];
          orders->append(order_val);
        }
        mysql_free_result(res);
        return true;
      }
      bool Update(const Json::Value &order){
#define ORDER_UPDATE "update tb_orders set table_id=%d,dishes='%s',status=%d,uptime=now(),where id=%d;"
        Json::Value dishes = order["dishes"];// 获取本桌菜品信息的json数据
        Json::FastWriter writer;
        std::string str_dishes = writer.write(dishes);//获取菜品信息字符串
        char buf[4096]={0};
        sprintf(buf,ORDER_UPDATE,order["table_id"].asInt(),str_dishes.c_str(),order["status"].asInt(),order["id"].asInt());
        if(MysqlQuery(_mysql,buf)==false){
          return false;
        }
        return true;
      }
      bool Delete(const int order_id){
#define ORDER_DELETE "delete from tb_orders where id = %d;"
        char buf[4096]={0};
        sprintf(buf,ORDER_DELETE,order_id);
        if(MysqlQuery(_mysql,buf)==false){
          return false;
        }
        return true;
      }
    private:
      MYSQL* _mysql;
  };
}
