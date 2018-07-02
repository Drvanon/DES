APP_NAME = des
CC=gcc
SOURCES = example.c des.c system/system_velocity.c # assemblage.c
LDFLAGS = -Icomponents -Isystems

default:
	$(CC) $(SOURCES) -o $(APP_NAME) $(LDFLAGS)

debug:
	$(CC) $(SOURCES) -g -o $(APP_NAME) $(LDFLAGS)
