LIBS = -lSDL2 -lSDL2_image -lm
build:
	gcc ./src/main.c $(LIBS) -o breakout
