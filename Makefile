PROGRAM=minestorm

CFLAGS=-O0 -g -Wall -Wextra -Wno-unused-parameter -Werror=implicit-function-declaration -Iinclude
CFLAGS+=-Wno-unused-variable -Wno-deprecated-declarations

LDLIBS=include/toolbox.a -Llibs/x86_64-linux-gnu -lglfw3 -lcanvas -lstdc++ -ldl -lm -lpthread

OBJS=app/main.o app/app.o app/player.o app/enemy.o app/physics.o
DEPS=$(OBJS:.o=.d)

.PHONY: all clean

all: $(PROGRAM)

-include $(DEPS)

%.o: %.c
	gcc -c $(CFLAGS) $< -o $@

$(PROGRAM): $(OBJS)
	gcc $(CFLAGS) $^ $(LDLIBS) -o $@

clean:
	rm -f $(OBJS) $(DEPS) config.bin
