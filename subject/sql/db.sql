CREATE DATABASE IF NOT EXISTS db_order_sys;

USE db_order_sys;

CREATE TABLE IF NOT EXISTS tb_dishes(\

id int primary key auto_increment comment "菜品ID",\
name varchar(32) comment "菜品名称" ,

price int comment "菜品单价，单位:分",

uptime datetime comment "菜品的修改时间"
);

insert into tb_dishes values(NULL,"红烧肉",2700,now());
insert into tb_dishes values(NULL,"豆角茄子",1900,now());
insert into tb_dishes values(NULL,"宫保鸡丁",3200,now());

CREATE TABLE IF NOT EXISTS tb_orders(\

id int primary key auto_increment comment "订单ID",table_id int comment "订单桌号",

dishes varchar(32) comment "保存当前订单的菜品ID",status tinyint comment "订单状态0-已完结/1-未完结",
uptime datetime
);
