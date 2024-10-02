#include <iostream>
#include <vector>

using namespace std;



int main ( ){

	
	vector<vector<double>> A = {{4, -1, 2}, {2, 5, 1}, {3 , 2, 8}};
	vector<double> R = {7, 5, 10};
	
	double semilla  [3] = {0};	
	cout<<endl<<endl<<"\tTabla :   "<<endl<<endl;
		
	for (int i = 0; i<5 ; i++){
		
		double x = (R[0] - (A[0][2] * semilla [2]) - (A[0][1] * semilla[1]) ) / A[0][0];
		double y = (R[1] - (A[1][0] * semilla [0]) - (A[1][2] * semilla[2]) ) / A[1][1];
		double z = (R[2] - (A[2][0] * semilla [1]) - (A[2][3] * semilla[3]) ) / A[2][2];
		
		cout<<" | "<<x<<" | "<<y<<" | "<<z<<endl;
		
		semilla [0] = x;  
		semilla [1] = y;
		semilla [2] = z;
	
	}	
	return 0; 
}