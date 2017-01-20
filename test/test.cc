#include <iostream>
#include <Simulator.h>

#include "VirtualSequence.h"
#include "Bitset.h"

using namespace std;
random_device seed;
mt19937 rng(seed());

int main(int argc,char** argv)
{
	
	// Pruebas preliminares de datos 
	/*
	cout<<"Test - Pruebas preliminares de datos\n";
	
	VirtualSequence v_seq("AACACGTTT", 9);
	for(unsigned int i = 0; i < 12; ++i){
		cout<<"v_seq["<<i<<"]: "<<v_seq.at(i)<<"\n";
	}
	
	Bitset seq(14, 2572888);
	string seq_str = seq.to_string();
	cout<<"Test - Bitset(14, 2572888): "<<seq_str<<"\n";
	
	VirtualSequence v_seq2(14, 2572888);
	string seq2_str = v_seq2.to_string();
	cout<<"Test - VirtualSequence(14, 2572888): "<<seq2_str<<"\n";
	
	cout<<"Test - Pruebas preliminares terminadas\n-----  -----\n";
	*/
	
	cout<<"Test - Inicio\n";
	
	boost::property_tree::ptree fsettings;
	cout<<"Test - read_json...\n";
	read_json(argv[1],fsettings);

	cout<<"Test - new Simulator...\n";
	Simulator *sim = new Simulator(fsettings);
	cout<<"Test - sim->run...\n";
	sim->run();

	cout<<"Test - sim->populations...\n";
	for(auto p : sim->populations()){
		boost::property_tree::ptree findices=p->indices(1.0);
		stringstream ss;
		write_json(ss,findices);
		cout << ss.str() << endl;
	}

	cout<<"Test - delete sim...\n";
	delete sim;
	
	cout<<"Test - Fin (const_str: "<<Bitset::count_str<<", const_int: "<<Bitset::count_int<<", const_copy: "<<Bitset::count_copy<<", const_del: "<<Bitset::count_del<<", const_mut: "<<Bitset::count_mut<<")\n";
	return(0);
}
