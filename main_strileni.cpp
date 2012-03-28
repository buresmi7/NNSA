
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

double nahodneCislo(double fMin, double fMax){
	double f;
	f = (double)rand() / RAND_MAX;		
	return fMin + f * (fMax - fMin);
}
class ControllerClovek : public Controller{	
	sf::RenderWindow *App;
	int citac;
	int buffer;
public:
	ControllerClovek(Objekt *l, sf::RenderWindow *App) : Controller(l){
		this->App = App;
		citac = 0;
		buffer = 0;
		this->pocet_zivotu = 10000;
	};
	void provedAkci(){
		const sf::Input& Input = App->GetInput();
		if (Input.IsKeyDown(sf::Key::Left))
			vlevo(-2);
		if (Input.IsKeyDown(sf::Key::Right))
			vpravo(2);
		if (Input.IsKeyDown(sf::Key::Up))
			nahoru(-2);
		if (Input.IsKeyDown(sf::Key::Down))
			dolu(2);
		if(Input.IsKeyDown(sf::Key::Space)){
			if(buffer < 3)
				buffer++;
		}
			
		if(citac == 20){
			if(buffer > 0){
				l->vystrel(this);
				buffer--;
			}
			citac = 0;
		}
		else
			citac++;
	};
};
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
class ControllerFRNNStrilej : public Controller{	
	FRNeuralNetwork *inteligence;
	int citac;
	int buffer;
public:
	ControllerFRNNStrilej (Objekt *l, FRNeuralNetwork *i) : Controller(l){
		inteligence = i;
		citac = 0;
		buffer = 0;
		this->nastavNesmrtelnost();
	};
	void provedAkci(){
		std::vector<double> v = nejblizsiVzdalenostiObjektu(3);
		std::vector<double> vystupy = this->inteligence->update(v);	
				
		l->posunX(vystupy[0]*2);	
		l->posunX(vystupy[1]*(-2));	
		l->posunY(vystupy[2]*2);
		l->posunY(vystupy[3]*(-2));

		if(vystupy[4] < 0.5){
			if(buffer < 3)
				buffer++;
			this->prictiSkore(-1);
		}

		if(citac == 10){
			if(buffer > 0){
				l->vystrel(this);
				buffer--;
			}
			citac = 0;
		}
		else
			citac++;
	};
};
class ControllerShoraDolu : public Controller{	
public:
	ControllerShoraDolu(Objekt *l) : Controller(l){
		this->pocet_zivotu = 5;
	};
	void provedAkci(){
		dolu(1);
	};
};
class ControllerShoraDoluStrilej : public Controller{
	int citac;
public:
	ControllerShoraDoluStrilej(Objekt *l) : Controller(l){
		citac = 0;
		this->pocet_zivotu = 3;
	};
	void provedAkci(){
		dolu(1);
		if(citac == 80){
			l->vystrel(this);
			citac = 0;
		}
		else
			citac++;
	};
};
class ControllerZdolaNahoru : public Controller{	
public:
	ControllerZdolaNahoru(Objekt *l) : Controller(l){
		this->pocet_zivotu = 3;
	};
	void provedAkci(){
		nahoru(1);
	};
};
class ControllerZdolaNahoruStrilej : public Controller{
	int citac;
public:
	ControllerZdolaNahoruStrilej(Objekt *l) : Controller(l){
		citac = 0;
		this->pocet_zivotu = 1;
	};
	void provedAkci(){
		nahoru(1);
		if(citac == 80){
			l->vystrel(this);
			citac = 0;
		}
		else
			citac++;
	};
};

// funkce ohodnoceni, dulezita pro vybirani kvality jedincu	
int ohodnoceni2(FRNeuralNetwork *f){	
	srand(0);
	Space space(400,300);	
	Skudce *l = new Skudce(&space, 175, 50);
	l->nastavPocitaniKolizi();
	ControllerFRNNStrilej *c = new ControllerFRNNStrilej(l, f);			
	space.addController(c);

	int citac = 0;
	for(int j = 0; j < 1000; j++){
		if(citac%85 == 0){
			space.addController(new ControllerZdolaNahoruStrilej(new Lod(&space, nahodneCislo(0,350), 300)));
		}
		if(citac%105 == 0){
			space.addController(new ControllerZdolaNahoruStrilej(new Lod(&space, nahodneCislo(0,350), 300)));
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
	Skudce *l = new Skudce(&space, 175, 50);
	l->nastavPocitaniKolizi();
	Controller *c = new ControllerFRNNStrilej(l, f);			
	space.addController(c);

	int citac = 0;
	for(int j = 0; j < 700; j++){
		if(citac == 0){
			space.addController(new ControllerZdolaNahoru(new Lod(&space, 0, 300)));
			space.addController(new ControllerZdolaNahoru(new Lod(&space, 150, 300)));
			space.addController(new ControllerZdolaNahoru(new Lod(&space, 300, 300)));
		}
		if(citac == 200){			
			space.addController(new ControllerZdolaNahoru(new Lod(&space, 75, 300)));
			space.addController(new ControllerZdolaNahoru(new Lod(&space, 225, 300)));
			space.addController(new ControllerZdolaNahoru(new Lod(&space, 350, 300)));
		}

		space.ProvedKolo();	

		if(citac == 400)
			citac = 0;
		else
			citac++;
	}
	int skore = c->getSkore();
	return skore;
}

int ohodnoceni3(FRNeuralNetwork *f){	
	Space space(400,300);	
	Skudce *l = new Skudce(&space, 175, 50);
	l->nastavPocitaniKolizi();
	ControllerFRNNStrilej *c = new ControllerFRNNStrilej(l, f);			
	space.addController(c);

	int citac = 0;
	for(int j = 0; j < 1000; j++){
		if(citac == 0){
			space.addController(new ControllerZdolaNahoruStrilej(new Lod(&space, 0, 300)));
			space.addController(new ControllerZdolaNahoruStrilej(new Lod(&space, 150, 300)));
			space.addController(new ControllerZdolaNahoruStrilej(new Lod(&space, 300, 300)));
		}
		if(citac == 200){			
			space.addController(new ControllerZdolaNahoruStrilej(new Lod(&space, 75, 300)));
			space.addController(new ControllerZdolaNahoruStrilej(new Lod(&space, 225, 300)));
			space.addController(new ControllerZdolaNahoruStrilej(new Lod(&space, 350, 300)));
		}

		space.ProvedKolo();	

		if(citac == 400)
			citac = 0;
		else
			citac++;
	}
	int skore = c->getSkore();
	return skore;
}


int main(){
	// nekonstantni nastaveni generatoru nahodnych cisel
	srand((unsigned int)time(0));
	// server cast
	clock_t t1, t2;
	t1 = clock();	

	int NP = 20;
	double F = 0.7; 
	double CR = 0.8; 
	int Generations = 10; 
	int pocet_neuronu = 5; 

	DiferencialniEvoluce *d1 = new DiferencialniEvoluce(NP, F, CR, Generations, pocet_neuronu, &ohodnoceni2);
	d1->UlozNejlepsiJedinec("nejlepsi_jedinec.txt");

	
	//std::vector<std::pair <int, FRNeuralNetwork *> > populace = d1->getPopulace();
	//DiferencialniEvoluce *d2 = new DiferencialniEvoluce(NP, F, CR, 20, pocet_neuronu, &ohodnoceni2, &populace);
	//d2->UlozNejlepsiJedinec("nejlepsi_jedinec.txt");


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
	FRNeuralNetwork f1("nejlepsi_jedinec.txt");
	//FRNeuralNetwork f2("nejlepsi_jedinec1.txt");

	Skudce j(&space, 175, 50);
	j.nastavPocitaniKolizi();
	Skudce k(&space, 200, 50);
	k.nastavPocitaniKolizi();

	Lod l(&space, 175, 150);
	l.nastavPocitaniKolizi();

	//ControllerClovek *c = new ControllerClovek(&l, &App);	
	//Controller *c = new ControllerFRNNStrilej(&j, &f1);
	//ControllerFRNN *e = new ControllerFRNN(&k, &f2);
	ControllerFRNNStrilej *c = new ControllerFRNNStrilej(&j, &f1);
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
			std::cout << "\rKolo: " << kola << " Hodnoceni: " << c->getSkore() << "\n";

			if(citac%85 == 0){
				space.addController(new ControllerZdolaNahoruStrilej(new Lod(&space, nahodneCislo(0,350), 300)));
			}
			if(citac%105 == 0){
				space.addController(new ControllerZdolaNahoruStrilej(new Lod(&space, nahodneCislo(0,350), 300)));
			}
			/*if(citac == 0){
				space.addController(new ControllerShoraDoluStrilej(new Skudce(&space, 0, 0)));
				space.addController(new ControllerShoraDoluStrilej(new Skudce(&space, 150, 0)));
				space.addController(new ControllerShoraDoluStrilej(new Skudce(&space, 300, 0)));
			}
			if(citac == 100){			
				space.addController(new ControllerShoraDoluStrilej(new Skudce(&space, 75, 0)));
				space.addController(new ControllerShoraDoluStrilej(new Skudce(&space, 225, 0)));
				space.addController(new ControllerShoraDoluStrilej(new Skudce(&space, 350, 0)));
			}*/
			/*if(citac == 0){
				space.addController(new ControllerZdolaNahoru(new Lod(&space, 0, 300)));
				space.addController(new ControllerZdolaNahoru(new Lod(&space, 150, 300)));
				space.addController(new ControllerZdolaNahoru(new Lod(&space, 300, 300)));
			}
			if(citac == 100){			
				space.addController(new ControllerZdolaNahoru(new Lod(&space, 75, 300)));
				space.addController(new ControllerZdolaNahoru(new Lod(&space, 225, 300)));
				space.addController(new ControllerZdolaNahoru(new Lod(&space, 350, 300)));
			}*/
			/*if(citac%55 == 0){
				space.addController(new ControllerShoraDolu(new Skudce(&space, nahodneCislo(0,350), 0)));
			}
			if(citac%85 == 0){
				space.addController(new ControllerShoraDolu(new Skudce(&space, nahodneCislo(0,350), 0)));
			}
			if(citac%105 == 0){
				space.addController(new ControllerShoraDolu(new Skudce(&space, nahodneCislo(0,350), 0)));
			}*/

			space.ProvedKolo();	

			if(citac == 200)
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
