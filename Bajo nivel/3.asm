;GARCIA GONZALEZ MIGUEL ANGEL       BOLETA : 2022350794

imprime MACRO cadena
  LEA DX, cadena
  MOV AH, 09h
  INT 21h
ENDM

scand MACRO
  MOV AH, 01
  INT 21h
ENDM

.model small
.data
  num      DB ?
  signo    DB 0
  mult     DW 0001h
  signoM   DB "-$"
  mensaje  DB 10, 13, "Ingrese un numero: ", 10, 13, "$"
  es_par   DB "  El numero es PAR", 10, 13, "$"
  es_impar DB "  El numero es IMPAR", 10, 13, "$" 
  n1       DB "1$"
  n2       DB "2$"
  n3       DB "3$"
  n4       DB "4$"
  n5       DB "5$"
  n6       DB "6$"
  n7       DB "7$"
  n8       DB "8$"
  n9       DB "9$"

.code
  MOV AX, @data
  MOV DS, AX
  MOV CX, 6  
  imprime mensaje

loopee:

  scand
  SUB AL, '0'
  CMP AL, 1h
  JNE compara
  ADD signo, 1h

compara:

  TEST AL, 1    ; Comprueba el bit menos significativo para determinar si es par o impar
  JZ numero_par ; Si el bit está en 0, salta a la etiqueta "numero_par"
  MOV AH, 00H
  MUL mult        
  MOV mult,AX
  imprime es_impar
  JMP sig1
           
numero_par:   
  MOV AH, 00H
  MUL mult 
  MOV mult,AX
  imprime es_par

sig1:

  LOOP loopee 
  
  TEST signo, 1
  JZ resultado

  imprime signoM  
                                                                                
resultado:                                                                    

           
  MOV BX,mult  ;El resultado de la multiplicacion de pasa a BX
  
  CMP BX,90    
  JGE m9
         
  CMP BX,80
  JGE m8   
             
  CMP BX,70
  JGE m7      
                
  CMP BX,60       
  JGE m6           
                    
  CMP BX,50          
  JGE m5              
                                                                                         
  CMP BX,40               
  JGE m4   
                            
  CMP BX,30
  JGE m3   
  
  CMP BX,20
  JGE m2
     
  CMP BX,10
  JGE m1
  
  JMP fin
  
m1:
  SUB BX,10
  imprime n1
  JMP fin
    
m2:
  SUB BX,20
  imprime n2
  JMP fin
    
m3:
  SUB BX,30
  imprime n3
  JMP fin
m4:
  SUB BX,40
  imprime n4
  JMP fin
m5:
  SUB BX,50
  imprime n5
  JMP fin
m6:
  SUB BX,60
  imprime n6
  JMP fin
m7:
  SUB BX,70
  imprime n7
  JMP fin
m8:
  SUB BX,80
  imprime n8
  JMP fin
m9:
  SUB BX,90
  imprime n9    
   
fin:        

  ADD BL, 30h
  MOV DL, BL
  MOV AH, 02
  INT 21h   
  
  MOV AX, mult
  MOV BL, 2
  
  DIV BL   
  
  MOV DL,AL  ; El resultado de la divicion se encuentra en AL y el residuo en AH 
  MOV AH,02
  INT 21h
  

  MOV AH, 4Ch
  INT 21h
  
END


     
    


    
    
    
    
    