
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

using namespace std;

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
int ohodnoceni(FRNeuralNetwork *f){	
	Space *s = new Space(400,300);	
	Lod *l = new Lod(s, 175, 150);
	l->nastavPocitaniKolizi();
	ControllerFRNN *c = new ControllerFRNN(l, f);			
	s->addController(c);

	int citac = 0;
	for(int j = 0; j < 600; j++){
		if(citac == 0){
			s->addController(new ControllerShoraDolu(new Skudce(s, 0, 0)));
			s->addController(new ControllerShoraDolu(new Skudce(s, 150, 0)));
			s->addController(new ControllerShoraDolu(new Skudce(s, 300, 0)));
		}
		if(citac == 100){			
			s->addController(new ControllerShoraDolu(new Skudce(s, 75, 0)));
			s->addController(new ControllerShoraDolu(new Skudce(s, 225, 0)));
			s->addController(new ControllerShoraDolu(new Skudce(s, 350, 0)));
		}

		s->ProvedKolo();	

		if(citac == 200)
			citac = 0;
		else
			citac++;
	}
	int skore = c->getSkore();
	delete s;
	return skore;
}

int main(){
	//srand((unsigned int)time(0));
	// server cast
	clock_t t1, t2;
	t1 = clock();
	// nekonstantni nastaveni generatoru nahodnych cisel

	int NP = 100;
	double F = 0.7; 
	double CR = 0.3; 
	int Generations = 100; 
	int pocet_neuronu = 15; 

	DiferencialniEvoluce *d1 = new DiferencialniEvoluce(NP, F, CR, Generations, pocet_neuronu, &ohodnoceni);
	d1->UlozNejlepsiJedinec("nejlepsi_jedinec.txt");

	
	//std::vector<std::pair <int, FRNeuralNetwork *> > populace = d1->getPopulace();
	//DiferencialniEvoluce *d2 = new DiferencialniEvoluce(NP, F, CR, Generations, pocet_neuronu, &ohodnoceni, &populace);
	//d2->UlozNejlepsiJedinec("nejlepsi_jedinec2.txt");


	// 32bit - pretece cca po 36 minutach
	t2 = clock();
	cout << "Doba behu programu: " << ((double) (t2 - t1))/CLOCKS_PER_SEC << endl;

	// klient cast
	
	// nacteni vstupniho souboru
	std::vector<double> vahy;		
	double s;
	ifstream infile;
	infile.open ("nejlepsi_jedinec.txt");
	infile >> s;
	//int pocet_neuronu = (int)s;
	while(!infile.eof()){
		infile >> s;		
		vahy.push_back(s);
		//cout << s << endl;
	}
	infile.close();		

	Space space(400,300);		
	
	FRNeuralNetwork f(pocet_neuronu);
	f.nastavVahy(vahy);
	//f.nastavPocetVstupu(3*3);

	Lod l(&space, 175, 150);
	l.nastavPocitaniKolizi();

	ControllerFRNN *c = new ControllerFRNN(&l, &f);
	space.addController(c);
	
	bool zacni = false;
	bool pauza = false;
	int kola = 0;

	// Create the main rendering window
    sf::RenderWindow App(sf::VideoMode(400, 300, 32), "NNSA");
	App.SetFramerateLimit(40);

	App.UseVerticalSync(true);

	sf::Sprite pozadi;
	sf::Image Image;
	Image.LoadFromFile("pics/bg.png");
	sf::Image cervena;
	cervena.LoadFromFile("pics/raketa2.tga");
	sf::Image zelena;
	zelena.LoadFromFile("pics/raketa.tga");
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
			std::cout << "Kolo: " << kola << " Hodnoceni: " << c->getSkore() << "\n";

			space.ProvedKolo();	
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
