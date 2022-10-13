NAME = aircraftsystem
TARGET = ../bin/$(NAME)

LIBS = -L../../libgpio/bin -lgpio
LIBS += -L../../libmlauncher/bin -lmlauncher
LIBS += -L../../libcmqtt/bin -lcmqtt
LIBS += -L/usr/lib -lusb

INC = -I../include
INC += -I../../libgpio/include/
INC += -I../../libmlauncher/include/
INC += -I../../libcmqtt/include
INC += -I/usr/lib

SRC = main.c

$(TARGET): $(SRC)
	$(CC) -o $(TARGET) $(SRC) $(INC) $(LIBS) -lpthread
clean:
	rm -f $(OBJS) $(TARGET)

purge: clean $(TARGET)
