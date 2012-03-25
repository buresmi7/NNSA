#ifndef OBJEKT
#define OBJEKT

#include <iostream>
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
	int pocetPredchozichKolizi;
	Space *space;
	int pocitadlo;
	Objekt* vlastnik;
public:	
	Objekt(Space *s, double pozice_x = 0, double pozice_y = 0, Objekt *vlastnik = NULL);
	Objekt(const Objekt &o);
	void nastavPocitaniKolizi(){pocitej_kolize = true;};
	bool pocitejKolize(){return pocitej_kolize;};
	void nastavPozici(int x, int y);
	void melKolizi(){pocetPredchozichKolizi++;};
	void nemelKolizi(){pocetPredchozichKolizi = 0;};
	int pocetKolizi(){return pocetPredchozichKolizi;};
	void Draw();
	double getPoziceX();
	double getPoziceY();
	int getPolomer(){return polomer;};

	void posun(int x, int y);
	void posunX(double x);
	void posunY(double y);
	void vystrel();
	virtual char getName() = 0;
	virtual char getTyp() = 0;
	void nastavVlastnika(Objekt *v){vlastnik = v;};
	Objekt* getVlastnik(){return vlastnik;};
};

class Lod : public Objekt{
public:
	Lod(Space *s = NULL, double pozice_x = 0, double pozice_y = 0, Objekt *vlastnik = NULL);	
	char getName(){return 'n';};
	char getTyp(){return 'z';};
};

class Skudce : public Objekt{
public:
	Skudce(Space *s = NULL, double pozice_x = 0, double pozice_y = 0, Objekt *vlastnik = NULL);
	char getName(){return 'n';};
	char getTyp(){return 'r';};
};

class Strela : public Objekt{
public:
	Strela(Space *s = NULL, double pozice_x = 0, double pozice_y = 0, Objekt *vlastnik = NULL);
	char getName(){return 's';};
	char getTyp(){return 's';};
};
#endif

