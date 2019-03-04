CC = gcc
CCOPT = -Wall

SRC_SERVER = server.c
SRC_CLIENT = client.c
APP_SERVER = server
APP_CLIENT = client

.DEFAULT_GOAL := all

all: $(APP_SERVER) $(APP_CLIENT)

run_server: $(APP_SERVER)
	./$(APP_SERVER)

run_client: $(APP_CLIENT)
	./$(APP_CLIENT)


$(APP_SERVER): $(SRC_SERVER) Makefile
	$(CC) $(CCOPT) -o $(APP_SERVER) $(SRC_SERVER) -lpthread

$(APP_CLIENT): $(SRC_CLIENT) Makefile
	$(CC) $(CCOPT) -o $(APP_CLIENT) $(SRC_CLIENT)

.PHONY: clean
clean:
	rm -f $(APP_SERVER)
	rm -f $(APP_CLIENT)

