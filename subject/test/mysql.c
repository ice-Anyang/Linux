/*
 *通过mysql的从语言接口实现mysql数据库操作
 *
 */

#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<mysql/mysql.h>

int main()
{
  MYSQL* mysql = mysql_init(NULL);//定义并初始化数据库,饭后操作句柄;
  //my_real_connect(句柄，主机地址,用户名,密码,数据库名称,端口,套接字文件,标志)
  if(mysql_real_connect(mysql,"127.0.0.1","root","123","db_order_sys",0,NULL,0)==NULL){
    printf("mysql connect failed:%s\n",mysql_error(mysql));
    return -1;
  }
  int ret = mysql_set_character_set(mysql,"utf8");
  if(ret != 0){
    printf("set character to utf8 failed:%s\n",mysql_error(mysql));
    return -1;
  }

  char *sql_str = "select * from tb_dishes;";
  ret = mysql_query(mysql,sql_str);//执行语句
  if(ret != 0){
    printf("sql query failed:%s\n",mysql_error(mysql));
    return -1;
  }
  
  MYSQL_RES *res = mysql_store_result(mysql);//过去执行结果集
  if(res == NULL){
    printf("store result failed:%s\n",mysql_error(mysql));
    return -1;
  }
  
  int row_nums = mysql_num_rows(res);//获取结构条数
  int col_nums = mysql_num_fields(res);//获取结果列数
  int i = 0,j = 0;
  for(;i<row_nums;++i){
    MYSQL_ROW row = mysql_fetch_row(res);
    for(j = 0; j< col_nums; j++){
      printf("%s ",row[j]);//获取遍历每一条数据
    }
    printf("\n");
  }

  mysql_free_result(res);
  mysql_close(mysql);//关闭数据库释放资源
  return 0;
}
