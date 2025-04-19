
all: 
	make compile
	make run

.PHONY: compile
compile: 
	gcc -o run_me dht11_ctrl.c -lpigpio -lrt -lpthread

.PHONY: run
run:
	sudo ./run_me
