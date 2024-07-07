import os

def limpiar_pantalla():
    os.system('cls' if os.name == 'nt' else 'clear')

limpiar_pantalla()

listaX = [7,1,10,5,4,3,13,10,2]
listaY = [2,9,2,5,7,11,2,5,14]
listaXY = []     
listaX2 = []
sumaX = 0
sumaY = 0
sumaXY = 0
sumaX2 = 0

for i in range(0,9):
    listaXY.append(listaX[i] * listaY[i])    
    listaX2.append(listaX[i] * listaX[i])
    sumaX = sumaX + listaX[i]
    sumaY = sumaY + listaY[i]
    sumaXY = sumaXY + listaXY[i]
    sumaX2 = sumaX2 + listaX2[i]

m = (sumaXY - ((sumaX * sumaY) / 9)) / (sumaX2 - ((sumaX * sumaX) / 9))
b = (sumaY / 9) + (-m * (sumaX / 9))

print("La formula de la pendiente es :",m,"x + " ,b) 