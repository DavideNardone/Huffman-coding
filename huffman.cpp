#include "huffman.h"





nodo::nodo(int x,DATA_32 frequenza){
fit=false;
bits=""; 
sx=NULL;
dx=NULL;
father=NULL;
valore=DATA(x); 
freq=frequenza; 
}




nodo::nodo(nodo* SX,nodo* DX){
fit=true; 
bits="";
sx=SX;
dx=DX;	
father=NULL;
valore=0; 
freq=SX->freq+DX->freq;
SX->father=sx;
DX->father=dx; 
}



int nodo::getval(){
return valore;
}

int nodo::getfreq(){
return freq;
}

bool nodo::isNode() 
{ return fit; } 

nodo* nodo::Sx() 
{ return sx; } 

nodo* nodo::Dx() 
{ return dx; }

code_word& nodo::Code() 
{ return bits; }



int Min_priority_queue::Parent(int i){
return (i/2)-1;
}

int Min_priority_queue::Left(int i){
return 2*i+1;
}

int Min_priority_queue::Right(int i){
return 2*i+2;
}



void Min_priority_queue::Build_Min_Heap(vector<nodo*>& v){
for (int i=(v.size()-1)/2; i>=0;i--){
Min_Heapify(v,i); } 
}



void Min_priority_queue::Min_Heapify(vector<nodo*>& v,int i){
int l,r,mini,size=v.size()-1;
l=Left(i);
r=Right(i);
if( l <= size && v[l]->getfreq() < v[i]->getfreq() ) 
mini=l;
else mini=i;
if(r <= size && v[r]->getfreq() < v[mini]->getfreq() )
mini=r;
if(mini!=i)
{
swap(v[i],v[mini]);
Min_Heapify(v,mini);
}
}




nodo* Min_priority_queue::Heap_extract_min(vector<nodo*>& v){
if(v.size() < 1){
cout<< "Underflow dell’ heap" <<endl;
exit(1);}
nodo* n=v[0];
v[0]=v[v.size()-1];
v.erase(v.end()-1);
Min_Heapify(v,0);
return n;
}




void Min_priority_queue::Min_heap_insert(vector<nodo*>& v,nodo* key){
int i=v.size()-1;
v.push_back(key);
while( i>1 && v[Parent(i)]->getfreq() > v[i]->getfreq()){
swap(v[i],v[Parent(i)]);
i=Parent(i);
}}



HuffmanTree::HuffmanTree(ifstream& source) 
:input(source) { 
root=NULL;
orig_size=0;	
ns=0;
built=false;
}



HuffmanTree::~HuffmanTree() {
if(root) {
DestroyRoot(root);
cout<<"Memoria Liberata!!!"<<endl;
}}



void HuffmanTree::DestroyRoot(nodo* HuffmanTree_node) {
if (HuffmanTree_node->isNode()) { 
DestroyRoot(HuffmanTree_node->Sx()); 
DestroyRoot(HuffmanTree_node->Dx());
} delete HuffmanTree_node; } 



void HuffmanTree::BuildHuffman() {
int f[DATA_MAX]={0};	
int x;
code_word choice;
for (orig_size=0;(x=input.get())!=EOF;orig_size++) f[x]++;
vector<nodo*> v;
for (x=0;x<DATA_MAX;x++)
if (f[x]) v.push_back(new nodo(x,f[x]));
ns=(short)v.size();  // e come chiamare un metodo Huffman_Tree che li setta !
root=Make_tree(v);	
BuildEncodeMAP(root,"");
built=true;			
cout<<"Vuoi stampare la codifica di Huffman ?		YES/NO "<<endl;
cin>>choice;
if(choice=="YES") View();
}



nodo* HuffmanTree::Make_tree(vector<nodo*>& V) {
int size=V.size();
Min_priority_queue *coda_p=new Min_priority_queue;
coda_p->Build_Min_Heap(V);
for(int i=0;i<size-1;i++){
nodo* x=coda_p->Heap_extract_min(V);
//cout<<"X_VAl: "<<x->getval()<<endl;
//cout<<"X_FREQ: "<<x->getfreq()<<endl;
nodo* y=coda_p->Heap_extract_min(V);
nodo* z=new nodo(y,x);
coda_p->Min_heap_insert(V,z);
}
return V[0];
} 



void HuffmanTree::BuildEncodeMAP(nodo* nod,code_word enc_str) {
if (nod->isNode()) {
BuildEncodeMAP(nod->Sx(),enc_str+"0");
BuildEncodeMAP(nod->Dx(),enc_str+"1");
} else {
nod->Code()=enc_str; 
EncodeMAP[nod->getval()]=nod;
} } 



void HuffmanTree::View() {
if (!built) BuildHuffman(); 
map<DATA,nodo*>::iterator it; 
cout<<"Sono presenti "<<EncodeMAP.size()<<" caratteri ASCII "<<endl<< "=========================================="<<endl; 
for (it=EncodeMAP.begin();it!=EncodeMAP.end();it++) {
cout<< "Pair: "<<it->first<<" >>>CODE<<< "<<it->second->Code()<<endl; } }




Huffman::Huffman(ifstream& source,ofstream& dest)
: HuffmanTree(source),output(dest)
{}



void Huffman::Encode() {
DATA x;			
code_word code;		
if (!built) BuildHuffman();
// scrive l'header sull'output
output.write((char*)&orig_size,sizeof(orig_size)); 
output.write((char*)&ns,sizeof(ns));
map<DATA,nodo*>::iterator it;
for (it=EncodeMAP.begin();it!=EncodeMAP.end();it++) { 
nodo b(it->second->getval(),it->second->getfreq());	//usata solo come variabile per scrivere su output
output.write((const char*)&b,sizeof(nodo));
}
input.clear();
input.seekg(0,ios::beg);
while (!input.eof()) {
x=input.get();
Encode_Pair(x,code); 
WriteCode(code);
}
for (int i=0;i<7;i++) putbit(0);
cout<<endl<<endl;
Compression_Rate();
}




bool HuffmanTree::Encode_Pair(const DATA& x,code_word& codice) { 
if (!built) BuildHuffman();
map<DATA,nodo*>::iterator it; 
it=EncodeMAP.find(x); 
if (it!=EncodeMAP.end()) {
codice=it->second->Code(); 
return true;
} return false; }



void Huffman::Compression_Rate(){
DATA_32 lenght=0;
DATA_32 dim_bit=0;
output.seekp(0,ios::end);		// definisce la dimensione del file codificato
lenght=output.tellp();
Huffsize(root,dim_bit);
cout<<endl<<endl<<endl;
cout<<"Il numero di bit per codificare il file e': "<<dim_bit<<endl;
cout<<"La lunghezza del file originale e': "<<orig_size<<" byte "<<endl;
cout<<"La lunghezza del file codificato e': "<<lenght<<" byte "<<endl;
cout<<"Tasso di compressione: "<<(float)(((1-(float)lenght/(float)orig_size))*100)<<" % "<<endl;
}




void HuffmanTree::Huffsize(nodo* n,DATA_32& dimensioni_in_bit) {
if (n->isNode()) { 
Huffsize(n->Sx(),dimensioni_in_bit); 
Huffsize(n->Dx(),dimensioni_in_bit);
}
else 
dimensioni_in_bit+=n->getfreq() * n->Code().size();
} 





void Huffman::WriteCode(code_word& bit_sequence) {
code_word::iterator it;
for (it=bit_sequence.begin();it!=bit_sequence.end();it++)
if (*it=='0'){
putbit(0); }
else{
putbit(1);	//ricordarsi carattere terminatore nella codifica come bit=1
}
}



// Il metodo putbit scrive su un file di stream di output 8 bit alla volta
// buffer contiene il valore che dovrà essere scritto per la sequenza di bit ricevuti
// fintanto che wmask non si azzera memorizziamo il valore per i primi 8 bit
void Huffman::putbit(int bit) {
char c;
static unsigned int buffer=0;  // variabile di memorizzazione della codifica dei primi 8 bit
static unsigned int wmask=128; // rappresenta 1/8 bit ricevuti in input
cin>>c;
cout<<"bit: "<<bit<<endl;
if (bit) 
buffer=buffer | wmask; //se il bit è 1 memorizziamo il suo valore effettuando una "OR" tra BUFFER | WMASK
if ((wmask=wmask>>=1) == 0){
cout<<"CHAR: "<<(char)buffer<<endl;
output.put((char)buffer);
buffer = 0;  // vengono reimpostati buffer a 0
wmask = 128; // e wmask a 128 per leggere i successivi 8 bit
}
cout<<"buffer_extern: "<<buffer<<endl;
cout<<"wmask_extern: "<<wmask<<endl;
}



void Huffman::Decode() {
DATA_32 fsize; 		
short ns;
// Legge l'header
input.read((char*)&fsize,sizeof(fsize)); // legge le dim. originale del file
//cout<<"FSIZE: "<<fsize<<endl;
input.read((char*)&ns,sizeof(ns)); // legge il numero di simboli(singoli) che comparivano nel file di input codificato precedentemente.
//cout<<"NS: "<<ns<<endl;
vector<nodo*> v; 
nodo h; //variabile d' appoggio contenente gli attributi: valore/freq
for(int i=0;i<ns;i++) { 
input.read((char*)&h,sizeof(nodo));
nodo* n=new nodo(h.getval(),h.getfreq()); 
v.push_back(n);
}
root=Make_tree(v); 
for (unsigned long i=0;i<fsize;i++) { //fsize=numero di caratteri file originale
nodo* n=root; 
while (n->isNode()) {
if (getbit()) // se il valore che ritorna è 1 scendi sul sottoalbero di DX
n=n->Dx();
else		
n=n->Sx();
}//END WHILE
//cout<<"VAL: "<<n->getval()<<endl;
output.put((int)n->getval()); 
}//END FOR
}


// legge 1 bit (0,1)
// Effettuando il processo inverso a putbit e quindi leggendo 1 byte alla volta
// restituiamo una sequenza di bit che corrisponde alla parola in codice
// che decifreremo sull' albero per estrarre il carattere esatto e scriverlo sul file di output
int Huffman::getbit() {
char t;
static unsigned int buffer;
static unsigned int rmask=0;
if ((rmask=rmask >>= 1) == 0) {
cout<<"rmask_intern: "<<rmask<<endl;
cin>>t;
buffer=input.get(); 
rmask = 128;
}
cout<<"rmask_extern: "<<rmask<<endl;
cout<<"Buffer: "<<buffer<<endl;
cout<<"Bit: "<<((buffer & rmask)!=0)<<endl;
return ((buffer & rmask)!=0); }









