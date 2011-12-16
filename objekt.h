#ifndef OBJEKT
#define OBJEKT

#include <iostream>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
//#include "space.h"

class Space;

class Objekt{
protected:
	int pozice_x;
	int pozice_y;
	int sirka;
	int vyska;
	sf::Sprite sprite;
	sf::Image Image;
	sf::RenderWindow *app;//odkaz na okno do ktereho ma vykreslovat
	Space *space;
	int pocitadlo;
	Objekt* vlastnik;
public:
	Objekt(sf::RenderWindow *App, Space *s, int pozice_x = 0, int pozice_y = 0, Objekt *vlastnik = NULL);
	Objekt(const Objekt &o);
	void nastavPozici(int x, int y);
	void Draw();
	sf::RenderWindow * getApp();
	sf::Sprite getSprite();
	int getPoziceX();
	int getPoziceY();
	void posunLeft();
	void posunRight();
	void posunUp();
	void posunDown();
	// dvojnasobna rychlost
	void posunLeftD();
	void posunRightD();
	void posunUpD();
	void posunDownD();

	void posun(int x, int y);
	void otoc();
	void vystrel();
	virtual char getName() = 0;
	void nastavVlastnika(Objekt *v){vlastnik = v;};
	Objekt* getVlastnik(){return vlastnik;};
};

class Lod : public Objekt{
public:
	Lod(sf::RenderWindow *App, Space *s, int pozice_x, int pozice_y, Objekt *vlastnik = NULL);	
	char getName(){return 'n';};
};

class Skudce : public Objekt{
public:
	Skudce(sf::RenderWindow *App, Space *s, int pozice_x, int pozice_y, Objekt *vlastnik = NULL);
	char getName(){return 'n';};
};

class Strela : public Objekt{
public:
	Strela(sf::RenderWindow *App, Space *s, int pozice_x, int pozice_y, Objekt *vlastnik);
	char getName(){return 's';};	
};
#endif

