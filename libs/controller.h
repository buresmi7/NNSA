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
	int pocet_zivotu;
	bool nesmrtelnost;
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
	int uberZivot();
	void nastavNesmrtelnost(){nesmrtelnost = true;};
	bool jeNesmrtelny(){return nesmrtelnost;};
};

class ControllerStrelyDolu : public Controller{
public:
	ControllerStrelyDolu (Objekt *l, std::set<Controller*> *o = NULL) : Controller(l, o){};	
	void provedAkci(){l->posun(0, 3);};
};
class ControllerStrelyNahoru : public Controller{
public:
	ControllerStrelyNahoru (Objekt *l, std::set<Controller*> *o = NULL) : Controller(l, o){};	
	void provedAkci(){l->posun(0, -3);};
};

#endif


