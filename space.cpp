#include "space.h"
#include <cmath>

Space::Space(sf::RenderWindow *App){	
	this->App = App;	
	// pokud je App == NULL => program byl spusten jako generator -> okeni system je vypnuty
	if(App == NULL){		
		vyska = 300;
		sirka = 400;		
	}
	else{
		vyska = App->GetHeight();
		sirka = App->GetWidth();
		//segfault!!!		
		Image.LoadFromFile("bg.png");		
		pozadi.SetImage(Image);
		pozadi.Resize((float)sirka, (float)vyska);
	}
	citac = 0;		
	citac2 = 0;
}
sf::RenderWindow* Space::getApp(){
	return this->App;
}
void Space::addController(Controller *c){
	c->addPoleObjektu(&pole);
	nove.insert(c);
}bool Space::CircleTest(Objekt *a, Objekt *b){
	return std::sqrt((float)(std::pow((double)a->getPoziceX() - b->getPoziceX(),2) + std::pow((double)a->getPoziceY() - b->getPoziceY(),2))) < a->getPolomer() + b->getPolomer();
}
void Space::ProvedKolo(){	
	// generovani nepratel
	if((citac)%50 == 0)
		pole.insert(new ControllerShoraDolu(new Skudce(App, this, genrand(0, sirka-40), 0), &pole));
	if(citac%150 == 0){
		pole.insert(new ControllerShoraDolu(new Skudce(App, this, 0, 0), &pole));
		pole.insert(new ControllerShoraDolu(new Skudce(App, this, sirka-40, 0), &pole));
		pole.insert(new ControllerShoraDolu(new Skudce(App, this, 180, 0), &pole));
	}	
	if(citac == 400)
		citac = 0;
	else
		citac++;

	
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
		// vymazani objektu mimo mapu
		if((*i)->getObjekt()->getPoziceY() < 0 || (*i)->getObjekt()->getPoziceY() > vyska){
			vymaz.insert((*i));
			//std::cout << "objekt vymazan " << std::endl;
		}		
		for(j = pole.begin(); j != pole.end(); ++j){
			if((*i) == (*j))break;
			//if((*i)->getObjekt()->pocitejKolize() == false && (*j)->getObjekt()->pocitejKolize() == false)
				//break;
			if(CircleTest((*i)->getObjekt(), (*j)->getObjekt())){
				(*i)->prictiSkore(-1);
				(*j)->prictiSkore(-1);				
				/*if((*i)->getObjekt()->getName() == 's' && (*j)->getObjekt()->getName() == 's'){
					//std::cout << "kolize dvou strel " << std::endl;
				}
				else if((*i)->getObjekt()->getName() == 's' || (*j)->getObjekt()->getName() == 's'){
					if((*i)->getObjekt()->getVlastnik() != NULL && (*i)->getObjekt()->getVlastnik() == (*j)->getObjekt())
						break;
					if((*j)->getObjekt()->getVlastnik() != NULL && (*j)->getObjekt()->getVlastnik() == (*i)->getObjekt())
						break;
					vymaz.insert((*i));
					vymaz.insert((*j));
					//std::cout << "objekty vymazany" << std::endl;
				}				
				*/
				//std::cout << "kolize ";
			}
		}
	}
	if(vymaz.size() != 0){
		//std::set_difference(pole->begin(), pole->end(), vymaz.begin(), vymaz.end(), pole->begin());
		//std::set_difference(pole.begin(), pole.end(), vymaz.begin(), vymaz.end(), std::inserter(pole, pole.end()));
		for(ii = vymaz.begin(); ii != vymaz.end(); ++ii){
			(*ii)->~Controller();
			pole.erase((*ii));
		}
		vymaz.clear();
	}
}
void Space::VykresliVsechny(){
	App->Draw(pozadi);	

	std::set<Controller*>::iterator ii;
	for(ii = pole.begin(); ii != pole.end(); ++ii){
		(*ii)->getObjekt()->Draw();	

		//std::cout << "skore 1: " << (*ii)->getSkore() << "\n";			
	}		
}



