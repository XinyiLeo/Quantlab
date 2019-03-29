#include <iostream>
#include <fstream>
#include <cstring>
#include <sstream>
using namespace std;

const int MAXELTS = 10000;
class Stocks {
	public:
		char name[4];
		unsigned long long int LastTimeStamp;
		unsigned long long int MaxTradingGap;
		unsigned long long int Volume;
		unsigned int MaxTradePrice;
		unsigned int AvgPrice;
		bool operator==(const Stocks& rhs) const { return (strcmp(name, rhs.name) == 0); };
		bool operator< (const Stocks& rhs) const { return (strcmp(name, rhs.name) < 0); };
		bool operator> (const Stocks& rhs) const { return (strcmp(name, rhs.name) > 0); };
};

template <class T>
class StockSet {		
		int numElts;
	public:
		T elts[MAXELTS];
		StockSet();
		void insert(T v);
		int indexOf(T v) const;
		bool query(T v) const;
		int size() const;
		void display();
};

template <class T>
StockSet<T>::StockSet():numElts(0) {}

template <class T>
int StockSet<T>::indexOf(T v) const { // Binary Search
	int left = 0;
	int right = numElts-1;
	while (right >= left) {
		int middle = left + (right - left + 1)/2;
		if (elts[middle] == v)
			return middle;
		else if (elts[middle] < v)
			left = middle+1;
		else
			right = middle-1;
	}
	return MAXELTS;
}

template <class T>
bool StockSet<T>::query(T v) const { 
	return (indexOf(v) != MAXELTS);
}

template <class T>
void StockSet<T>::insert(T v) { // Sorted
	if (!query(v)) {
		if (numElts == MAXELTS) throw MAXELTS; 
		int cand = numElts-1; 
		while ((cand >= 0) && elts[cand] > v) {
			elts[cand+1] = elts[cand];
			cand--;
		}
		elts[cand+1] = v;
		numElts++; 
	}
}

template <class T>
int StockSet<T>::size() const {
	return numElts;
}

template <class T>
void StockSet<T>::display() {
	// <symbol>,<MaxTimeGap>,<Volume>,<WeightedAveragePrice>,<MaxPrice>
	ofstream oFile ("output.csv");
	ostringstream oStream;
	for( int i = 0; i < size(); i++ ) {
		oStream << elts[i].name << ',' << elts[i].MaxTradingGap << ',' << 
			elts[i].Volume << ',' << elts[i].MaxTradePrice << ',' << elts[i].AvgPrice << '\n';
	}
	oFile << oStream.str();
	oFile.close();
}

int main() {
	string line;
	ifstream iFile;
	iFile.open("input.csv");
	istringstream iStream;
	unsigned long long int TimeStamp;
	char Symbol[4];
	unsigned int Quantity, Price;
	char comma;
	unsigned long long int A,B,E;	
	int C, D;

	StockSet<Stocks> a;
	while(getline(iFile, line)) {
		iStream.str(line);
		Stocks test;
		// Read to variables
		iStream >> TimeStamp;
		iStream.ignore();
		iStream.get(Symbol, 4);
		iStream.ignore();
		iStream >> Quantity >> comma >> Price;
		// Put into data structure
		strcpy(test.name, Symbol);
		if(!a.query(test)) {
			Stocks ipt;
			strcpy(ipt.name, Symbol);
			ipt.LastTimeStamp = TimeStamp;
			ipt.MaxTradingGap = 0;
			ipt.Volume = Quantity;
			ipt.MaxTradePrice = Price;
			ipt.AvgPrice = Price;
			a.insert(ipt);
		}
		else {
			A = TimeStamp - a.elts[a.indexOf(test)].LastTimeStamp;
			B = a.elts[a.indexOf(test)].MaxTradingGap;
			a.elts[a.indexOf(test)].MaxTradingGap = A > B ? A : B;
			E = a.elts[a.indexOf(test)].Volume;
			a.elts[a.indexOf(test)].Volume += Quantity;
			C = a.elts[a.indexOf(test)].MaxTradePrice;
			a.elts[a.indexOf(test)].MaxTradePrice = Price > C ? Price : C;
			D = a.elts[a.indexOf(test)].AvgPrice;
			a.elts[a.indexOf(test)].AvgPrice = (D * E + Quantity * Price) / (E + Quantity);
		}
	}
	a.display();
	iFile.close();
	return 0;
}























