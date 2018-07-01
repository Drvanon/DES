APP_NAME = des
CC=clang
SOURCES = example.c des.c assemblage.c
LDFLAGS =

default:
	$(CC) $(SOURCES) -o $(APP_NAME) $(LDFLAGS)

debug:
	$(CC) $(SOURCES) -g -o $(APP_NAME) $(LDFLAGS)
