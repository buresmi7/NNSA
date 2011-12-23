#ifndef DE
#define DE

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include <vector>
#include <iostream>
#include <algorithm>
#include <string>
#include <sstream>
#include "controller.h"
#include "frnn.h"
#include "space.h"

class DiferencialniEvoluce{
	//vektor populace
	std::vector<std::pair <int, FRNeuralNetwork *> > populace;
	std::vector<std::pair <int, FRNeuralNetwork *> > nova_populace;
	sf::RenderWindow *App;
	FRNeuralNetwork* best;	
	int nej;
	int pocet_kroku;

	int ohodnoceni(FRNeuralNetwork *f){			
		Space s(App);	
		Lod *l = new Lod(App, &s, 200, 150);
		l->nastavPocitaniKolizi();
		ControllerFRNN *c = new ControllerFRNN(l, f);			
		s.addController(c);
		for(int j = 0; j < pocet_kroku; j++){
			s.ProvedKolo();				
		}
		delete l;
		return c->getSkore();
	}
public:
	// NP - velikost populace
	DiferencialniEvoluce(int NP, double F, double CR, int Generations, int pocet_kroku, int pocet_neuronu, sf::RenderWindow *App){
		ofstream myfile;
		myfile.open("data.txt");
		
		this->pocet_kroku = pocet_kroku;
		this->App = App;			
		//tvorba populace
		std::cout << "tvorba populace\n";
		for(int i = 0; i < NP; i++){			
			FRNeuralNetwork *n = new FRNeuralNetwork(pocet_neuronu);			
			int o = ohodnoceni(n);			
			populace.push_back(std::make_pair(o, n));
		}

		for(int k = 0; k < Generations; k++){
			nej = -5000;
			//krizeni
			std::cout << "nova populace " << k+1 << "\n";
			for(int i = 0; i < NP; i++){
				//zvoleni 3 nahodnych jedincu
				FRNeuralNetwork *p = new FRNeuralNetwork(pocet_neuronu);
				FRNeuralNetwork *prvni;
				FRNeuralNetwork *druhy;
				FRNeuralNetwork *treti;
				while(true){
					int nahodne = p->RandomUniform(0, NP-1);
					if(nahodne == i)continue;
					prvni = populace[nahodne].second;
					break;
				}
				while(true){
					int nahodne = p->RandomUniform(0, NP-1);
					if(nahodne == i)continue;
					druhy = populace[nahodne].second;
					if(prvni == druhy)continue;
					break;
				}
				while(true){
					int nahodne = p->RandomUniform(0, NP-1);
					if(nahodne == i)continue;
					treti = populace[nahodne].second;
					if(treti == prvni)continue;
					if(treti == druhy)continue;
					break;
				}

				//vypocet diferencniho vahoveho vektoru
				std::vector<double> zkusebni_vektor;
				for(int j = 0; j < prvni->getVahy().size(); j++){
					int nahodne = p->RandomUniform(0, 1);
					if(nahodne < CR)
						zkusebni_vektor.push_back((prvni->getVahy()[j] - druhy->getVahy()[j])*F + treti->getVahy()[j]);
					else
						zkusebni_vektor.push_back(populace[i].second->getVahy()[j]);										
				}				

				FRNeuralNetwork *n = new FRNeuralNetwork(pocet_neuronu);
				n->nastavVahy(zkusebni_vektor);
				int o = ohodnoceni(n);

				//vyber lepsiho jedince do nove populace
				//std::cout << "vybiram mezi jedinci: " << o << " a " << populace[i].first << "\n";		
				if(o >= populace[i].first){
					nova_populace.push_back(std::make_pair(o, n));
					//std::cout << "ohodnoceni viteze: " << o << "\n";
					if(o >= nej){
						best = n;
						nej = o;
					}
									
				}
				else{
					nova_populace.push_back(populace[i]);					
					//std::cout << "ohodnoceni viteze: " << populace[i].first << "\n";
					if(populace[i].first >= nej){
						best = populace[i].second;
						nej = populace[i].first;
					}
				}				
				//std::cout << "nejlepsi: " << nej << "\n";
			}			
			
			for(int j = 0; j < populace.size(); j++){
				if(populace[j] == nova_populace[j])
					continue;
				else{
					delete populace[j].second;
					populace[j] = nova_populace[j];
				}				
			}			

			ostringstream ss;
			ss << "populace " << k+1 << "\t";
			int p = 0;
			//std::cout << "\nvysledna populace:\n"; 
			for(int j = 0; j < populace.size(); j++){
				//std::cout << populace[j].first << " ";
				p += populace[j].first;
				//zapis do souboru
				ss << populace[j].first << "\t";	
			}								
			myfile << ss.str() << endl;

			std::cout << "nejlepsi jedinec: " << nej;
			std::cout << "\nprumer vysledne populace: " << p/NP;

			std::cout << "\n";
			nova_populace.clear();
		}
		myfile.close();	
	}
	FRNeuralNetwork* getBest(){
		return best;
	}
};


#endif

