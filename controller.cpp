
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

	/*std::cout << "neserazene pole: \n";
	std::vector<std::pair<int, int> >::iterator i;
	for(i =n.begin(); i!=n.end(); i++){
		std::cout << (*i).first << "-" << (*i).second << " ";
	}*/
	//std::cout << std::endl;

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

void ControllerClovek::provedAkci(){
	const sf::Input& Input = l->getApp()->GetInput();
	if (Input.IsKeyDown(sf::Key::Left))
		l->posunLeftD();
	if (Input.IsKeyDown(sf::Key::Right))
		l->posunRightD();
	if (Input.IsKeyDown(sf::Key::Up))
		l->posunUpD();
	if (Input.IsKeyDown(sf::Key::Down))
		l->posunDownD();
	if(Input.IsKeyDown(sf::Key::Space))
		l->vystrel();			
}

void ControllerFRNN::provedAkci(){
	std::vector<double> v;
	v = nejblizsiObjekty(3);

	/*std::cout << "nejblizsi objekty: ";
	std::vector<double>::iterator i;
	for(i =v.begin(); i!=v.end(); i++){
		std::cout << (*i) << ", ";
	}
	std::cout << std::endl;*/

	std::vector<double> vystupy;	
	
	vystupy = this->inteligence->update(v);

	/*std::vector<double>::iterator j;
		std::cout << "vystupy FRNN: ";
		for(j=vystupy.begin(); j!=vystupy.end(); j++){
			std::cout << (*j) << " ";
		}
		std::cout << std::endl;
		std::cout << "skore: " << this->skore << "\n";*/
	
	if (vystupy[0] > 0.5)
		l->posunLeftD();
	else if (vystupy[1] > 0.5)
		l->posunRightD();
	else if (vystupy[2] > 0.5)
		l->posunUpD();
	else if (vystupy[3] > 0.5)
		l->posunDownD();
	//if(vystupy[4] > 0.5)
		//l->vystrel();			
	//std::cout << "***pozice: " << this->getObjekt()->getPoziceX() << "-" << this->getObjekt()->getPoziceY() << ", skore: " << this->getSkore() << "\n";
}

void ControllerShoraDolu::provedAkci(){
	l->posunDown();
}

void ControllerStrely::provedAkci(){
	l->posun(0, -3);
}


