
CC=gcc
CFLAGS=-Wall -I/usr/X11R6/include -L/usr/X11R6/lib\
	-lXaw -lXmu -lXt -lSM -lICE -lXext -lX11
SRCS=TGUI.cc main.cc graphics.cc event_handlers.cc tgtimer.cc\
	map.cc tank_game.cc bot.cc
NAME=tank

all:
	$(CC) $(CFLAGS) $(SRCS) -o $(NAME)
