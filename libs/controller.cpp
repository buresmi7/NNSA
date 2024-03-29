#include <utility>
#include <cmath>
#include "controller.h"
#include "frnn.h"
#include <algorithm>

Controller::Controller(Objekt *l, std::set<Controller*> *objekty){
	this->l = l;
	this->skore = 0;
	this->nesmrtelnost = false;
	this->pocet_zivotu = 1;
	this->pole = objekty;
}
int Controller::uberZivot(){
	if(!nesmrtelnost)
		pocet_zivotu--;
	return pocet_zivotu;
}
Controller& Controller::operator=(const Controller &c){
		if(this != &c){
			this->l = c.l;
			this->skore = c.skore;
			this->pole = c.pole;
			this->pocet_zivotu = c.pocet_zivotu;
			this->nesmrtelnost = c.nesmrtelnost;
		}
		return *this;
	}
Controller::~Controller(){
	delete l;
}
int Controller::getSkore(){
	return skore;
}
void Controller::addPoleObjektu(std::set<Controller*> *o){
	pole = o;
}
Objekt* Controller::getObjekt(){
	return l;
}
void Controller::prictiSkore(int hodnota){
	skore += hodnota;
}
void Controller::vystrel(Controller* strelec){
	l->vystrel(strelec);
}
std::vector<double> Controller::nejblizsiObjekty(int pocetObjektu){	
	std::vector<std::pair<double, double> > n;
	std::vector<double> vysledek;
	if(pole == NULL) return vysledek;
	
	std::set<Controller*>::iterator j;
	for(j=pole->begin(); j!=pole->end(); j++){
		n.push_back(std::make_pair((*j)->getObjekt()->getPoziceX(), (*j)->getObjekt()->getPoziceY()));
	}	

	// serazeni pole	
	for(unsigned int c = 0; c < n.size() - 1; c++){
		for(unsigned int i = 0; i < n.size() - 1; i++){
			double a = std::sqrt((float)(std::pow((double)this->getObjekt()->getPoziceX() - n[i].first,2) + std::pow((double)this->getObjekt()->getPoziceY() - n[i].second,2)));
			double b = std::sqrt((float)(std::pow((double)this->getObjekt()->getPoziceX() - n[i+1].first,2) + std::pow((double)this->getObjekt()->getPoziceY() - n[i+1].second,2)));
			if(a > b){
				std::pair<double, double> prom = n[i];
				n[i] = n[i+1];
				n[i+1] = prom;
			}
		}
	}

	for(int c = 0;( c < (int)n.size()) && (c < pocetObjektu); c++){
		vysledek.push_back(n[c].first);
		vysledek.push_back(n[c].second);
	}
	while((int)vysledek.size() < pocetObjektu*2){
		vysledek.push_back(0);
		vysledek.push_back(0);
	}

	return vysledek;
}

bool srovnej(std::pair<std::pair<int, int>, double> i,std::pair<std::pair<int, int>, double> j) { 
	return (i.second < j.second); 
}
bool srovnej2(double i,double j) { 
	return (i < j); 
}
std::vector<double> Controller::nejblizsiVzdalenostiObjektu2(int pocetObjektu){	
	std::vector<double> vysledek(pocetObjektu, 0);//alokace mista - pozice (vlevo - 1) a vzdalenost	
	std::vector<double> p;
	
	std::set<Controller*>::iterator j;
	for(j=pole->begin(); j!=pole->end(); j++){		
		p.push_back(std::sqrt((float)(std::pow((double)this->getObjekt()->getPoziceX() - (*j)->getObjekt()->getPoziceX(),2) + std::pow((double)this->getObjekt()->getPoziceY() - (*j)->getObjekt()->getPoziceY(),2))));		
	}	
	sort(p.begin(),p.end(), srovnej2);
	p.erase(p.begin());// vymazani prvni vzdalenosti - je nula	

	for(int i = 0; i < pocetObjektu; i++){
		vysledek[i] = p[i];
	}
	return vysledek;
}

std::vector<double> Controller::nejblizsiVzdalenostiObjektu(int pocetObjektu){	
	std::vector<double> vysledek(pocetObjektu*3, 0);//alokace mista - pozice (vlevo - 1) a vzdalenost	
	std::vector<std::pair<std::pair<int, int>, double> > p;
	
	std::set<Controller*>::iterator j;
	for(j=pole->begin(); j!=pole->end(); j++){
		int vlevo = -1;
		int nahore = -1;
		if(this->getObjekt()->getPoziceX() < (*j)->getObjekt()->getPoziceX())
			vlevo = 1;
		if(this->getObjekt()->getPoziceY() < (*j)->getObjekt()->getPoziceY())
			nahore = 1;
		// naplneni pole vzdalenostmi od ostatnich objektu
		p.push_back(std::make_pair(std::make_pair(vlevo, nahore),std::sqrt((float)(std::pow((double)this->getObjekt()->getPoziceX() - (*j)->getObjekt()->getPoziceX(),2) + std::pow((double)this->getObjekt()->getPoziceY() - (*j)->getObjekt()->getPoziceY(),2)))));		
	}	
	sort(p.begin(),p.end(), srovnej);
	p.erase(p.begin());// vymazani prvni vzdalenosti - je nula	

	int k = 0;
	for(int i = 0; i < (int)p.size() && i < pocetObjektu; i++){
		vysledek[k] = p[i].second;
		vysledek[k+1] = p[i].first.first;
		vysledek[k+2] = p[i].first.second;
		k += 3;
	}
	return vysledek;
}


