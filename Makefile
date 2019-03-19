CC = gcc
CCOPT = -Wall -Wextra

SRC_SERVER = server.c
SRC_CLIENT = client.c
APP_SERVER = server
APP_CLIENT = client
SRCS = authentication.c utils.c
HEADERS = $(wildcard *.h)

.DEFAULT_GOAL := all

all: $(APP_SERVER) $(APP_CLIENT)

run_server: $(APP_SERVER)
	./$(APP_SERVER)

run_client: $(APP_CLIENT)
	./$(APP_CLIENT)

$(APP_SERVER): $(SRC_SERVER) $(SRCS) $(HEADERS) Makefile
	$(CC) $(CCOPT) -o $@ $(SRC_SERVER) $(SRCS) -lpthread

$(APP_CLIENT): $(SRC_CLIENT) $(SRCS) $(HEADERS) Makefile
	$(CC) $(CCOPT) -o $@ $(SRC_CLIENT) $(SRCS) -lpthread

.PHONY: clean
clean:
	rm -f $(APP_SERVER)
	rm -f $(APP_CLIENT)

