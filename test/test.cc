#include <iostream>
#include <Simulator.h>

#include "VirtualSequence.h"
#include "Reference.h"
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
	
	
	// Prueba de correctitud de VirtualSequence
	Reference *ref;
	VirtualSequence *seq;
	string s1, s2;
//	string ref_text = "AACACGTTTAACACGTTTAACACGTTT";
	string ref_text = "AAAAAAAA";
	uniform_int_distribution<> dist(0, 100);
	
	// Constructor con texto de referencia
	ref = new Reference(ref_text);
	seq = new VirtualSequence(ref_text);
	s1 = ref->to_string();
	s2 = seq->to_string();
	cout<<"s1: "<<s1<<"\n";
	cout<<"s2: "<<s2<<"\n";
	if( s1.compare(s2) != 0 ){
		cout<<"Error texto\n";
	}
//	for(unsigned int i = 0; i < 10; ++i){
//		ref->mutate();
//		seq->mutate();
//		s1 = ref->to_string();
//		s2 = seq->to_string();
//		if( s1.compare(s2) != 0 ){
//			cout<<"Error texto m "<<i<<"\n";
//		}
//	}
	rng.seed(0);
	cout<<"rng: "<<(dist(rng))<<", "<<(dist(rng))<<", "<<(dist(rng))<<", "<<(dist(rng))<<", "<<(dist(rng))<<"\n";
	for(unsigned int i = 0; i < 5; ++i){
		ref->mutate();
		s1 = ref->to_string();
		cout<<"s1: "<<s1<<"\n";
	}
	cout<<"-----\n";
	rng.seed(0);
	cout<<"rng: "<<(dist(rng))<<", "<<(dist(rng))<<", "<<(dist(rng))<<", "<<(dist(rng))<<", "<<(dist(rng))<<"\n";
	for(unsigned int i = 0; i < 5; ++i){
		seq->mutate();
		s2 = seq->to_string();
		cout<<"s2: "<<s2<<"\n";
	}
	cout<<"-----\n";
	delete ref;
	delete seq;
	
	// Constructor con int
	ref = new Reference(20, 123456);
	seq = new VirtualSequence(20, 123456);
	s1 = ref->to_string();
	s2 = seq->to_string();
	if( s1.compare(s2) != 0 ){
		cout<<"Error int\n";
	}
	delete ref;
	delete seq;
	
	// Constructor de copia
	
	
	
	
	
	
	/*
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
	*/
	
	
}
