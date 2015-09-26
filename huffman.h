#include <iostream> 
#include <fstream> 
#include <vector> 
#include <string> 
#include <map>
using namespace std;

typedef unsigned long DATA_32; 
typedef unsigned char DATA; 
typedef string code_word; 			
#define DATA_MAX 256   			




class nodo {  				
private: 
DATA valore; 		
DATA_32 freq;
nodo *sx,*dx,*father;		
code_word bits;		
bool fit;						
public: 	  
nodo(){}
nodo(int,DATA_32); 			
nodo(nodo*,nodo*); 			
int getval();
int getfreq();
bool isNode(); 				
nodo* Sx(); 				
nodo* Dx(); 				
code_word& Code();
}; 



class Min_priority_queue {
public:
int Parent(int);
int Left(int);
int Right(int);
void Build_Min_Heap(vector<nodo*>&);
void Min_Heapify(vector<nodo*>&,int);
nodo* Heap_extract_min(vector<nodo*>&);
void Min_heap_insert(vector<nodo*>&,nodo*);
};




class HuffmanTree {
protected: 
nodo* root;				
DATA_32 orig_size;	
short ns;
bool built;
ifstream& input; 			
map<DATA,nodo*> EncodeMAP;
public:
HuffmanTree(ifstream&); 
~HuffmanTree();	 	
void BuildHuffman();		
nodo* Make_tree(vector<nodo*>&);		
void BuildEncodeMAP(nodo*,code_word); 	
void View();						
bool Encode_Pair(const DATA&, code_word&);			
void Huffsize(nodo*,DATA_32&); 	
void DestroyRoot(nodo*);	
};





class Huffman: public HuffmanTree { 
private: 
ofstream& output; 	
public:
Huffman(ifstream&,ofstream&); 
void Encode(); 			
void Decode(); 			
void WriteCode(code_word&);		
void Compression_Rate();
void putbit(int); 				
int getbit(void);				
}; 

