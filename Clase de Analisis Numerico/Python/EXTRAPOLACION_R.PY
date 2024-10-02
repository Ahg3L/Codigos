import os

def limpiar_pantalla():
    os.system('cls' if os.name == 'nt' else 'clear')

def funcionP(x):
    return (-.1)*(x**4) - 0.15*(x**3) - 0.5*(x**2) - 0.25*(x) + 1.2

limpiar_pantalla()
h1= .5
h2 =.25

paso1 = ((funcionP(h1 + h1) - funcionP(h1 - h1)) / (2*h1)) 
paso2 = ((funcionP(h1 + h2) - funcionP(h1 - h2)) / (2*h2)) 

Extra_Richar = ((4*paso2)/3) - (paso1 / 3)
print(Extra_Richar)