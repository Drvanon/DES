APP_NAME = bin/des
CC=gcc
SOURCES = example.c des.c system/system_velocity.c assemblage.c
LDFLAGS = -Icomponents -Isystems

default:
	$(CC) $(SOURCES) $(LDFLAGS) -g -o $(APP_NAME)

win32:
	$(CC) $(SOURCES) $(LDFLAGS) -g -o $(APP_NAME).exe

sanitize:
	$(CC) $(SOURCES) $(LDFLAGS) -g -o $(APP_NAME) -fsanitize=memory
