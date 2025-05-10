#include <iostream>
#include <cmath>

using namespace std;

double taylorLn1x(double x, int n)
{
	if (x <= -1)
	{
		cout << "Error: ln(1 + x) no está definido para x <= -1" << endl;
		return NAN;
	}

	double resultado = 0.0;

	for (int i = 1; i <= n; ++i)
	{
		double termino = pow(-1, i + 1) * pow(x, i) / i;
		resultado += termino;
	}

	return resultado;
}

int main()
{
	double x;
	int iteraciones;

	cout << "Ingrese el valor de x (x > -1) para ln(1 + x): ";
	cin >> x;
	cout << "Ingrese el número de términos de la serie de Taylor: ";
	cin >> iteraciones;

	double aproximacion = taylorLn1x(x, iteraciones);
	cout << "\nAproximación de ln(1 + " << x << ") usando " << iteraciones << " términos: " << aproximacion << endl;
	cout << "Valor real con log(): " << log(1 + x) << endl;

	return 0;
}
