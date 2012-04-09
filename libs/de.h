#ifndef DE
#define DE

#include <vector>
#include <iostream>
#include <algorithm>
#include <string>
#include <sstream>
#include <fstream>

#include "frnn.h"

using namespace std;

double genrand(int min, int max){
	//srand((unsigned int)time(0));
	return min + (max-min)*(double) rand()/RAND_MAX;;
}

class DiferencialniEvoluce{	
	std::vector<std::pair <int, FRNeuralNetwork *> > populace;// vektor populace
	std::vector<std::pair <int, FRNeuralNetwork *> > nova_populace;
	FRNeuralNetwork* nejlepsi_jedinec;	
	int nejlepsi_ohodnoceni;
	int pocet_neuronu;
	
	// nahodne cislo, neopakujici se (permutace)
	void NahodneCislo(int max, int &prvni, int &druhy, int &treti, int vynech){
		//srand((unsigned int)time(0));
		std::vector<int> elements(max);

		// inizialize
		for (int i = 0; i < (int)elements.size(); ++i){
			elements[i] = i;
		}
		elements.erase(elements.begin()+vynech);

		for (int i = elements.size() - 1; i > 0; --i) {
		  // generate random index
		  int w = rand()%i;
		  // swap items
		  int t = elements[i];
		  elements[i] = elements[w];
		  elements[w] = t;
		}
		prvni = elements[0];
		druhy = elements[1];
		treti = elements[2];
	}
public:
	DiferencialniEvoluce(int NP, double F, double CR, int Generations, int pocet_neuronu, int (*ohodnoceni)(FRNeuralNetwork *f), int tichy = 0, char* soubor = "data.txt", std::vector<std::pair <int, FRNeuralNetwork *> > *pocatecni_populace = NULL){
		this->pocet_neuronu = pocet_neuronu;
		
		ofstream myfile;
		myfile.open(soubor);// otevreni souboru pro zapis informaci o populacich
		
		if(pocatecni_populace == NULL){
			double hotovo = 0;
			int pre = 0;
			int o = 0;
			nejlepsi_ohodnoceni = -50000000;
			std::cout << "Generuji novou populaci 0%";
			//tvorba populace, vytvoreni nahodnych jedincu a vypocet jejich ohodnoceni
			for(int i = 0; i < NP; i++){			
				FRNeuralNetwork *n = new FRNeuralNetwork(pocet_neuronu);	
				o = ohodnoceni(n);				
				populace.push_back(std::make_pair(o, n));

				//zjisteni zda ma lepsi jedinec nejlepsi ohodnoceni
				if(o >= nejlepsi_ohodnoceni){
					nejlepsi_ohodnoceni = o;
					nejlepsi_jedinec = n;					
				}
				//ukecany vypis
				if(tichy == 0){
					hotovo += 100/(double)NP;
					if((int)hotovo%5 == 0)
						if((int)hotovo > pre)
							std::cout << "\rGeneruji novou populaci " << (int)hotovo << "%";
					pre = (int)hotovo;
				}
			}		
			// vypis vysledku populace do souboru
			ostringstream ss;
			ss << "populace " << 0 << "\t";
			int p = 0;			
			for(unsigned int j = 0; j < populace.size(); j++){
				//std::cout << populace[j].first << " ";
				p += populace[j].first;
				//zapis do souboru
				ss << populace[j].first << "\t";	
			}								
			myfile << ss.str() << endl;
			std::cout << "\n -> prumer populace: " << p/NP;
			std::cout << "\n -> nejlepsi jedinec: " << nejlepsi_ohodnoceni;
			std::cout << "\n\n";
		}
		else{
			double hotovo = 0;
			std::cout << "Nahravam novou populaci 0%";
			for(int i = 0; i < NP; i++){
				int o = ohodnoceni((*pocatecni_populace)[i].second);	
				populace.push_back(std::make_pair(o, (*pocatecni_populace)[i].second));
				
				hotovo += 100/(double)NP;				
				std::cout << "\rNahravam novou populaci " << (int)hotovo << "%";
			}
			std::cout << "\n\n";
		}
		//hlavni cyklus
		for(int k = 0; k < Generations; k++){
			nejlepsi_ohodnoceni = -50000000;
			//krizeni
			std::cout << "Vytvarim novou populaci (" << k+1 << ") 0%";
			double hotovo = 0;
			int pre = 0;
			for(int i = 0; i < NP; i++){				
				//zvoleni 3 nahodnych jedincu
				int p, d, t;
				NahodneCislo(NP, p, d, t, i);
				FRNeuralNetwork *prvni = populace[p].second;
				FRNeuralNetwork *druhy = populace[d].second;
				FRNeuralNetwork *treti = populace[t].second;
				
				//vypocet diferencniho vahoveho vektoru
				std::vector<double> zkusebni_vektor;
				for(unsigned int j = 0; j < prvni->getVahy().size(); j++){
					double nahodne = genrand(0, 1);
					if(nahodne < CR)
						zkusebni_vektor.push_back((prvni->getVahy()[j] - druhy->getVahy()[j])*F + treti->getVahy()[j]);
					else
						zkusebni_vektor.push_back(populace[i].second->getVahy()[j]);										
				}				

				FRNeuralNetwork *n = new FRNeuralNetwork(pocet_neuronu);
				n->nastavVahy(zkusebni_vektor);
				int o = ohodnoceni(n);

				//vyber lepsiho jedince do nove populace		
				if(o >= populace[i].first){
					nova_populace.push_back(std::make_pair(o, n));													
				}
				else{
					nova_populace.push_back(populace[i]);					
				}	

				//zjisteni zda ma lepsi jedinec nejlepsi ohodnoceni
				if(nova_populace[i].first >= nejlepsi_ohodnoceni){
					nejlepsi_ohodnoceni = nova_populace[i].first;
					nejlepsi_jedinec = nova_populace[i].second;					
				}
				//ukecany vypis
				if(tichy == 0){
					hotovo += 100/(double)NP;	
					if((int)hotovo%5 == 0)
						if((int)hotovo > pre)
							std::cout << "\rVytvarim novou populaci (" << k+1 << ") "<< (int)hotovo << "%";
					pre = (int)hotovo;
				}
			}			
			
			// kopirovani nove populace
			for(unsigned int j = 0; j < populace.size(); j++){
				if(populace[j] == nova_populace[j])
					continue;
				else{
					delete populace[j].second;
					populace[j] = nova_populace[j];
				}				
			}	
			nova_populace.clear();

			// vypis vysledku populace do souboru
			ostringstream ss;
			ss << "populace " << k+1 << "\t";
			int p = 0;			
			for(unsigned int j = 0; j < populace.size(); j++){
				//std::cout << populace[j].first << " ";
				p += populace[j].first;
				//zapis do souboru
				ss << populace[j].first << "\t";	
			}								
			myfile << ss.str() << endl;
			// vypis vysledku populace na konzoli			
			std::cout << "\n -> prumer populace: " << p/NP;
			std::cout << "\n -> nejlepsi jedinec: " << nejlepsi_ohodnoceni;
			std::cout << "\n\n";
			
		}
		myfile.close();	

	}
	FRNeuralNetwork* getBest(){
		return nejlepsi_jedinec;
	}
	std::vector<std::pair <int, FRNeuralNetwork *> > getPopulace(){
		return populace;
	}
	void UlozNejlepsiJedinec(char *soubor){
		//zapis do souboru
		ofstream myfile;
		myfile.open(soubor);	
		myfile << pocet_neuronu << "\n";
		for(int i = 0; i < this->getBest()->getDelkaGenomu(); i++){
			myfile << this->getBest()->getVahy()[i] << "\n";
		}
		//myfile << "\n";
		myfile.close();
		cout << "Nejlepsi jedinec zapsan do souboru " << soubor << " ." << endl;
	}
};


#endif

