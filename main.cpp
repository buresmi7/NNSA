#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>

#include "space.h"
#include "de.h"

int main()
{
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

	int NP; double F; double CR; int Generations;
	std::cout << "zadej velikost populace (60): ";
	std::cin >> NP;
	std::cout << "zadej mutacni konstantu (0.2): ";
	std::cin >> F;
	std::cout << "zadej prah krizeni (0.7): ";
	std::cin >> CR;
	std::cout << "zadej pocet generaci (?): ";
	std::cin >> Generations;

	DiferencialniEvoluce d(NP, F, CR, Generations, &App);

	std::cout << "genom: ";
	for(int i = 0; i < d.getBest()->getDelkaGenomu(); i++){
		std::cout << d.getBest()->getVahy()[i] << " ";
	}
	std::cout << "\n";

	ControllerFRNN *c = new ControllerFRNN(new Lod(&App, &space, 200, 150), d.getBest());
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

    return EXIT_SUCCESS;
}