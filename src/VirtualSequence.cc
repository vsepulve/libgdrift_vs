#include "VirtualSequence.h"

const unsigned int VirtualSequence::alphabet_size = 4;
const char VirtualSequence::alphabet[] = {'A', 'C', 'G', 'T'};
//mt19937 VirtualSequence::class_rng;

VirtualSequence::VirtualSequence(bool _read_only){

	size = 0;
	data = NULL;
	owns_data = false;
	
	cur_count = 0;
	cur_read = _read_only;
}

// Constructor real que COPIA el buffer de texto
VirtualSequence::VirtualSequence(const char *_ref, unsigned int _size, bool _read_only){
	
//	cout<<"VirtualSequence - Inicio (text: "<<_ref<<", size: "<<_size<<")\n";
	size = (seq_size_t)_size;
//	data = new char[size+1];
	unsigned int data_size = (size>>2);
	if( size & 0x3 ){
		++data_size;
	}
	data = new unsigned char[ data_size ];
	memset(data, 0, data_size);
//	strcpy(data, _ref);
	unsigned char *p = data;
	unsigned int disp = 0;
	unsigned char value = 0;
	for(unsigned int i = 0; i < size; ++i){
		switch ( _ref[i] ){
			// Tambien notar que seria mas seguro asociar esto a alphabet
			case 'A' : value = 0; break;
			case 'C' : value = 1; break;
			case 'G' : value = 2; break;
			case 'T' : value = 3; break;
			default : break;
		}
//		cout<<"Escribiendo "<<_ref[i]<<" (value: "<<(unsigned int)value<<", disp: "<<disp<<")\n";
		value <<= disp;
//		cout<<"New value: "<<(unsigned int)value<<"\n";
		*p |= value;
//		cout<<"*p: "<<(unsigned int)(*p)<<"\n";
		disp += 2;
		if(disp > 6){
			++p;
			disp = 0;
		}
	}
	owns_data = true;
	
//	for(unsigned int i = 0; i < data_size; ++i){
//		cout<<"data["<<i<<"]: "<<(unsigned int)data[i]<<"\n";
//	}
	
	cur_count = 0;
	cur_read = _read_only;
}

// Constructor real que COPIA el buffer de texto, pero que recibe solo 1 entero (32 bits) de secuencia
// Llena el resto de A's (con un memset... 0)
VirtualSequence::VirtualSequence(const unsigned int _size, const unsigned int _seq, bool _read_only){

	size = (seq_size_t)_size;
	unsigned int data_size = (size>>2);
	if( size & 0x3 ){
		++data_size;
	}
	data = new unsigned char[ data_size ];
	memset(data, 0, data_size);
	
	// Leo cada numero e invierto su posicion
	// Para la lectura, itero por cada numero de seq y voy moviendo mask
	// Para tomar el valor aplico mask y la muevo en pos<<1 (x2 pues son 2 bits por valor)
	// Para la invertir la posicion, la resto de size-1
	// Uso la posicion inversa para escoger el byte (/4, es decir >>2)
	// ... y para mover los bits (el resto, es decir &0x3, <<1 pues son 2 bits por valor)
	unsigned int mask = 0x3;
	unsigned int new_pos = 0;
	unsigned int value = 0;
	for(unsigned int pos = 0; pos < size; ++pos, mask<<=2){
		value = (_seq & mask) >> (pos<<1);
		new_pos = size - 1 - pos;
		data[ (new_pos>>2) ] |= (value << ((new_pos & 0x3)<<1) );
	}
	
//	for(unsigned int i = 0; i < data_size; ++i){
//		cout<<"data["<<i<<"]: "<<(unsigned int)data[i]<<"\n";
//	}
	
	cur_count = 0;
	cur_read = _read_only;
	
}

// Constructor real que COPIA el buffer de texto
VirtualSequence::VirtualSequence(const string &_ref, bool _read_only)
 : VirtualSequence(_ref.c_str(), _ref.length(), _read_only)
{

}

// Constructor de copia que solo almacena un puntero al buffer de texto
VirtualSequence::VirtualSequence(const VirtualSequence &original){

	size = original.size;
	data = original.data;
	owns_data = false;
	
	cur_count = 0;
	cur_read = original.cur_read;
}

VirtualSequence::~VirtualSequence(){
	mutations.clear();
	if(owns_data){
		delete [] data;
	}
	data = NULL;
	size = 0;
}

void VirtualSequence::mutate(mt19937 *arg_rng){
	// escoger al azar posicion
	// mutar (agregar a mapa)
	// para que esto sea totalmente efectivo, 
	// ...hay que mutar hasta encontrar una posicion no escogida previamente (o el maximo)
	if(arg_rng == NULL){
		arg_rng = &rng;
	}
	// Version de texto completo, podría usarse una expresión reducida de la mutacion
	// En esta version NO estoy verificando que la mutacion sea efectiva
	uniform_int_distribution<> pos_dist(0, size-1);
	uniform_int_distribution<> val_dist(0, alphabet_size-1);
	seq_size_t pos = pos_dist(*arg_rng);
	char value = alphabet[val_dist(*arg_rng)];
	mutations[pos] = value;
}

char VirtualSequence::at(seq_size_t pos){
	if(pos >= size || data == NULL){
		// exception !
		return 0;
	}
	// Aplico la mutacion si existe (es decir, tiene prioridad)
	map<seq_size_t, char>::iterator res = mutations.find(pos);
	if( res != mutations.end() ){
		return res->second;
	}
	else{
		// Primero tomo el byte (data en posicion pos/4)
		unsigned char byte = data[ pos>>2 ];
//		cout<<"VirtualSequence::at - pos: "<<pos<<", byte: "<<(unsigned int)byte<<"\n";
		// Ahora tomo el valor correcto del byte, los 2 bits que busco
		// Para eso aplico la mascara 0x3 (2 bits) corrida en el resto de pos/4, x2
		// El resto de pos/4 lo calculo como pos & 0x3 (por los 2 bits de la division)
		// El x2 (porque son 2 bits por posicion) lo aplico con un <<1 adicional
		unsigned char val = byte & (0x3 << ((pos & 0x3)<<1) );
//		cout<<"VirtualSequence::at - val: "<<(unsigned int)val<<"\n";
		// Por ultimo, muevo el valor (que estaba en medio del byte) a su posicion inicial
		val >>= ((pos & 0x3)<<1);
//		cout<<"VirtualSequence::at - final val: "<<(unsigned int)val<<" ("<<alphabet[val]<<")\n";
		return alphabet[val];
	}
}

string VirtualSequence::to_string(){
	string seq;
	if(size == 0 || data == NULL){
		return seq;
	}
	for(unsigned int i = 0; i < size; ++i){
		// La logica de la decodificacion y aplicacion de mutaciones esta en at()
		seq.push_back(at(i));
	}
	// reverse ???
	// Esto no es necesario ahora que el at decodifica en el orden correcto
	// reverse(seq.begin(), seq.end());
	return seq;
}

vector< pair<seq_size_t, char> > VirtualSequence::get_mutations(){
	vector< pair<seq_size_t, char> > res;
	map<seq_size_t, char>::iterator it;
	for(it = mutations.begin(); it != mutations.end(); it++){
		res.push_back(pair<seq_size_t, char>(it->first, it->second));
	}
	return res;
}

void VirtualSequence::increase(){
	++cur_count;
}

void VirtualSequence::decrease(){
	--cur_count;
}

uint32_t VirtualSequence::count() const{
	return cur_count;
}

bool VirtualSequence::read_only() const{
	return cur_read;
}

bool VirtualSequence::operator==(const VirtualSequence &seq){
	if( mutations.size() != seq.mutations.size() ){
		return false;
	}
	map<seq_size_t, char>::const_iterator it1, it2;
	for( it1 = mutations.begin(), it2 = seq.mutations.begin(); it1 != mutations.end(); it1++, it2++ ){
		if( (it1->first != it2->first) || (it1->second != it2->second) ){
			return false;
		}
	}
	return true;
}

unsigned int VirtualSequence::length() const{
	return (unsigned int)size;
}














