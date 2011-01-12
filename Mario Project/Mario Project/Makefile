SDL_CFLAGS := $(shell sdl-config --cflags)
SDL_LDFLAGS := $(shell sdl-config --libs)

INCLUDE = -I/librairies

tout : POKER

POKER : main.o message.o partie.o sprite.o joueur.o objet.o bouton.o carte.o texte.o keystate.o texture.o
	gcc -Wall -o POKER -lGL -lGLU  -lSDL -lSDLmain -lSDL_image $(INCLUDE) main.o message.o partie.o sprite.o joueur.o objet.o bouton.o carte.o texte.o keystate.o texture.o

main.o : main.c sprite.h message.h
	gcc -c main.c

message.o : message.c message.h partie.h
	gcc -c message.c message.h

partie.o : partie.c partie.h joueur.h texte.h
	gcc -c partie.c partie.h

sprite.o : sprite.c sprite.h joueur.h texte.h
	gcc -c -lGl -lGLU sprite.c sprite.h

joueur.o : joueur.c joueur.h carte.h objet.h
	gcc -c joueur.c joueur.h

objet.o : objet.c objet.h texture.h bouton.h
	gcc -c objet.c objet.h

bouton.o : bouton.c bouton.h keystate.h
	gcc -c bouton.c bouton.h

carte.o : carte.c carte.h texture.h
	gcc -c carte.c carte.h

texte.o : texte.c texte.h texture.h
	gcc -c texte.c texte.h

keystate.o : keystate.c keystate.h constantes.h
	gcc -c keystate.c keystate.h

texture.o : texture.c texture.h constantes.h
	gcc -c -lGl -lGLU  texture.c texture.h

clean : 
	rm *.o *~ *.gch
