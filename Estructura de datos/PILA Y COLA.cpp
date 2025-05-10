#include <iostream>
#include <map>

using namespace std;

int maximoP = 0, maximoC = 0, minimoP = 0, minimoC =0 ;	
int contadorP=0,contadorC = 0;    	
int pila [15] = {0};
int cola [15]={0};

void anadir(int c, int d){
	int opsa = 0;
	int valor = 0;
	
	cout<<"Pila o cola ?"<<endl;
	cout<<"1.- Pila"<<endl;
	cout<<"2.- colas"<<endl;
	cout<<"Ingresa una opscion : ";cin>>opsa;
	switch (opsa){	
		case 1:
			cout<<endl<<"Ingrese un valor: : ";cin>>valor;
			pila[c] = valor;
			contadorP++;
		break;
		
		case 2:		
			cout<<endl<<"Ingrese un valor: : ";cin>>valor;
			cola[d] = valor;
			contadorC++;
		break;
		
		default:
            cout << "Opción no válida." << endl;
        break;
	}
}


struct Mm{
int maximoP = 0, maximoC = 0, minimoP = 0, minimoC =0 ;	
}maxi,mini;

void max() {
	maxi.maximoP = 0, maxi.maximoC = 0;	
	
	for(int i=0;i<15;i++){
		if(pila[i]>maxi.maximoP){
			maxi.maximoP = pila[i];
		}
		if(cola[i]>maxi.maximoC){
			maxi.maximoC = cola[i];
		}
	}
}

void min(){
	mini.minimoP = pila[0];
	mini.minimoC = cola[0];
	for (int j =0;j<contadorP;j++){
		if(pila[j]<mini.minimoP) {
	       mini.minimoP = pila[j];
	    }
	}
	
	for (int i =0;i<contadorC;i++){	
	    if(cola[i]< mini.minimoC) {
	       mini.minimoC = cola[i];
	    }
	}
}


struct moda{
	int modaP, modaC;
};

void moda(){
	map<int, int> frequency;
	map<int, int> frequencyC;
	int maxCount = 0, mode= -1, maxCount2 = 0, mode2 = -1;

    for (int i = 0; i < contadorP; i++) {
        frequency[pila[i]]++;
        if (frequency[pila[i]] > maxCount) {
            maxCount = frequency[pila[i]];
            mode = pila[i];
        }    
    }
    
    if (maxCount > 1) {
        cout<<endl<<"La moda de la pila es: "<<mode<< " con "<<maxCount<<" repeticiones."<<endl;
    } else {
        cout<<"No hay una moda en el arreglo."<<endl;
    }
    
    for (int i = 0; i < contadorC; i++) {
		frequencyC[cola[i]]++;
        if (frequencyC[cola[i]] > maxCount2) {
            maxCount2 = frequencyC[cola[i]];
            mode2 = cola[i];
        }
	}
	if (maxCount2 > 1) {
        cout<<endl<<"La moda de la cola es: "<<mode2<< " con "<<maxCount2<<" repeticiones."<<endl<<endl;
    } else {
        cout<<"No hay una moda en el arreglo."<<endl;
    }
}


struct suma{
	float sumapila ;
	float sumacola ;
}S; 

void sumaT(){
	S.sumapila=0;
	S.sumacola=0;
	for(int j = 0; j<15;j++){
		S.sumapila= S.sumapila + pila[j];
		S.sumacola= S.sumacola + cola[j];
	}
}

struct prom{
	float prompila ;
	float promcola ;
}P;

void promedio(){
	sumaT();
	P.prompila=S.sumapila/contadorP;
	P.promcola=S.sumacola/contadorC;
}


void rango(){
	min();
	max();
	cout<<endl<<endl<<"El rango de la pila : "<<(maxi.maximoP)-(mini.minimoP)<<endl;
	cout<<"El rango de la cola : "<<(maxi.maximoC)-(mini.minimoC)<<endl;
}


void desviacion(){
	float desP,desC = 0.0;
	promedio();
	for(int i= 0;i<contadorP;i++){
		desP=desP + ( (pila[i]- P.prompila)/contadorP );
	}
	for(int i= 0;i<contadorC;i++){
		desC=desC + ( (cola[i]- P.promcola)/contadorC );
	}
	cout<<endl<<"La desviacion media de la pila es : "<<desP<<endl;
	cout<<"La desviacion media de la cola es : "<<desC<<endl<<endl;	
}

int main(){
	
	int opcion = 0;

	while (true) {
		system("cls");
		
		cout<<"  Pila  :  ";
		for(int i=0; i<15; i++){
			cout<<pila[i]<<" | ";
		}
		cout<<endl<<"  Cola  :  ";
		for(int i=14; i>=0; i--){
			cout<<cola[i]<<" | ";
		}
		cout << endl<<endl<<"Elija una opcion "<<endl;
		cout << "1. Anadir un elemento" << endl;
	    cout << "2. Encontrar el maximo" << endl;
	    cout << "3. Encontrar la moda" << endl;
	    cout << "4. Encontrar la sumatoria" << endl;
		cout << "5. Encontrar el promedio" << endl;
	    cout << "6. Desplegar el rango" << endl;
	    cout << "7. Calcular la desviacion media" << endl;
		cout << "8. Salir" << endl;
		
        cin >> opcion;
        
        switch (opcion) {
        	case 1:
        		anadir(contadorP,contadorC);
        	break;
        	
        	case 2: // Encontrar el máximo
                max(); 
              	cout<<endl<<"Maximo Pila : "<<maxi.maximoP<<"         Maximo Cola : "<<maxi.maximoC<<endl;
				system("pause");
            break;
            
            case 3: // Encontrar la moda
            	moda();
            	system("pause");
            break;
                
            case 4: // Encontrar la sumatoria
            	sumaT();
                cout<<endl<<endl<<"La sumatoria de la pila es : "<<S.sumapila<<"   y el de cola es : "<<S.sumacola<<endl;
                system("pause");
            break;
            
            case 5: // Encontrar el promedio
                promedio();
                cout<<"Promedio pila : "<<P.prompila<<endl;
				cout<<"Promedio cola : "<<P.promcola<<endl;
                system("pause");
            break;
            
            case 6: // Desplegar el rango
                rango();
                system("pause");
            break;
                
            case 7: // Calcular la desviación media
        		desviacion();
        		system("pause");
			break;
			
			case 8: // Termina el programa 
			return 0;
			break;
			
			default:
            	cout << "Opción no válida." << endl;
            break;
    	}
	}
	return 0;
}