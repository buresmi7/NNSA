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
	double pozice_x;
	double pozice_y;
	int polomer;
	int sirka;
	int vyska;
	bool pocitej_kolize;
	sf::Sprite sprite;	
	sf::RenderWindow *app;//odkaz na okno do ktereho ma vykreslovat
	Space *space;
	int pocitadlo;
	Objekt* vlastnik;
public:	
	Objekt(sf::RenderWindow *App, Space *s, int pozice_x = 0, int pozice_y = 0, Objekt *vlastnik = NULL);
	Objekt(const Objekt &o);
	void nastavPocitaniKolizi(){pocitej_kolize = true;};
	bool pocitejKolize(){return pocitej_kolize;};
	void nastavPozici(int x, int y);
	void Draw();
	sf::RenderWindow * getApp();
	sf::Sprite getSprite();
	double getPoziceX();
	double getPoziceY();
	int getPolomer(){return polomer;};
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
	void posunX(double x);
	void posunY(double y);
	void otoc();
	void vystrel();
	virtual char getName() = 0;
	void nastavVlastnika(Objekt *v){vlastnik = v;};
	Objekt* getVlastnik(){return vlastnik;};
};

class Lod : public Objekt{
	static sf::Image Image;
public:
	Lod(sf::RenderWindow *App = NULL, Space *s = NULL, int pozice_x = 0, int pozice_y = 0, Objekt *vlastnik = NULL);	
	char getName(){return 'n';};
	static bool Init(const std::string& ImageFile){return Image.LoadFromFile(ImageFile);};
};

class Skudce : public Objekt{
	static sf::Image Image;
public:
	Skudce(sf::RenderWindow *App = NULL, Space *s = NULL, int pozice_x = 0, int pozice_y = 0, Objekt *vlastnik = NULL);
	char getName(){return 'n';};
	static bool Init(const std::string& ImageFile){return Image.LoadFromFile(ImageFile);};
};

class Strela : public Objekt{
	static sf::Image Image;
public:
	Strela(sf::RenderWindow *App = NULL, Space *s = NULL, int pozice_x = 0, int pozice_y = 0, Objekt *vlastnik = NULL);
	char getName(){return 's';};
	static bool Init(const std::string& ImageFile){return Image.LoadFromFile(ImageFile);};
};
#endif

