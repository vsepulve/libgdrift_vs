#ifndef _VIRTUAL_SEQUENCE_H_
#define _VIRTUAL_SEQUENCE_H_

#include <iostream>
#include <string>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <sstream>
#include <algorithm>
#include <limits.h>

#include <map>
#include <random>

using namespace std;


// #define seq_size_t unsigned long long
// #define seq_size_t unsigned int
#define seq_size_t unsigned short

// Tercera Version, texto en 2 bits por base
// En esta version, el constructor de copia ahorra espacio (guarda solo un puntero al buff del original)

extern random_device seed;
extern mt19937 rng;

class VirtualSequence{

protected:

	static const unsigned int alphabet_size;
	static const char alphabet[];
	
//	static mt19937 class_rng;
	
	// Texto de referencia de la INSTANCIA (=> ahorra espacio solo el constructor de copia)
	// Notar que size es el largo del TEXTO (n_bytes = size/4)
	seq_size_t size;
	unsigned char *data;
	bool owns_data;
	
	// Variables de la instancia (mutaciones)
	map<seq_size_t, char> mutations;
//	set<seq_size_t> mutations;
	
	uint32_t cur_count;
	bool cur_read;

public:
	
	VirtualSequence(bool _read_only = true);
	VirtualSequence(const char *_ref, unsigned int _size, bool _read_only = true);
	VirtualSequence(const string &_ref, bool _read_only = true);
	VirtualSequence(const unsigned int _size, const unsigned int _seq, bool _read_only = true);
	VirtualSequence(const VirtualSequence &original);
	
	virtual ~VirtualSequence();
	
	void mutate(mt19937 *arg_rng = NULL);
	char at(seq_size_t pos) const;
	
//	void mutatePunctual();
//	void mutateOTHER();
	
	string to_string();
	
	vector< pair<seq_size_t, char> > get_mutations();
//	vector<seq_size_t> get_mutations();
	
	void increase();
	void decrease();
	uint32_t count() const;
	bool read_only() const;
	bool operator==(const VirtualSequence&);
	unsigned int length() const;
	
};

#endif
