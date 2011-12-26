#ifndef SPACE_H_
#define SPACE_H_

#include <iostream>
#include <typeinfo>
#include <algorithm>
#include <SFML/Window.hpp>
#include <vector>
#include <set>
#include <iterator>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include "controller.h"

class Space{
	int vyska;
	int sirka;
	std::set<Controller*> pole;
	std::set<Controller*> nove; //nove objekty, priradi se do pole az po cyklu "kola" nebot jinak to habruje s iteratorem a zmenou pole uprostred cyklu
	sf::RenderWindow *App;
	sf::Sprite pozadi;
	sf::Image Image;
	sf::Font MyFont;
	int citac;
	int citac2;
	int genrand(int min, int max){return min+(rand()%(++max-min));};
	bool CircleTest(Objekt *a, Objekt *b);
public:
	Space(sf::RenderWindow *App);
	sf::RenderWindow *getApp();
	void addController(Controller *c);
	void ProvedKolo();
	void VykresliVsechny();
	int getVyska(){return vyska;};
	int getSirka(){return sirka;};
};

#endif
