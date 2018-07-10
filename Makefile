APP_NAME = bin/des
CC=gcc
SOURCES = example.c des.c system/system_velocity.c assemblage.c
LDFLAGS = -Icomponents -Isystems

default:
	$(CC) $(SOURCES) $(LDFLAGS) -o $(APP_NAME)

# Aopend .exe for clang
win32:
	$(CC) $(SOURCES) $(LDFLAGS) -g -o $(APP_NAME).exe

debug:
	$(CC) $(SOURCES) $(LDFLAGS) -g -o $(APP_NAME)

sanitize:
	$(CC) $(SOURCES) -g -o $(APP_NAME) $(LDFLAGS) -fsanitize=memory

test:
	$(CC) $(SOURCES) test.c -g -o $(APP_NAME) $(LDFLAGS)
