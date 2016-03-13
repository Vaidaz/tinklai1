build:
	gcc client.c command_select.c information_get.c connector.c ../common/hash.c -o client
run:
	valgrind --tool=memcheck ./client
clean:
	rm client