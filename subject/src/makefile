.PHONY:order_system 
order_system:order_system.cpp 
	g++ -shared -fPIC -std=c++11 $^ -o $@ -L/usr/lib64/mysql -lmysqlclient -ljsoncpp -lpthread
