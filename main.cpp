#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <fstream>

#include "anyoption.h"
#include "space.h"
#include "de.h"

using namespace std;

int generator(int NP, double F, double CR, int Generations){
	DiferencialniEvoluce d(NP, F, CR, Generations, NULL);
	std::cout << "genom:\n";
	for(int i = 0; i < d.getBest()->getDelkaGenomu(); i++){
		std::cout << d.getBest()->getVahy()[i] << "\n";
	}
	std::cout << "\n";

	return 0;
}

int tester(std::vector<double> vahy){
	bool zacni = false;
	bool pauza = false;
	int kola = 0;

	srand((unsigned int)time(0));

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
	opt->addUsage( " -t   --tester input.txt	Testovani neuronove site " ); 
	opt->addUsage( " -g   --generator	Generator neuronove site " ); 
	opt->addUsage( " " );
	opt->addUsage( "      --velikost-populace 60 " );
	opt->addUsage( "      --mutacni-konstanta 0.3 " );
	opt->addUsage( "      --prah-krizeni 0.9 " );
	opt->addUsage( "      --pocet-generaci 100 " );
    opt->addUsage( "" );

	opt->setFlag( "help", 'h');   /* a flag (takes no argument), supporting long and short form */
	opt->setFlag( "generator", 'g');
	opt->setOption( "tester", 't');

	opt->setOption( "velikost-populace");
	opt->setOption( "mutacni-konstanta");
	opt->setOption( "prah-krizeni");
	opt->setOption( "pocet-generaci");

	opt->processCommandArgs(argc, argv);

	if( ! opt->hasOptions()) { /* print usage if no options */
        opt->printUsage();
	    delete opt;
		return 0;
	}

	if( opt->getFlag( "help" ) || opt->getFlag( 'h' ) )
		opt->printUsage();
	if( opt->getFlag( "generator" ) || opt->getFlag( 'g' ) ) {
		// kontrola parametru testeru
		if(opt->getValue("velikost-populace") == NULL || opt->getValue("mutacni-konstanta") == NULL || opt->getValue("prah-krizeni") == NULL || opt->getValue("pocet-generaci") == NULL){
			cout << "Chybny parametr generatoru" << endl;
			opt->printUsage(); 
			delete opt;
			return 0;
		}
		int NP = atoi(opt->getValue("velikost-populace"));
		double F = atoi(opt->getValue("mutacni-konstanta"));
		double CR = atoi(opt->getValue("prah-krizeni")); 
		int Generations = atoi(opt->getValue("pocet-generaci"));
		cout << "Generator neuronove site" << endl;
		generator(NP, F, CR, Generations);
	}
	else if( opt->getValue( "tester" ) != NULL || opt->getValue( "t" ) != NULL ) {			
		// nacteni vstupniho souboru
		std::vector<double> vahy;		
		double s;
		ifstream infile;
		infile.open (opt->getValue( "tester" ));
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

    return 0;
}