#include <utility>
#include <cmath>
#include "controller.h"
#include "frnn.h"

Controller::Controller(Objekt *l, std::set<Controller*> *objekty){
	this->l = l;
	skore = 0;
	pole = objekty;
}
Objekt* Controller::getObjekt(){
	return l;
}
int Controller::getSkore(){
	return skore;
}
void Controller::prictiSkore(int hodnota){
	skore += hodnota;
}
std::vector<double> Controller::nejblizsiObjekty(int pocetObjektu){	
	std::vector<std::pair<int, int> > n;
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
				std::pair<int, int> prom = n[i];
				n[i] = n[i+1];
				n[i+1] = prom;
			}
		}
	}

	for(unsigned int c = 0;( c < n.size()) && (c < pocetObjektu); c++){
		vysledek.push_back(n[c].first);
		vysledek.push_back(n[c].second);
	}
	while(vysledek.size() < pocetObjektu*2){
		vysledek.push_back(0);
		vysledek.push_back(0);
	}

	return vysledek;
}

bool srovnej(std::pair<std::pair<int, int>, double> i,std::pair<std::pair<int, int>, double> j) { 
	return (i.second < j.second); 
}

std::vector<double> Controller::nejblizsiVzdalenostiObjektu(int pocetObjektu){	
	std::vector<double> vysledek(pocetObjektu*3);//alokace mista - pozice (vlevo - 1) a vzdalenost	
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

	for(int i = 0; i < vysledek.size() && i < pocetObjektu*3; i++){
		vysledek[i] = p[i].second;
		vysledek[i+1] = p[i].first.first;
		vysledek[i+1] = p[i].first.second;
		i++;i++;
	}

	// doplneni pole na pozadovanou velikost a naplni je nulami
	while(true){		
		if(vysledek.size() > pocetObjektu)
			break;
		vysledek.push_back(0);
	}
	//vysledek.erase (vysledek.begin()+pocetObjektu, vysledek.end());// vymazani prebyvajicich vzdalenosti
	/*std::cout << "serazene pole: ";
	for(int i = 0; i < vysledek.size(); i++){
		std::cout << vysledek[i] << " ";
	}
	std::cout << "\n";*/
	return vysledek;
}
void ControllerClovek::provedAkci(){
	const sf::Input& Input = l->getApp()->GetInput();
	if (Input.IsKeyDown(sf::Key::Left))
		l->posunX(-2);
	if (Input.IsKeyDown(sf::Key::Right))
		l->posunX(2);
	if (Input.IsKeyDown(sf::Key::Up))
		l->posunY(-2);
	if (Input.IsKeyDown(sf::Key::Down))
		l->posunY(2);
	if(Input.IsKeyDown(sf::Key::Space))
		l->vystrel();				
}

void ControllerFRNN::provedAkci(){
	std::vector<double> v;
	v = nejblizsiVzdalenostiObjektu(2);
	//v = nejblizsiObjekty(3);
	//v.push_back((double)l->getPoziceX());
	//v.push_back((double)l->getPoziceY());

	std::vector<double> vystupy;	
	
	vystupy = this->inteligence->update(v);
				
	if(vystupy[0] > 0.5)
		l->posunX(vystupy[0]*2);
	else
		l->posunX(vystupy[0]*(-2));
	
	l->posunY(1);
	l->posunY(vystupy[1]*(-3));		
	//std::cout << "***pozice: " << this->getObjekt()->getPoziceX() << "-" << this->getObjekt()->getPoziceY() << ", skore: " << this->getSkore() << "\n";
}

void ControllerShoraDolu::provedAkci(){
	l->posunDown();
}

void ControllerStrely::provedAkci(){
	l->posun(0, -3);
}


