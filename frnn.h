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
		double f = (double)rand() / RAND_MAX;
		return fMin + f * (fMax - fMin);
	}	
	double Sigmoid(double x){
		return 1 / (1 + exp(-x));
	}
	double Sum(int offset, std::vector<double> vystupy, std::vector<double> vstupy){
		double sum = 0;

		for(int i = 0; i < pocet_neuronu; i++){
			sum += vahy[i + offset * delka_jednoho_neuronu] * vystupy[i];
		}

		sum += vahy[pocet_vystupu + offset * delka_jednoho_neuronu] * 1;

		int j = 0;
		for(int i = pocet_vystupu + 1; i < pocet_vystupu + pocet_vystupu + 1; i++){
			sum += vahy[i + offset * delka_jednoho_neuronu] * vstupy[j];
			j++;
		}
		return sum;
	}

public:
	double RandomUniform(double minValue,double maxValue){
		long j;
		long k;
		static long idum;
		static long idum2=123456789;
		static long iy=0;
		static long iv[NTAB];
		double result;

		if (iy == 0)
			idum = SEED;

		if (idum <= 0)
		{
			if (-idum < 1)
				idum = 1;
			else
				idum = -idum;

			idum2 = idum;

			for (j=NTAB+7; j>=0; j--)
			{
				k = idum / IQ1;
				idum = IA1 * (idum - k*IQ1) - k*IR1;
				if (idum < 0) idum += IM1;
				if (j < NTAB) iv[j] = idum;
			}

			iy = iv[0];
		}

		k = idum / IQ1;
		idum = IA1 * (idum - k*IQ1) - k*IR1;

		if (idum < 0)
			idum += IM1;

		k = idum2 / IQ2;
		idum2 = IA2 * (idum2 - k*IQ2) - k*IR2;

		if (idum2 < 0)
			idum2 += IM2;

		j = iy / NDIV;
		iy = iv[j] - idum2;
		iv[j] = idum;

		if (iy < 1)
			iy += IMM1;

		result = AM * iy;

		if (result > RNMX)
			result = RNMX;

		result = minValue + result * (maxValue - minValue);
		//std::cout<<"nahodne cislo: " << result << "\n";
		return(result);
	}

	FRNeuralNetwork(int pocet_neuronu){
		pocet_vystupu = 4;
		if(pocet_neuronu < pocet_vystupu){
			std::cout << "chyba!!! pocet neuronu je mensi nez pocet vystupu" << std::endl;
			exit(1);
		}
		this->pocet_neuronu = pocet_neuronu;
		pocet_vstupu = 4;		
		delka_jednoho_neuronu = pocet_vystupu + pocet_vstupu + 1;
		delka_genomu = delka_jednoho_neuronu * pocet_neuronu;
		//nahodne naplneni vsech genomu;		
		for(int i = 0; i < delka_genomu; i++){
			vahy.push_back(RandomUniform(-1, 1));
		}
		
		/*std::cout << "genom: ";
		for(int i = 0; i < delka_genomu; i++){
			std::cout << vahy[i] << " ";
		}
		std::cout << "\n";*/
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
		if(vstupy.size() != pocet_vstupu)
			return vystupy;		
		
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

