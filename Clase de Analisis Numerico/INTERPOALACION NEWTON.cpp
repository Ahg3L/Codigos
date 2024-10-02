 		#include<iostream>
#include <vector>

using namespace std;

	vector<vector<double>> A = {{-3,-51},{-1,-11}, {1,-11}} ; 
	double R  [2];
	double R2;
		
	double funcion(double x){
   		return A[0][1] + (R[0] * (x - A[0][0])) + (R2 * (x - A[0][0]) * (x - A[1][0]));
	}

main(){
		 
	for(int i = 0 ; i<=1; i++){
		R [i] = (A[i+1][1] - A[i][1]) / (A[i+1][0] - A[i][0]);
	}
	
	R2 = (R[1] - R[0]) / (A[2][0] - A[0][0]);	
	
	for(double i = -5; i<=5; i++ ){
		cout<<i<<" , "<<funcion(i)<<endl;	
	}	
	
	cout<<funcion(.5)<<endl<<funcion(4)<<endl<<funcion(3.4);
}