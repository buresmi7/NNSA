#include <vector>
#include <algorithm>
#include "space.h"

Space::Space(int sirka, int vyska){		
	this->vyska = vyska;
	this->sirka = sirka;		
	citac = 0;		
}
Space::~Space(){
	std::set<Controller*>::iterator i;
	for(i = pole.begin(); i != pole.end(); ++i){
		(*i)->~Controller();
	}
}
void Space::addController(Controller *c){
	c->addPoleObjektu(&pole);
	this->nove.insert(c);
}
bool Space::CircleTest(Objekt *a, Objekt *b){
	return std::sqrt((float)(std::pow((double)a->getPoziceX()+a->getSirka()/2 - b->getPoziceX()-b->getSirka()/2,2) + std::pow((double)a->getPoziceY()+a->getVyska()/2 - b->getPoziceY()-b->getVyska()/2,2))) < a->getPolomer() + b->getPolomer();
}
void Space::ProvedKolo(){	

	// aktivuje vsecky controlery a provede u nich jednu akci
	std::set<Controller*>::iterator ii;
	for(ii = pole.begin(); ii != pole.end(); ++ii){
		(*ii)->provedAkci();	
	}

	// pripoji nove objekty do "pole" objektu
	if(nove.size() != 0){	
		//std::set<Controller*> pom(pole.begin(), pole.end());
		//std::copy(nove.begin(), nove.end(), std::inserter(pole, pole.end()));
		std::set_union(nove.begin(), nove.end(), pole.begin(), pole.end(),std::inserter(pole, pole.end()));
		nove.clear();
	}

	// test na kolize vsech objektu a odecteni skore
	std::set<Controller*>::iterator i;
	std::set<Controller*>::iterator j;
	std::set<Controller*> vymaz;
	for(i = pole.begin(); i != pole.end(); ++i){				
		// vymazani objektu za spodnim okrajem
		if((*i)->getObjekt()->getPoziceY() > vyska){
			vymaz.insert((*i));			
		}	
		// vymazani objektu za hornim okrajem
		if((*i)->getObjekt()->getPoziceY() < -52){
			vymaz.insert((*i));			
		}
		// pokud je to objekt, ktery chceme ohodnocovat, a dostane se za okraj, jen mu odecteme strasne moc bodu, to ho nauci...
		/*if((*i)->getObjekt()->getPoziceY() < 10 || (*i)->getObjekt()->getPoziceY() > vyska - 60 || (*i)->getObjekt()->getPoziceX() < 10 || (*i)->getObjekt()->getPoziceX() > sirka - 60){
			if((*i)->getObjekt()->pocitejKolize()){				
				(*i)->prictiSkore(-10);
			}						
		}*/
		for(j = pole.begin(); j != pole.end(); ++j){
			if((*i) == (*j))continue; // pokud se porovnava stejny objekt, tak nepocitej kolize
			if((*i)->getObjekt()->pocitejKolize() == false && (*j)->getObjekt()->pocitejKolize() == false)// pocita kolize jen u tech objektu, u kterych chceme
				continue;
			if(CircleTest((*i)->getObjekt(), (*j)->getObjekt())){
				if((*i)->getObjekt()->getName() == 's'){
					if((*j)->getObjekt()->getName() != 's'){
						(*i)->getObjekt()->getStrelec()->prictiSkore(10);
						vymaz.insert((*i));

						(*j)->prictiSkore(-100);

						if((*j)->uberZivot() < 1)
							vymaz.insert((*j));
					}

				}
				
				(*i)->getObjekt()->melKolizi();
				(*j)->getObjekt()->melKolizi();

				int a = (*i)->getObjekt()->pocetKolizi();
				int b = (*j)->getObjekt()->pocetKolizi();

				(*i)->prictiSkore(-1*a);
				(*j)->prictiSkore(-1*b);			
			}
			else{
				// pokud nemeli objekty kolizi, tak se jim to nastavi, kvuli spravnemu pocitani
				(*i)->getObjekt()->nemelKolizi();
				(*j)->getObjekt()->nemelKolizi();
			}
		}
	}
	if(vymaz.size() != 0){
		for(ii = vymaz.begin(); ii != vymaz.end(); ++ii){
			(*ii)->~Controller();
			pole.erase((*ii));
		}
		vymaz.clear();
	}
}
void Space::VykresliVsechny(){	

	std::set<Controller*>::iterator ii;
	for(ii = pole.begin(); ii != pole.end(); ++ii){
		//(*ii)->getObjekt()->Draw();	

		//std::cout << "skore 1: " << (*ii)->getSkore() << "\n";			
	}		
}
std::set<Controller*>* Space::GetVsechnyObjekty(){
	return &pole;	
}



