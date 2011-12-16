#ifndef CONTROLLER
#define CONTROLLER

#include <iostream>
#include "objekt.h"
#include "frnn.h"

class Controller{
protected:
	Objekt *l;
	int skore;
	int vektor_y;
	std::set<Controller*> *pole;

	static bool razeni(std::pair<int , int> i,std::pair<int, int> j);
	std::vector<double> nejblizsiObjekty(int pocetObjektu);	
public:
	Controller(Objekt *l, std::set<Controller*> *objekty);
	~Controller(){delete l;};
	virtual void provedAkci() = 0;
	Objekt* getObjekt();
	int getSkore();
	void prictiSkore(int hodnota);
	void addPoleObjektu(std::set<Controller*> *o){pole = o;};
};

class ControllerClovek : public Controller{	
public:
	ControllerClovek(Objekt *l, std::set<Controller*> *o = NULL) : Controller(l, o){};
	void provedAkci();
};

class ControllerFRNN : public Controller{	
	FRNeuralNetwork *inteligence;
public:
	ControllerFRNN(Objekt *l, FRNeuralNetwork *i, std::set<Controller*> *o = NULL) : Controller(l, o){inteligence = i;};
	void provedAkci();
};

class ControllerShoraDolu : public Controller{	
public:
	ControllerShoraDolu(Objekt *l, std::set<Controller*> *o) : Controller(l, o){};
	void provedAkci();
};

class ControllerStrely : public Controller{
public:
	ControllerStrely(Objekt *l, std::set<Controller*> *o = NULL) : Controller(l, o){};	
	void provedAkci();
};

#endif


