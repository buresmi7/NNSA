
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

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
#include "libs/anyoption.h"

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
				
		nahoru(vystupy[0]*2);	
		dolu(vystupy[1]*2);	
		vlevo(vystupy[2]*2);
		vpravo(vystupy[3]*2);

	};
};
class ControllerZdolaNahoru : public Controller{	
public:
	ControllerZdolaNahoru(Objekt *l) : Controller(l){
		this->pocet_zivotu = 5;
	};
	void provedAkci(){
		l->posun(0, -1);
	};
};


// funkce ohodnoceni, dulezita pro vybirani kvality jedincu	
int ohodnoceni(FRNeuralNetwork *f){	
	srand(0);
	Space space(400,300);	
	Skudce *l = new Skudce(&space, 175, 50);
	l->nastavPocitaniKolizi();
	Controller *c = new ControllerFRNN(l, f);	
	c->nastavNesmrtelnost();
	space.addController(c);

	int citac = 0;
	for(int j = 0; j < 3000; j++){
		if(citac%77 == 0){
				space.addController(new ControllerZdolaNahoru(new Lod(&space, nahodneCislo(50,300), 300)));
			}
			if(citac%149 == 0){				
					space.addController(new ControllerZdolaNahoru(new Lod(&space, 0, 300)));
				
					}
			if(citac%161 == 0){	
				space.addController(new ControllerZdolaNahoru(new Lod(&space, 350, 300)));
			}

		space.ProvedKolo();	

		if(citac == 1000)
			citac = 0;
		else
			citac++;
	}
	int skore = c->getSkore();
	srand((unsigned int)time(0));
	return skore;
}

int main(int argc, char **argv){
	// nekonstantni nastaveni generatoru nahodnych cisel
	srand((unsigned int)time(0));
	// server cast
	clock_t t1, t2;
	t1 = clock();	

	int NP = 50;
	double F = 0.7; 
	double CR = 0.6; 
	int Generations = 10; 
	int pocet_neuronu = 5; 
	char *nejlepsi_jedinec = "nejlepsi_jedinec.txt";
	char *data = "data.txt";
	int tichy = 0;

	// parsovani parametru
	AnyOption *opt = new AnyOption();
	opt->setVerbose(); /* print warnings about unknown options */
    opt->autoUsagePrint(true); /* print usage for bad options */

	opt->addUsage( "" );
	opt->addUsage( "NNSA (Neural Network Space Arcade) - Testovani rekuretni neuronove site na jednoduche arkade" );
    opt->addUsage( "" );
	opt->addUsage( "Pouziti: " );
    opt->addUsage( "" );
    opt->addUsage( " -h   --help		Zobrazi tuto napovedu " );    
	opt->addUsage( " -n jedinec.txt	Nejlepsi jedinec " ); 
	opt->addUsage( " -d data.txt		Udaje o jednotlivych generacich " ); 
	opt->addUsage( " " );
	opt->addUsage( "      --NP 60	Velikost populace " );
	opt->addUsage( "      --F 0.7	Mutacni konstanta " );
	opt->addUsage( "      --CR 0.8	Prah krizeni " );
	opt->addUsage( "      --generations 100	Pocet kol slechteni populace " );
	opt->addUsage( "      --neurons 4	Pocet neuronu v neuronove siti " );
	opt->addUsage( "      --quiet 1		Neukecany rezim " );
    opt->addUsage( "" );

	opt->setFlag( "help", 'h');   /* a flag (takes no argument), supporting long and short form */
	opt->setOption('n');
	opt->setOption('d');
	opt->setOption("NP");
	opt->setOption("F");
	opt->setOption("CR");
	opt->setOption("generations");
	opt->setOption("neurons");
	opt->setOption("quiet");
	
	opt->processCommandArgs(argc, argv);

	NP = atoi(opt->getValue("NP"));
	F = atof(opt->getValue("F"));
	CR = atof(opt->getValue("CR")); 
	Generations = atoi(opt->getValue("generations"));
	pocet_neuronu = atoi(opt->getValue("neurons"));
	nejlepsi_jedinec = opt->getValue('n');
	data = opt->getValue('d');
	tichy = atoi(opt->getValue("quiet"));

	cout << "Parametry diferencialni evoluce:" << endl;
	cout << " -> Velikost populace NP = " << NP << endl;
	cout << " -> Mutacni konstanta F = " << F  << endl;
	cout << " -> Prah krizeni CR = " << CR << endl;
	cout << " -> Pocet generaci = " << Generations << endl;
	cout << " -> Pocet neuronu = " << pocet_neuronu << endl;

	DiferencialniEvoluce *d1 = new DiferencialniEvoluce(NP, F, CR, Generations, pocet_neuronu, &ohodnoceni, tichy, data);
	d1->UlozNejlepsiJedinec(nejlepsi_jedinec);
	

	// 32bit - pretece cca po 36 minutach
	t2 = clock();
	cout << "Doba behu programu: " << ((double) (t2 - t1))/CLOCKS_PER_SEC << endl;


	return 0;
}
