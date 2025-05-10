#include <iostream>

using namespace std;

float suma (float a,int b[], int c){
	a=0.0;
	for (int i =0; i<c;i++){
        a =a + b[i];
	}
	return a;
}

float promedio(float g,int h[], int i){
	float P = suma(g,h,i);
	return P/i;
}

int numeroM(float j,int k[], int m){
	int tall = k[0];
	for (int i =0; i<m;i++){
		if(k[i]> tall) {
	       tall = k[i];
	    }
	}
	return tall;
}

int numerom(float o,int p[], int q){
	int small = p[0];
	for (int i =0; i<q;i++){
		if(p[i]< small) {
	       small = p[i];
	    }
	}
	return small;
}

void mostrar_menu(float d,int e[], int f){
	int ops=0;
	
	system("cls");
	for(int i=0;i<f;i++){cout<<e[i]<<" | ";}
	
	cout<<endl<<endl<<"Que dato requieres? "<<endl;
    cout<<"1.- Suma de todos los numeros. "<<endl;
    cout<<"2.- Promedio de los numeros. "<<endl;
    cout<<"3.- Numero mas alto."<<endl;
	cout<<"4.- Numero mas bajo."<<endl;	

	cin>>ops;
	switch(ops ){
		case 1:
			cout<<endl<<"El resultado de la suma es: "<<suma(d,e,f)<<endl;
		break;
			
		case 2:
			cout<<endl<<"El resultado del promedio es: "<<promedio(d,e,f)<<endl;
		break;
		
		case 3:
			cout<<endl<<"El numero mas alto es: "<<numeroM(d,e,f)<<endl;
		break;
		
		case 4:
			cout<<endl<<"El numero mas bajo es: "<<numerom(d,e,f)<<endl;
		break;
		
		default:
			cout<<endl<<"No es una opcion valida : ( "<<endl;
	}
}

int main(){
	
    int ops1,lista = 0;
    float suma1 = 0.0;
	system("cls");
	
    cout<<"Cuantos datos tiene? ";
    cin>>lista;
    int vector[lista];
    
    cout<<"Escribe los datos consecutivamente:"<<endl;
    for(int t=0;t<lista;t++){
        cin>>vector[t];
    }
    
	for(int x=0;x<1;x){
	mostrar_menu(suma1,vector,lista);
	cout<<endl<<"¿Quieres realizar otra operacion?"<<endl; 
	cout<<"1.- Si."<<endl;
	cout<<"2.- No."<<endl;
	cin>>ops1;
		switch (ops1){
			case 1:
				x=0;
			break;
			case 2:
				x=2;
			break;	
		}
    }
system("pause");
    return 0;
}
