#include "space.h"
#include <cmath>

Space::Space(sf::RenderWindow *App){	
	this->App = App;	
	// pokud je App == NULL => program byl spusten jako generator -> okeni system je vypnuty
	if(App == NULL){		
		vyska = 400;
		sirka = 600;		
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
	if(citac%52 == 0)
		pole.insert(new ControllerShoraDolu(new Skudce(App, this, genrand(0, sirka-52), 0), &pole));
	//if(citac%90 == 0)
		//pole.insert(new ControllerShoraDolu(new Skudce(App, this, genrand(0, sirka-52), 0), &pole));
	
	if(citac%300 == 0){
		pole.insert(new ControllerShoraDolu(new Skudce(App, this, 0, 0), &pole));
		pole.insert(new ControllerShoraDolu(new Skudce(App, this, sirka-52, 0), &pole));
		pole.insert(new ControllerShoraDolu(new Skudce(App, this, 275, 0), &pole));
	}
	if(citac == 400)
		citac = 0;
	else
		citac++;

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
		// vymazani objektu mimo mapu
		if((*i)->getObjekt()->getPoziceY() < 0 || (*i)->getObjekt()->getPoziceY() > vyska){
			vymaz.insert((*i));			
		}	
		// pokud je to objekt, ktery chceme ohodnocovat, a dostane se za okraj, jen mu odecteme strasne moc bodu, to ho nauci...
		/*if((*i)->getObjekt()->getPoziceY() <= 52 || (*i)->getObjekt()->getPoziceY() >= vyska - 52 || (*i)->getObjekt()->getPoziceX() <= 52 || (*i)->getObjekt()->getPoziceX() >= sirka - 52){
			if((*i)->getObjekt()->pocitejKolize()){				
				(*i)->prictiSkore(-10);
			}						
		}*/
		for(j = pole.begin(); j != pole.end(); ++j){
			if((*i) == (*j))continue; // pokud se porovnava stejny objekt, tak nepocitej kolize
			if((*i)->getObjekt()->pocitejKolize() == false && (*j)->getObjekt()->pocitejKolize() == false)// pocita kolize jen u tech objektu, u kterych chceme
				continue;
			if(CircleTest((*i)->getObjekt(), (*j)->getObjekt())){
				(*i)->getObjekt()->melKolizi();
				(*j)->getObjekt()->melKolizi();

				int a = (*i)->getObjekt()->pocetKolizi();
				int b = (*j)->getObjekt()->pocetKolizi();

				(*i)->prictiSkore(-1*a*a);
				(*j)->prictiSkore(-1*b*b);				
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
	App->Draw(pozadi);	

	std::set<Controller*>::iterator ii;
	for(ii = pole.begin(); ii != pole.end(); ++ii){
		(*ii)->getObjekt()->Draw();	

		//std::cout << "skore 1: " << (*ii)->getSkore() << "\n";			
	}		
}



