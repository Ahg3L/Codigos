#include <iostream>
#include <vector>

using namespace std;

void LU(vector<vector<double>>& A, vector<vector<double>>& L, vector<vector<double>>& U, vector<double>& R) {
    int n = A.size();
	float x,y,z = 0;

    L = vector<vector<double>>(n, vector<double>(n, 0.0));
    U = A;

    for (int i = 0; i < n; i++) {
        L[i][i] = 1.0;

        for (int j = i + 1; j < n; j++) {
            double factor = U[j][i] / U[i][i];
            
            L[j][i] = factor;
            for (int k = i; k < n; k++) {
                U[j][k] -= factor * U[i][k];
            }
        }
    }
    
    x = R[0];	
    y = (R[1] - L[1][0] * x);    
    z = R[2] - (L[2][0] * (x)) - (L[2][1]*y);
    
    
	
	R[2] = z / U[2][2];
	R[1] = (y - (U[1][2] * R[2])) / U[1][1]; 
	R[0] = (x - (U[0][2]* R[2]) - (U[0][1] * R[1])) / U[0][0];

}


int main() {

    vector<vector<double>> A = {{1, -1, 3}, {1, 1, 1}, {2 , 2, -1}};
	vector<vector<double>> L, U;
	vector<double> R = {13, 11, 7};
    int n = A.size();
    
    LU(A, L, U, R);
    
    cout << "Matriz A:\n";
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cout << A[i][j] << "\t";
        }
        cout << endl;
    }

    cout << "Matriz L (parte inferior de la descomposición LU):\n";
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cout << L[i][j] << "\t";
        }
        cout << endl;
    }

    cout << "Matriz U (parte superior de la descomposición LU):\n";
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cout << U[i][j] << "\t";
        }
        cout << endl;
    }
     
    cout << endl << "Matriz resuelta :\n";
  
    cout << "x = "<< R[0] << endl ; 
    cout << "y = "<< R[1] << endl ; 
	cout << "x = "<< R[2] << endl ; 
    return 0;
}

