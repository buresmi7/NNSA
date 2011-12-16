#ifndef DE
#define DE

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include <vector>
#include <iostream>
#include <algorithm>
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

	int ohodnoceni(FRNeuralNetwork *f){		
		//sf::RenderWindow Appp(sf::VideoMode(400, 300, 32), "Jedinec");
		//Appp.SetFramerateLimit(160);
		Space s(App);		
		ControllerFRNN *c = new ControllerFRNN(new Lod(App, &s, 200, 150), f);
			
		s.addController(c);
		for(int j = 0; j < 5000; j++){
			s.ProvedKolo();				
		}
		// Start game loop
		/*int j = 0;
		while (true){       
			//Appp.Clear();				
			s.ProvedKolo();
			//s.VykresliVsechny();
			
			//Appp.Display();	
			j++;
			if(j == 5000)break;
		}*/
		return c->getSkore();
	}
public:
	// NP - velikost populace
	DiferencialniEvoluce(int NP, double F, double CR, int Generations, sf::RenderWindow *App){
		this->App = App;			
		//tvorba populace
		std::cout << "tvorba populace\n";
		for(int i = 0; i < NP; i++){
			FRNeuralNetwork *n = new FRNeuralNetwork();
			int o = ohodnoceni(n);
			while(o == 0){				
				n = new FRNeuralNetwork();
				o = ohodnoceni(n);
			}
			//std::cout << "ohodnoceni: " << o << "\n";
			populace.push_back(std::make_pair(o, n));
		}

		for(int k = 0; k < Generations; k++){
			nej = -5000;
			//krizeni
			std::cout << "nova populace " << k+1 << "\n";
			for(int i = 0; i < NP; i++){
				//zvoleni 3 nahodnych jedincu
				FRNeuralNetwork *p = new FRNeuralNetwork();
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
				std::vector<double> dif_vah_vektor;
				for(int j = 0; j < prvni->getVahy().size(); j++){
					dif_vah_vektor.push_back(prvni->getVahy()[j] - druhy->getVahy()[j]);
				}
				//std::transform(prvni->getVahy().begin(), prvni->getVahy().end(), druhy->getVahy().begin(), dif_vah_vektor.begin(), std::minus<double>());
			
				for(int j = 0; j < dif_vah_vektor.size(); j++){
					dif_vah_vektor[j] *= F;
				}

				//vypocet sumoveho vektoru
				std::vector<double> sum_vektor(dif_vah_vektor.size());
				std::transform(dif_vah_vektor.begin(), dif_vah_vektor.end(), treti->getVahy().begin(), sum_vektor.begin(), std::plus<double>());

				//vypocet zkusebniho vektoru
				std::vector<double> zkusebni_vektor;
				for(int j = 0; j < sum_vektor.size(); j++){
					int nahodne = p->RandomUniform(0, 1);
					if(nahodne < CR)
						zkusebni_vektor.push_back(sum_vektor[j]);
					else
						zkusebni_vektor.push_back(populace[i].second->getVahy()[j]);
				}

				FRNeuralNetwork *n = new FRNeuralNetwork();
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
			/*std::cout << "\nstara populace:\n"; 
			for(int j = 0; j < populace.size(); j++){
				std::cout << populace[j].first << " ";
			}
			std::cout << "\nnova populace:\n"; 
			for(int j = 0; j < populace.size(); j++){
				std::cout << nova_populace[j].first << " ";
			}*/
			for(int j = 0; j < populace.size(); j++){
				if(populace[j] == nova_populace[j])
					continue;
				else{
					delete populace[j].second;
					populace[j] = nova_populace[j];
				}
			}
			int p = 0;
			//std::cout << "\nvysledna populace:\n"; 
			for(int j = 0; j < populace.size(); j++){
				//std::cout << populace[j].first << " ";
				p += populace[j].first;
			}
			std::cout << "nejlepsi jedinec: " << nej;
			std::cout << "\nprumer vysledne populace: " << p/NP;

			std::cout << "\n";
			nova_populace.clear();
		}
	}
	FRNeuralNetwork* getBest(){
		return best;
	}
};


#endif

