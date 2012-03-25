#ifndef FRNN
#define FRNN

#include <vector>
#include <deque>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>

using namespace std;

struct FRNeuralNetwork{   
	int pocet_neuronu;
	int delka_genomu;
	unsigned int pocet_vstupu;
	int pocet_vystupu;
	int delka_jednoho_neuronu;
	std::vector<double> vahy;

	std::vector<double> pre_vystupy;

	double nahodneCislo(double fMin, double fMax){
		double f;
		f = (double)rand() / RAND_MAX;		
		return fMin + f * (fMax - fMin);
	}	
	double Sigmoid(double x){
		static float max=50;
		if (x > max)
			return  1;
		if (x <  -max)
			return  0;
		return 1 / (1 + exp(-x*1));
	}
	double Sum(int offset, std::vector<double> vystupy, std::vector<double> vstupy){
		double sum = 0;
		for(int i = 0; i < pocet_neuronu; i++){
			sum += vahy[i + offset * delka_jednoho_neuronu] * vystupy[i];
		}

		sum += vahy[pocet_neuronu + offset * delka_jednoho_neuronu] * 1;

		int j = 0;
		for(int i = pocet_neuronu + 1; i < delka_jednoho_neuronu; i++){
			sum += vahy[i + offset * delka_jednoho_neuronu] * vstupy[j];
			j++;
		}
		return sum;
	}

public:
	FRNeuralNetwork(int pocet_neuronu){
		//predchozi vystup
		pre_vystupy = std::vector<double>(pocet_neuronu, 0);

		pocet_vystupu = 5;
		pocet_vstupu = 9;

		if(pocet_neuronu < pocet_vystupu){
			std::cout << "chyba!!! pocet neuronu je mensi nez pocet vystupu" << std::endl;
			exit(1);
		}
		this->pocet_neuronu = pocet_neuronu;
		this->pocet_vstupu = pocet_vstupu;		
		delka_jednoho_neuronu = pocet_neuronu + 1 + pocet_vstupu;
		delka_genomu = delka_jednoho_neuronu * pocet_neuronu;
		//nahodne naplneni vsech genomu;		
		for(int i = 0; i < delka_genomu; i++){
			vahy.push_back(nahodneCislo(-1, 1));
		}
	}
	int getDelkaGenomu(){
		return delka_genomu;
	}
	void nastavVahy(std::vector<double> v){
		vahy = v;
	}
	std::vector<double> getVahy(){
		return vahy;
	}
	std::vector<double> update(std::vector<double> vstupy, bool nulovat_predchozi_vystup = false;){
		// vystupy je mysleno vahy mezi neurony
		std::vector<double> vystupy(pocet_neuronu);
		if(nulovat_predchozi_vystup){
			for(int i = 0; i < pre_vystupy.size(); i++){
				pre_vystupy[i] = 0;
			}
		}
		// kontrola delky vstupu
		if(vstupy.size() != pocet_vstupu){
			std::cout << "chyba!!! pocet vstupu nesouhlasi" << std::endl;
			exit(1);
		}	

		// vypocetni cast pro kazdy neuron - cas t-1
		for(int i = 0; i < pocet_neuronu; i++){
			double s = Sum(i, (pre_vystupy), vstupy);
			vystupy[i] = Sigmoid(s);
		}
		
		// vypocetni cast pro kazdy neuron - cas t
		for(int i = 0; i < pocet_neuronu; i++){
			double s = Sum(i, vystupy, vstupy);
			pre_vystupy[i] = Sigmoid(s);
		}	

		return pre_vystupy;
	}

};
#endif

