#ifndef FRNN
#define FRNN

#include <vector>
#include <iostream>
#include <cstdlib>
#include <ctime>

/*------Constants for RandomUniform()---------------------------------------*/
#define SEED 3
#define IM1 2147483563
#define IM2 2147483399
#define AM (1.0/IM1)
#define IMM1 (IM1-1)
#define IA1 40014
#define IA2 40692
#define IQ1 53668
#define IQ2 52774
#define IR1 12211
#define IR2 3791
#define NTAB 32
#define NDIV (1+IMM1/NTAB)
#define EPS 1.2e-7
#define RNMX (1.0-EPS)

struct FRNeuralNetwork{   
   int pocet_neuronu;
   int delka_genomu;
   unsigned int pocet_vstupu;
   int pocet_vystupu;
   int delka_jednoho_neuronu;
   std::vector<double> vahy;

	double nahodneCislo(double fMin, double fMax){
		double f;
		f = (double)rand() / RAND_MAX;		
		return fMin + f * (fMax - fMin);
	}	
	double Sigmoid(double x){
		return 1 / (1 + exp(-x*100));
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
		pocet_vystupu = 2;
		if(pocet_neuronu < pocet_vystupu){
			std::cout << "chyba!!! pocet neuronu je mensi nez pocet vystupu" << std::endl;
			exit(1);
		}
		this->pocet_neuronu = pocet_neuronu;
		pocet_vstupu = 4;		
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
	std::vector<double> update(std::vector<double> vstupy){
		// vystupy je mysleno vahy mezi neurony
		std::vector<double> vystupy(pocet_neuronu);
		std::vector<double> pre_vystupy(pocet_neuronu, 0);
		// kontrola delky vstupu
		if(vstupy.size() != pocet_vstupu){
			std::cout << "chyba!!! pocet vstupu nesouhlasi" << std::endl;
			exit(1);
		}
		// vypocetni cast pro kazdy neuron - cas t
		for(int i = 0; i < pocet_neuronu; i++){
			double s = Sum(i, pre_vystupy, vstupy);
			vystupy[i] = Sigmoid(s);
		}
		for(int i = 0; i < pocet_neuronu; i++){
			pre_vystupy[i] = vystupy[i];
		}
		// vypocetni cast pro kazdy neuron - cas t+1
		for(int i = 0; i < pocet_neuronu; i++){
			double s = Sum(i, pre_vystupy, vstupy);
			vystupy[i] = Sigmoid(s);
		}			

		return vystupy;
	}

};
#endif

