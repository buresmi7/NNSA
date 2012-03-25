#ifndef CONTROLLER
#define CONTROLLER

#include <iostream>
#include <set>
#include "objekt.h"
#include "frnn.h"

class Controller{
protected:
	Objekt *l;
	int skore;
	std::set<Controller*> *pole;

	static bool razeni(std::pair<int , int> i,std::pair<int, int> j);
	std::vector<double> nejblizsiObjekty(int pocetObjektu);	
	std::vector<double> nejblizsiVzdalenostiObjektu(int pocetObjektu);
	std::vector<double> nejblizsiVzdalenostiObjektu2(int pocetObjektu);
public:
	Controller(Objekt *l, std::set<Controller*> *objekty = NULL);
	Controller& operator=(const Controller &c);
	virtual ~Controller();
	virtual void provedAkci() = 0;
	Objekt* getObjekt();
	int getSkore();
	void prictiSkore(int hodnota);
	void addPoleObjektu(std::set<Controller*> *o);
};

class ControllerStrely : public Controller{
public:
	ControllerStrely(Objekt *l, std::set<Controller*> *o = NULL) : Controller(l, o){};	
	void provedAkci();
};

#endif


