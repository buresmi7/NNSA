#ifndef SPACE_H_
#define SPACE_H_

#include <iostream>
#include <typeinfo>
#include <algorithm>
#include <vector>
#include <set>
#include <iterator>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include "controller.h"

class Space{
	int vyska;
	int sirka;
	std::set<Controller*> pole;
	std::set<Controller*> nove; //nove objekty, priradi se do pole az po cyklu "kola" nebot jinak to habruje s iteratorem a zmenou pole uprostred cyklu
	int citac;

	int genrand(int min, int max){return min+(rand()%(++max-min));};
	bool CircleTest(Objekt *a, Objekt *b);
public:
	Space(int vyska = 400, int sirka = 600);
	~Space();
	void addController(Controller *c);
	void ProvedKolo();
	void VykresliVsechny();
	std::set<Controller*>* GetVsechnyObjekty();
	int getVyska(){return vyska;};
	int getSirka(){return sirka;};
};

#endif
