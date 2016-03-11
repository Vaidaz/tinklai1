build:
	gcc client.c command_select.c information_get.c -o client
run:
	./client
clean:
	rm client