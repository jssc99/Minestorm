PROGRAM=minestorm

CFLAGS=-O0 -g -Wall -Wextra -Wno-unused-parameter -Werror=implicit-function-declaration
CFLAGS+=-Wno-unused-variable -Wno-deprecated-declarations
CPPFLAGS=-Iinclude -MMD

LDLIBS=include/toolbox.a -Llibs/x86_64-linux-gnu -lglfw3 -lcanvas -lstdc++ -ldl -lm -lpthread

OBJS=app/main.o app/app.o app/player.o app/enemy.o app/draw.o app/move_enemy.o app/physics.o app/menu.o
DEPS=$(OBJS:.o=.d)

.PHONY: all clean

all: $(PROGRAM)

-include $(DEPS)

%.o: %.c
	gcc -c $(CFLAGS) $(CPPFLAGS) $< -o $@

$(PROGRAM): $(OBJS)
	gcc $(CFLAGS) $(CPPFLAGS) $^ $(LDLIBS) -o $@

clean:
	rm -f $(OBJS) $(DEPS) config.bin
