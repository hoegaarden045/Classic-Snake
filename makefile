build: clean
	gcc -o snake.exe main.c -lpthread -lncurses
run: 
	./snake.exe 
clean:
	@rm -vf ./snake.exe
