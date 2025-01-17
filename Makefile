PROGRAM=minestorm.exe

CFLAGS=-O0 -g -Wall -Wextra -Werror=implicit-function-declaration
CPPFLAGS=-Iinclude -MMD

LDLIBS=include/toolbox.a -Llibs/x86_64-linux-gnu -lglfw3 -lcanvas -lstdc++ -ldl -lm -lpthread

OBJS=app/main.o app/app.o app/draw.o app/physics.o app/menu.o app/game.o app/enemy.o app/player.o app/collisions.o app/bullet.o app/animation.o app/audio.o
DEPS=$(OBJS:.o=.d)

.PHONY: all clean

all: $(PROGRAM)

-include $(DEPS)

%.o: %.c
	gcc -c $(CFLAGS) $(CPPFLAGS) $< -o $@

$(PROGRAM): $(OBJS)
	gcc $(CFLAGS) $(CPPFLAGS) $^ $(LDLIBS) -o $@

clean:
	rm -f $(OBJS) $(DEPS) config.bin minestorm