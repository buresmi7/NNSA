#ifndef OBJEKT
#define OBJEKT

#include <iostream>
//#include "controller.h"

class Controller;
class Space;

class Objekt{
protected:
	double pozice_x;
	double pozice_y;
	int polomer;
	int sirka;
	int vyska;
	bool pocitej_kolize;
	int pocetPredchozichKolizi;
	Space *space;
	int pocitadlo;
	Controller *strelec;
public:	
	Objekt(Space *s, double pozice_x = 0, double pozice_y = 0);
	Objekt(const Objekt &o);
	Objekt& operator=(const Objekt &o);
	void nastavPocitaniKolizi(){pocitej_kolize = true;};
	bool pocitejKolize(){return pocitej_kolize;};
	void nastavPozici(int x, int y);
	void melKolizi(){pocetPredchozichKolizi++;};
	void nemelKolizi(){pocetPredchozichKolizi = 0;};
	int pocetKolizi(){return pocetPredchozichKolizi;};
	void Draw();
	double getPoziceX();
	double getPoziceY();
	int getVyska(){return vyska;};
	int getSirka(){return sirka;};
	int getPolomer(){return polomer;};

	void posun(int x, int y);
	void posunX(double x);
	void posunY(double y);
	void vystrel(Controller *strelec);
	virtual char getName() = 0;
	virtual char getTyp() = 0;
	virtual Controller* getStrelec() = 0;
};

class Lod : public Objekt{
public:
	Lod(Space *s = NULL, double pozice_x = 0, double pozice_y = 0);	
	char getName(){return 'n';};
	char getTyp(){return 'z';};
	Controller *getStrelec(){return NULL;};
};

class Skudce : public Objekt{
public:
	Skudce(Space *s = NULL, double pozice_x = 0, double pozice_y = 0);
	char getName(){return 'n';};
	char getTyp(){return 'r';};
	Controller *getStrelec(){return NULL;};
};

class Strela : public Objekt{	
public:
	Strela(Space *s = NULL, double pozice_x = 0, double pozice_y = 0, Controller *strelec = NULL);
	char getName(){return 's';};
	char getTyp(){return 's';};
	Controller *getStrelec(){return strelec;};
};
#endif

