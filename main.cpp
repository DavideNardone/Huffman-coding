#include "huffman.h"



void menu(int &scelta){
do{
cout<<"***********************************MENU*********************************"<<endl;
cout<<"                              1)Codifica di Huffman                             "<<endl;
cout<<"                              2)Decodifica di Huffman                         "<<endl;
cout<<"                              0)Uscita                                         	        "<<endl;
cout<<"****************************************************************************"<<endl;
cout<<endl<<endl;
cout<<endl<<"\t\t\t     Scelta : ";
cin>>scelta;
cout<<endl;
if (scelta==0){
cout<<"Arrivederci!"<<endl;
exit(1);
}
if(scelta<0 | scelta>2)
cout<<endl<<endl<<"La scelta "<<scelta<<" e' errata."<<endl<<"Prego, ripetere l'inserimento!!!"<<endl;
}while (scelta<0 | scelta>2);
}

void init(int& opzione,char nomefile_in [],char nomefile_out[]){
if(opzione==1){
cout<<"Inserire il nome del file di input (con estensione): ";
cin>>nomefile_in;
}
else{
cout<<"Inserire il nome del file di input (senza estensione): ";
cin>>nomefile_in;
strcat(nomefile_in,".code");
}
if(opzione==1){
cout <<"Inserire il nome del file di output (senza estensione): ";
cin>>nomefile_out;
strcat(nomefile_out,".code");
}
else{
cout <<"Inserire il nome del file di output (con estensione): ";
cin>>nomefile_out;
}
}


int main()
{
int opzione;
char *nomefile_in=new char[64];
char *nomefile_out=new char[64];
menu(opzione);
init(opzione,nomefile_in,nomefile_out);
ifstream* in=new ifstream(nomefile_in,ios::binary);
ofstream* out=new ofstream(nomefile_out,ios::binary | ios::trunc); //ios::trunc apre il file con cancellazione del vecchio stream se esistente, altrimenti crea il file
Huffman* H=new Huffman(*in,*out);
switch(opzione){
case 0:
exit(1);
break;
case 1:
H->Encode();
break;
case 2:
H->Decode();
break;
default:
cout<<"Errore irreversibile"<<endl;
exit(1);
break;
}
in->close();
out->close();
delete []nomefile_in;
delete []nomefile_out;
delete in;
delete out;
delete H;
return 0;
}


