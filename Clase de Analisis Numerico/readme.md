## BISECCION 

Método de Bisección para encontrar raíces
Este código implementa el método numérico de bisección para encontrar una raíz de una función continua en un intervalo dado.

### Descripción

- La función a evaluar es:  

  f(x) = cos^2(x) - 2x + 6

- El usuario ingresa un intervalo [a,b] donde la función cambie de signo.  
- El método divide iterativamente el intervalo a la mitad y selecciona el subintervalo donde la función cambia de signo. Primero se evalua la funcion respecta a "a" y luego en el intervalo "b", y c = (a + b) / 2. Si f(c) es positivo a= c, si f(c) es negativo b = c; 
- El proceso se repite hasta que el valor absoluto de f(c) sea menor a un error especificado o se alcance un número máximo de iteraciones.
  
### Uso

1. Ejecuta el programa.  
2. Ingresa los valores de \(a\) y \(b\) cuando se soliciten.  
3. Observa las iteraciones y la raíz aproximada calculada.  

----------------------------------------------------------------------------------------------

## Cálculo Euler (e)

Este programa calcula una aproximación del número de Euler e utilizando su desarrollo en serie.

### Descripción

El número \( e \) puede calcularse mediante la siguiente serie infinita:


e = 1/0! + 1/1! + 1/2! + 1/3! + 1/4! ....

El programa solicita al usuario cuántas iteraciones (términos de la serie) desea usar para aproximar el valor de e, y calcula la suma parcial correspondiente.

### Detalles del programa

- La función `calcularEuler(int iteraciones)`:
  - Calcula e sumando 1/n! desde (n = 0) hasta `iteraciones`.
  - Usa un acumulador para evitar recalcular el factorial completo en cada paso.

- El `main()`:
  - Solicita al usuario el número de iteraciones.
  - Muestra el valor aproximado de e calculado con dicha cantidad de términos.

### Uso

1. Ejecuta el programa.
2. Ingresa el número de iteraciones (cuantos más términos, más precisa la aproximación).
3. El programa mostrará la aproximación del número e.

----------------------------------------------------------------------------------------------

## CALCULAR π 
usando la Serie de Leibniz

### Descripción

Este programa aproxima el valor de π utilizando la serie de Leibniz, una serie infinita alternada.  
El usuario ingresa la cantidad de iteraciones (términos de la serie) que desea utilizar para mejorar la precisión del resultado.


### Fundamento matemático

La serie de Leibniz está dada por la fórmula:

π / 4 = ((-1)^n​) / 2n+1

La serie es de convergencia lenta, por lo que se necesitan muchas iteraciones para obtener una buena aproximación.

### Lógica del programa

1. Se solicita al usuario el número de iteraciones.
2. Se usa un ciclo `for` que suma o resta los términos -1^n/2n+1 alternando el signo.
3. El resultado final se multiplica por 4 para obtener la aproximación de π.


