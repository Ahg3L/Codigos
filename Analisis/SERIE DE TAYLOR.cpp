#include <iostream>
#include <cmath>
#include <math.h>

using namespace std;
double calcular(int num){
	
	int aprox =1;
    float r= 0;
    
	for (int i=0;i=num;i++){
	       
	
	r+=(sqrt(aprox))-(sqrt(aprox)*(2*i-3/(pow(2*aprox,i)   )) );
	
    }
	
	cout<<"El resultado es : "<< r;	
}



int main (){
	
	int n=0;
	
	cout<<"Con cuantos decimales¿? "<<endl; 
	cin>>n;
	calcular (n);

	return 0;
}