#include "objekt.h"
#include "space.h"

Objekt::Objekt(Space* s, double pozice_x, double pozice_y, Objekt *vlastnik){
	this->pozice_x = pozice_x;
	this->pozice_y = pozice_y;
	sirka = 0;
	vyska = 0;
	polomer = 0;
	space = s;
	pocitadlo = 0;
	this->vlastnik = vlastnik;
	pocitej_kolize = false;
	pocetPredchozichKolizi = 0;
}
Objekt::Objekt(const Objekt &o){
	pozice_x = o.pozice_x;
	pozice_y = o.pozice_y;
	sirka = o.sirka;
	vyska = o.vyska;	
	polomer = o.polomer;
	space = o.space;
	pocitej_kolize = o.pocitej_kolize;
	pocetPredchozichKolizi = o.pocetPredchozichKolizi;
}
Objekt& Objekt::operator=(const Objekt &o){
	if(this != &o){
		pozice_x = o.pozice_x;
		pozice_y = o.pozice_y;
		sirka = o.sirka;
		vyska = o.vyska;	
		polomer = o.polomer;
		space = o.space;
		pocitej_kolize = o.pocitej_kolize;
		pocetPredchozichKolizi = o.pocetPredchozichKolizi;
	}
	return *this;
}
void Objekt::nastavPozici(int x, int y){
	pozice_x = x;
	pozice_y = y;
}

double Objekt::getPoziceX(){
	return pozice_x;
}
double Objekt::getPoziceY(){
	return pozice_y;
}
void Objekt::posun(int x, int y){
	pozice_x += x;
	pozice_y += y;
}
void Objekt::posunX(double x){
	if(x > 0){
		if(pozice_x < space->getSirka() - sirka - 10)
			pozice_x += x;
	}
	else{
		if(pozice_x > 0 + 10)
			pozice_x += x;
	}
}
void Objekt::posunY(double y){
	if(y > 0){
		if(pozice_y < space->getVyska() - vyska - 10)
			pozice_y += y;
	}
	else{
		if(pozice_y > 0 + 10)
			pozice_y += y;
	}
}
void Objekt::vystrel(){
	if(pocitadlo == 0)
		space->addController(new ControllerStrely(new Strela(space, pozice_x + sirka/2, pozice_y + 5, this)));
	pocitadlo++;
	if(pocitadlo == 10)
		pocitadlo = 0;
}

Lod::Lod(Space* s, double pozice_x, double pozice_y, Objekt *vlastnik) : Objekt(s, pozice_x, pozice_y){
	polomer = 25;
	vyska = 52;
	sirka = 52;
	return;
	
}	

Skudce::Skudce(Space* s, double pozice_x, double pozice_y, Objekt *vlastnik) : Objekt(s, pozice_x, pozice_y){
	polomer = 25;
	vyska = 52;
	sirka = 52;
	return;
}

Strela::Strela(Space* s, double pozice_x, double pozice_y, Objekt *vlastnik) : Objekt(s, pozice_x, pozice_y, vlastnik){
	vyska = 52;
	sirka = 52;
	return;
}



