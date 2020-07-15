all:
	cli_udp srv_udp 

cli_udp:cli_udp.cpp
	g++ $^ -o $@
srv_udp:srv_udp.c
	gcc $^ -o $@
