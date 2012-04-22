#include "objekt.h"
#include "space.h"

Objekt::Objekt(Space* s, double pozice_x, double pozice_y){
	this->pozice_x = pozice_x;
	this->pozice_y = pozice_y;
	sirka = 0;
	vyska = 0;
	polomer = 0;
	space = s;
	pocitadlo = 0;
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
		if(pozice_x < space->getSirka() - sirka - 1)
			pozice_x += x;
	}
	else{
		if(pozice_x > 0 + 1)
			pozice_x += x;
	}
}
void Objekt::posunY(double y){
	if(y > 0){
		if(pozice_y < space->getVyska() - vyska - 1)
			pozice_y += y;
	}
	else{
		if(pozice_y > 0 + 30)
			pozice_y += y;
	}
}
void Objekt::vystrel(Controller *strelec){
	Strela *s;
	Controller *c;
	if(strelec->getObjekt()->getTyp() == 'z'){
		s = new Strela(space, pozice_x+20, pozice_y - 20, strelec);
		c = new ControllerStrelyNahoru(s);
	}
	else{
		s = new Strela(space, pozice_x+20, pozice_y + 60, strelec);
		c = new ControllerStrelyDolu(s);
	}
	s->nastavPocitaniKolizi();
	space->addController(c);

}

Lod::Lod(Space* s, double pozice_x, double pozice_y) : Objekt(s, pozice_x, pozice_y){
	polomer = 25;
	vyska = 52;
	sirka = 52;
	return;

}	

Skudce::Skudce(Space* s, double pozice_x, double pozice_y) : Objekt(s, pozice_x, pozice_y){
	polomer = 25;
	vyska = 52;
	sirka = 52;
	return;
}

Strela::Strela(Space* s, double pozice_x, double pozice_y, Controller *strelec) : Objekt(s, pozice_x, pozice_y){
	polomer = 8;
	vyska = 15;
	sirka = 15;
	this->strelec = strelec;
	return;
}