CC = gcc
RM = rm

SERVER_OBJECTS = server.c
CLIENT_OBJECTS = client.c

CFLAGS += -D HAVE_LIBAIO -g

SER_TARGETS := server
CLI_TARGETS := client


all:$(SER_TARGETS) $(CLI_TARGETS)

$(SER_TARGETS) : $(SERVER_OBJECTS)
	$(CC) $(CFLAGS) $^ -o $@

$(CLI_TARGETS) : $(CLIENT_OBJECTS)
	$(CC) $(CFLAGS) $^ -o $@

clean:
	$(RM) -rf $(TARGETS)
