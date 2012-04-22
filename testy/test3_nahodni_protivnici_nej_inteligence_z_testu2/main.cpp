
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
class ControllerFRNNZhoraDolu : public Controller{	
	FRNeuralNetwork *inteligence;
public:
	ControllerFRNNZhoraDolu(Objekt *l, FRNeuralNetwork *i) : Controller(l){
		inteligence = i;
	};
	void provedAkci(){
		std::vector<double> v = nejblizsiVzdalenostiObjektu(3);
		std::vector<double> vystupy = this->inteligence->update(v);	
				
		nahoru(vystupy[0]*2);	
		dolu(vystupy[1]*2);	
		vlevo(vystupy[2]*2);
		vpravo(vystupy[3]*2);

		l->posun(0, 1);
		if(this->getObjekt()->getPoziceY() > 240){
			l->posun(0, 2);
		}
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
	for(int j = 0; j < 1000; j++){
		if(citac == 0){
			space.addController(new ControllerZdolaNahoru(new Lod(&space, 0, 300)));
			space.addController(new ControllerZdolaNahoru(new Lod(&space, 150, 300)));
			space.addController(new ControllerZdolaNahoru(new Lod(&space, 300, 300)));
		}
		if(citac == 100){			
			space.addController(new ControllerZdolaNahoru(new Lod(&space, 75, 300)));
			space.addController(new ControllerZdolaNahoru(new Lod(&space, 225, 300)));
			space.addController(new ControllerZdolaNahoru(new Lod(&space, 350, 300)));
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

	/*NP = atoi(opt->getValue("NP"));
	F = atof(opt->getValue("F"));
	CR = atof(opt->getValue("CR")); 
	Generations = atoi(opt->getValue("generations"));
	pocet_neuronu = atoi(opt->getValue("neurons"));
	nejlepsi_jedinec = opt->getValue('n');
	data = opt->getValue('d');
	tichy = atoi(opt->getValue("quiet"));*/

	cout << "Parametry diferencialni evoluce:" << endl;
	cout << " -> Velikost populace NP = " << NP << endl;
	cout << " -> Mutacni konstanta F = " << F  << endl;
	cout << " -> Prah krizeni CR = " << CR << endl;
	cout << " -> Pocet generaci = " << Generations << endl;
	cout << " -> Pocet neuronu = " << pocet_neuronu << endl;

	//DiferencialniEvoluce *d1 = new DiferencialniEvoluce(NP, F, CR, Generations, pocet_neuronu, &ohodnoceni, tichy, data);
	//d1->UlozNejlepsiJedinec(nejlepsi_jedinec);
	
	// 32bit - pretece cca po 36 minutach
	t2 = clock();
	cout << "Doba behu programu: " << ((double) (t2 - t1))/CLOCKS_PER_SEC << endl;

	// klient cast
	
	Space space(400,300);		
	
	bool zacni = false;
	bool pauza = false;
	int kola = 0;

	// Create the main rendering window
    sf::RenderWindow App(sf::VideoMode(400, 300, 32), "NNSA");
	App.SetFramerateLimit(40);

	/*******************************************************/
	FRNeuralNetwork f1("nejlepsi_jedinec11.txt");
	//FRNeuralNetwork f2("nejlepsi_jedinec1.txt");

	Skudce j(&space, 175, 50);
	j.nastavPocitaniKolizi();
	Skudce k(&space, 200, 50);
	k.nastavPocitaniKolizi();

	Lod l(&space, 175, 50);
	l.nastavPocitaniKolizi();

	Controller *c = new ControllerFRNN(&l, &f1);
	c->nastavNesmrtelnost();
	space.addController(c);

	App.UseVerticalSync(true);

	sf::Sprite pozadi;
	sf::Image Image;
	Image.LoadFromFile("pics/bg.png");
	sf::Image cervena;
	cervena.LoadFromFile("pics/raketa2.tga");
	sf::Image zelena;
	zelena.LoadFromFile("pics/raketa.tga");
	sf::Image strela;
	strela.LoadFromFile("pics/strela.tga");

	pozadi.SetImage(Image);
	pozadi.Resize((float)400, (float)300);

	// nastaveni pisma
	sf::Font MyFont;
	MyFont.LoadFromFile("pics/MyriadWebPro-Bold.ttf", 50);

	sf::String UvodniText("Stiskni Enter...", MyFont, 20);
	sf::String DalsiText("pro pauzu stiskni P", MyFont, 10);
	sf::String PauzaText("pro pokracovani stiskni P", MyFont, 10);
	UvodniText.SetPosition(20, 150);
	DalsiText.SetPosition(20, 170);
	PauzaText.SetPosition(20, 170);

	int citac = 0;
	srand(0);
    // Start game loop
    while (App.IsOpened())
    {
        // Process events
        sf::Event Event;
        while (App.GetEvent(Event))
        {
            // Close window : exit
            if (Event.Type == sf::Event::Closed)
                App.Close();
			// Escape key : exit
            if ((Event.Type == sf::Event::KeyPressed) && (Event.Key.Code == sf::Key::Escape))
                App.Close();
			// Escape key : exit
			if ((Event.Type == sf::Event::KeyPressed) && (Event.Key.Code == sf::Key::Return))
                zacni = true;
			if ((Event.Type == sf::Event::KeyPressed) && (Event.Key.Code == sf::Key::P))
                pauza = !pauza;
        }
		App.Clear();
		if(pauza){
			App.Draw(PauzaText);
		}
		else if(zacni){
			App.Draw(pozadi);
			kola++;			
			std::cout << "\rKolo: " << kola << " Hodnoceni: " << c->getSkore();
			if(citac%97 == 0){
				FRNeuralNetwork *fx1 = new FRNeuralNetwork("nejlepsi_jedinec11.txt");
				Skudce *x = new Skudce(&space, nahodneCislo(0,350), 0);
				Controller *xx = new ControllerFRNNZhoraDolu(x, fx1);
				space.addController(xx);
			}

			space.ProvedKolo();	

			if(citac == 1000)
				citac = 0;
			else
				citac++;

			std::set<Controller*> *pole = space.GetVsechnyObjekty();
			
			std::set<Controller*>::iterator ii;
			for(ii = pole->begin(); ii != pole->end(); ++ii){
				double x = (*ii)->getObjekt()->getPoziceX();		
				double y = (*ii)->getObjekt()->getPoziceY();	

				sf::Sprite raketa;
				if((*ii)->getObjekt()->getTyp() == 'r')
					raketa.SetImage(cervena);
				else if((*ii)->getObjekt()->getTyp() == 'z')
					raketa.SetImage(zelena);
				else if((*ii)->getObjekt()->getTyp() == 's')
					raketa.SetImage(strela);

				raketa.SetPosition((float)x, (float)y);
				App.Draw(raketa);
			}	
		}
		else{
			App.Draw(UvodniText);
			App.Draw(DalsiText);			
		}
        // Display window contents on screen
        App.Display();	
    }


	return 0;
}
