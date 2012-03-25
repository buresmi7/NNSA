
#include <iostream>
#include <fstream>
#include <time.h>
#include <sstream>
#include <string>


#include "libs/space.h"
#include "libs/de.h"
#include "libs/controller.h"
#include "libs/objekt.h"
#include "libs/frnn.h"

using namespace std;

double nahodneCislo(double fMin, double fMax){
	double f;
	f = (double)rand() / RAND_MAX;		
	return fMin + f * (fMax - fMin);
}

class ControllerFRNN : public Controller{	
	FRNeuralNetwork *inteligence;
public:
	ControllerFRNN(Objekt *l, FRNeuralNetwork *i) : Controller(l){
		inteligence = i;
	};
	void provedAkci(){
		std::vector<double> v = nejblizsiVzdalenostiObjektu(3);
		std::vector<double> vystupy = this->inteligence->update(v);	
				
		l->posunX(vystupy[0]*2);	
		l->posunX(vystupy[1]*(-2));	
		l->posunY(vystupy[2]*2);
		l->posunY(vystupy[3]*(-2));
	};
};
class ControllerShoraDolu : public Controller{	
public:
	ControllerShoraDolu(Objekt *l) : Controller(l){};
	void provedAkci(){
		l->posun(0, 1);
	};
};

// funkce ohodnoceni, dulezita pro vybirani kvality jedincu	
int ohodnoceni2(FRNeuralNetwork *f){	
	srand(0);
	Space space(400,300);	
	Lod *l = new Lod(&space, 175, 150);
	l->nastavPocitaniKolizi();
	ControllerFRNN *c = new ControllerFRNN(l, f);			
	space.addController(c);

	int citac = 0;
	for(int j = 0; j < 1000; j++){
		if(citac%55 == 0){
				space.addController(new ControllerShoraDolu(new Skudce(&space, nahodneCislo(0,350), 0)));
		}
		if(citac%85 == 0){
			space.addController(new ControllerShoraDolu(new Skudce(&space, nahodneCislo(0,350), 0)));
		}
		if(citac%105 == 0){
			space.addController(new ControllerShoraDolu(new Skudce(&space, nahodneCislo(0,350), 0)));
		}

		space.ProvedKolo();	

		if(citac == 200)
			citac = 0;
		else
			citac++;
	}
	int skore = c->getSkore();
	srand((unsigned int)time(0));
	return skore;
}

int ohodnoceni(FRNeuralNetwork *f){	
	Space space(400,300);	
	Lod *l = new Lod(&space, 175, 150);
	l->nastavPocitaniKolizi();
	ControllerFRNN *c = new ControllerFRNN(l, f);			
	space.addController(c);

	int citac = 0;
	for(int j = 0; j < 600; j++){
		if(citac == 0){
			space.addController(new ControllerShoraDolu(new Skudce(&space, 0, 0)));
			space.addController(new ControllerShoraDolu(new Skudce(&space, 150, 0)));
			space.addController(new ControllerShoraDolu(new Skudce(&space, 300, 0)));
		}
		if(citac == 100){			
			space.addController(new ControllerShoraDolu(new Skudce(&space, 75, 0)));
			space.addController(new ControllerShoraDolu(new Skudce(&space, 225, 0)));
			space.addController(new ControllerShoraDolu(new Skudce(&space, 350, 0)));
		}

		space.ProvedKolo();	

		if(citac == 200)
			citac = 0;
		else
			citac++;
	}
	int skore = c->getSkore();
	return skore;
}

int main(){
	srand((unsigned int)time(0));
	// server cast
	clock_t t1, t2;
	t1 = clock();
	// nekonstantni nastaveni generatoru nahodnych cisel

	int NP = 200;
	double F = 0.7; 
	double CR = 0.3; 
	int Generations = 30; 
	int pocet_neuronu = 15; 

	DiferencialniEvoluce *d1 = new DiferencialniEvoluce(NP, F, CR, Generations, pocet_neuronu, &ohodnoceni);
	d1->UlozNejlepsiJedinec("nejlepsi_jedinec.txt");

	
	std::vector<std::pair <int, FRNeuralNetwork *> > populace = d1->getPopulace();
	DiferencialniEvoluce *d2 = new DiferencialniEvoluce(NP, F, CR, Generations, pocet_neuronu, &ohodnoceni2, &populace);
	d2->UlozNejlepsiJedinec("nejlepsi_jedinec.txt");


	// 32bit - pretece cca po 36 minutach
	t2 = clock();
	cout << "Doba behu programu: " << ((double) (t2 - t1))/CLOCKS_PER_SEC << endl;


	return 0;
}
