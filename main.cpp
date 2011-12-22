#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <fstream>
#include <time.h>

#include "anyoption.h"
#include "space.h"
#include "de.h"

using namespace std;

int generator(int NP, double F, double CR, int Generations, int pocet, char* soubor){
	cout << "Parametry diferencialni evoluce" << endl;
	cout << "NP = " << NP << endl;
	cout << "F = " << F  << endl;
	cout << "CR = " << CR << endl;
	cout << "Generations = " << Generations << endl;
	DiferencialniEvoluce d(NP, F, CR, Generations, pocet, NULL);
	//zapis do souboru
	ofstream myfile;
	myfile.open(soubor);	
	for(int i = 0; i < d.getBest()->getDelkaGenomu(); i++){
		myfile << d.getBest()->getVahy()[i] << "\n";
	}
	myfile << "\n";
	myfile.close();
	cout << "nejlepsi jedinec zapsan do souboru " << soubor << endl;
	return 0;
}

int tester(std::vector<double> vahy){
	bool zacni = false;
	bool pauza = false;
	int kola = 0;

	// nacteni obrazku - inicializace v objektech
	Lod *a = new Lod();
	std::cout << "joj" << std::endl;
	a->Init("raketa.tga");
	std::cout << "joj" << std::endl;
	Skudce *b = new Skudce();
	b->Init("raketa2.tga");

    // Create the main rendering window
    sf::RenderWindow App(sf::VideoMode(400, 300, 32), "SFML Graphics");
	App.SetFramerateLimit(40);

	App.UseVerticalSync(true);

	// nastaveni pisma
	sf::Font MyFont;
	MyFont.LoadFromFile("MyriadWebPro-Bold.ttf", 50);

	sf::String UvodniText("Stiskni Enter...", MyFont, 20);
	sf::String DalsiText("pro pauzu stiskni P", MyFont, 10);
	sf::String PauzaText("pro pokracovani stiskni P", MyFont, 10);
	UvodniText.SetPosition(20, 150);
	DalsiText.SetPosition(20, 170);
	PauzaText.SetPosition(20, 170);

	Space space(&App);		
	
	FRNeuralNetwork f;
	f.nastavVahy(vahy);

	ControllerFRNN *c = new ControllerFRNN(new Lod(&App, &space, 200, 150), &f);
	//ControllerFRNN *c = new ControllerFRNN(new Lod(&App, &space, 200, 150), new FRNeuralNetwork());
	//ControllerClovek *c = new ControllerClovek(new Lod(&App, &space, 200, 150));
	space.addController(c);

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
			space.ProvedKolo();
			space.VykresliVsechny();
			//std::cout<< "kolo: " << ++kola;
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
int main(int argc, char **argv){
	clock_t t1, t2;
	t1 = clock();
	// nekonstantni nastaveni generatoru nahodnych cisel
	srand(time(0));	

	// parsovani parametru
	AnyOption *opt = new AnyOption();
	opt->setVerbose(); /* print warnings about unknown options */
    opt->autoUsagePrint(true); /* print usage for bad options */

	opt->addUsage( "" );
	opt->addUsage( "NNSA (Neural Network Space Arcade) - Testovani rekuretni neuronove site na jednoduche arkade" );
    opt->addUsage( "" );
	opt->addUsage( "Pouziti: " );
    opt->addUsage( "" );
    opt->addUsage( " -h   --help	Zobrazi tuto napovedu " );    
	opt->addUsage( " -t input.txt	Testovani neuronove site " ); 
	opt->addUsage( " -g output.txt	Generator neuronove site " ); 
	opt->addUsage( " " );
	opt->addUsage( "      --NP 60	Velikost populace " );
	opt->addUsage( "      --F 0.3	Mutacni konstanta " );
	opt->addUsage( "      --CR 0.9	Prah krizeni " );
	opt->addUsage( "      --generations 100	Pocet kol slechteni populace " );
	opt->addUsage( "      --pocet 5000	Pocet kol ohodnocovaci funkce " );
    opt->addUsage( "" );

	opt->setFlag( "help", 'h');   /* a flag (takes no argument), supporting long and short form */
	opt->setOption('g');
	opt->setOption('t');

	opt->setOption("NP");
	opt->setOption("F");
	opt->setOption("CR");
	opt->setOption("generations");
	opt->setOption("pocet");

	opt->processCommandArgs(argc, argv);

	if( ! opt->hasOptions()) { /* print usage if no options */
        opt->printUsage();
	    delete opt;
		return 0;
	}

	if( opt->getFlag( "help" ) || opt->getFlag( 'h' ) )
		opt->printUsage();
	if(opt->getValue('g') != NULL ) {
		// kontrola parametru testeru
		if(opt->getValue("NP") == NULL || opt->getValue("F") == NULL || opt->getValue("CR") == NULL || opt->getValue("generations") == NULL || opt->getValue("pocet") == NULL){
			cout << "Chybny parametr generatoru" << endl;
			opt->printUsage(); 
			delete opt;
			return 0;
		}
		int NP = atoi(opt->getValue("NP"));
		double F = atof(opt->getValue("F"));
		double CR = atof(opt->getValue("CR")); 
		int Generations = atoi(opt->getValue("generations"));
		int pocet = atoi(opt->getValue("pocet"));
		cout << "Generator neuronove site" << endl;
		generator(NP, F, CR, Generations, pocet, opt->getValue('g'));
	}
	else if(opt->getValue('t') != NULL ) {			
		// nacteni vstupniho souboru
		std::vector<double> vahy;		
		double s;
		ifstream infile;
		infile.open (opt->getValue('t'));
		while(!infile.eof()){
			infile >> s;
			vahy.push_back(s);
			//cout << s << endl;
		}
		infile.close();				
		cout << "Testovani neuronove site" << endl;
		tester(vahy);
	}	
	delete opt;	

	// 32bit - pretece cca po 36 minutach
	t2 = clock();
	cout << "Doba behu programu: " << ((double) (t2 - t1))/CLOCKS_PER_SEC << endl;
    return 0;
}