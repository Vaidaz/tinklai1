build:
	# gcc client.c command_select.c information_get.c connector.c ../common/hash.c -o client
	gcc client.c connector.c ../common/word.c -o client
t:
	valgrind --tool=memcheck ./client
run:
	./client
clean:
	rm client