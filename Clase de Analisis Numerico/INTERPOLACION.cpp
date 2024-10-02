#include <iostream>
#include <vector>

using namespace std;


int main ( ){

	float numM = 25;
	float num = 50;
	
	float ResulM =0.07;
	float Resulm =0.13;
	
	float Num = 30;
	float x =0;

	x = (((Num-numM) * ( Resulm - ResulM ))/ (num- numM)) + ResulM;		
	cout<< x;
	return 0; 
}