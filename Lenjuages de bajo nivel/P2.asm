;GArcia Gonzalez Miguel Angel   2022350794



imprime MACRO cadena    ; macro que ayuda a imprimir los mensajes  
    LEA DX, cadena
    MOV AH, 09h
    INT 21h
ENDM
 
scand MACRO              ; macro que ayuda a capturar los numeros
    MOV AH,01h
    INT 21h
ENDM

.model small
.data 
    mensaje1        DB "Elige la opcion ",13,10,"$"
    mensaje2        DB "1.- suma",13,10,"$" 
    mensaje3        DB "2.- resta",13,10,"$" 
    mensaje4        DB "3.- multiplicacion",13,10,"$"
    mensaje5        DB "4.- divicion ",13,10,"$" 
    mensaje6        DB "5.- salir",13,10,"$"    
    salto           DB "",13,10,"$"
    factorial1      DW 01
    factorial2      DW 01
    resultadosuma   DW 00
    resultadoresta  DW 00
    resultadomulti  DW 01
    resultadodivi   DW 01
.code   

    MOV AX, @data
    MOV DS, AX
    scand
    SUB AL,30h
    MOV CH,00
    MOV CL,AL
    
    ciclo:                     ;inicia el ciclo
    
    MOV AX, factorial1         ; muevo el valor inicial de 1 en el regisstro ax
    MUL CL                     ; multiplico por el valor del resultado por el numero del ciclo
    
    MOV factorial1 , AX        ;muevo el resultado a su variable
 
    LOOP ciclo                 ; incica de nuevo
    aaa
    MOV DX, factorial1         ;imprime resultado
    MOV AH, 02h
    INT 21h
    
    scand
    SUB AL,30h
    MOV CL,AL
    
    ciclo2:                    ;inicia de nuevo para el sugundo valor
    
    MOV AX, factorial2 
    MUL CL 
    
    MOV factorial2, AX         ;resultado del segundo numero
 
    LOOP ciclo2 
     
    MOV DX, factorial2         ;imprime segundo resultado
    MOV AH, 02h
    INT 21h
    
    re: 
     
    imprime salto
    imprime salto              ;imprime los mensajes
    imprime mensaje1
    imprime mensaje2
    imprime mensaje3
    imprime mensaje4
    imprime mensaje5
    imprime mensaje6
    
    
    scand                      ;escanea la opcion y salta a la opcion que es 
    CMP AL,31h 
    JE opc1  
    CMP AL,32h
    JE opc2 
    CMP AL,33h
    JE opc3 
    CMP AL,34h
    JE opc4 
    CMP AL,35h
    JE opc5 
       
     
    opc1: 
        ADD AX, factorial1 
        ADD AX, factorial2  
        MOV resultadosuma,AX
        MOV DX,resultadosuma
        MOV AH, 02h
        INT 21h  
        JMP re 
    
    opc2:
        MOV AX, factorial1 
        CMP AX, factorial2         ;compara cual numero es mayor
        JA mayor 
        MOV AX, factorial2
        SUB AX, factorial1        ;realiza la operacion 
        MOV resultadoresta, AX    ;mueve el resultado a su variable
        
        imprime salto                     ;imprime resultado
        MOV DX, resultadoresta
        MOV AH, 02h
        INT 21h 
        JMP re 
        
        mayor:        
        MOV AX, factorial1
        SUB AX, factorial2    
        MOV resultadoresta, AX 
        
        imprime salto                     ;imprime resultado
        MOV DX, resultadoresta
        MOV AH, 02h
        INT 21h
        
        JMP re
    
    opc3:
    
        MOV AX, factorial1
        MUL factorial2 
        MOV resultadomulti, AX
        
        MOV DX, resultadomulti
        MOV AH,02h
        INT 21h
        
    JMP re
    
    opc4:
    JMP re 
    
    opc5: 

    
  
     
    END  
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
        