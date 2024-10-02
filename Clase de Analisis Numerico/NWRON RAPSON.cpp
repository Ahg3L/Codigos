#include <iostream>
#include <math.h>

using namespace std;

float fun( double x){
	return pow(cos(x),2) - (2*x) + (6);
}

float derivada (int x){
	double R = (x*3.1416/180.0);
	return ((-sin(6)-2));
}

int main(){
	
	float x=3;	
	for(int i=0; i<10;i++){
		
		cout<<endl<<i<<" | "<< x <<" | "<<endl<<endl;
		float numero_sig = (x)-((fun(x))/(derivada(x)));
		float fun_numsig = fun(numero_sig);
		
		cout<< numero_sig <<"  "<<fun_numsig<<endl;
		cout<<fun(x)<<endl;
		cout<<derivada(x)<<endl;
		
		x=x-(fun(x)/derivada(x));
		system ("pause");
	}
	return 0;
}