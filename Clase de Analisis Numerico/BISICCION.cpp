#include <iostream>
#include <math.h>

using namespace std;

float funcion( double x[],int i){
	float R;
	R= (pow((cos(x[i])),2)) - (2*(x[i])) + (6);
	return R;
}

int main ( ){
	double a,b=0;
	double c[50];
	cout<<endl<<"Ingresa el rango 1 :"; cin>>a;
	cout<<endl<<"Ingresa el rango 2 : "; cin>>b;
	
	
	for (int i=0;i<50;i++){
		c[i] = ( (a+b) /2);
		cout<<endl<<i<<"   "<<a<<"|"<<b<<"  "<<c[i]<<"   "<<(c[i-1]-c[i])<<"   "<<funcion(c,i);
		
		if (funcion(c,i)>0){
			a = c[i];
		}
		else {
			b  = c[i];
		}
	}
	
	
	
	
	
	return 0;
}