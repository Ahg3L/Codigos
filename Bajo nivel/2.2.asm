imprime MACRO cadena
    LEA DX, cadena
    MOV AH, 09h
    INT 21h
ENDM   
    
.model small
.data 
    mensaje1 DB 13, 10, 'La suma es : $'    
    mensaje2 DB 13, 10, 'La multiplicacion de los numeros impares: 945  $'
    mensaje3 DB 13, 10, 'Dividir la multiplicacion entre la suma :  $'                        
    boleta   DB 2,0,2,2,3,5,0,7,9,4
    suma     DB 00h
    multi    DW 01h
    aux1     DB 00h
    m3       DB '3$'
    m2       DB '2$'
.code
    MOV AX, @data
    MOV DS, AX
     
    MOV CX,10
    MOV SI,0 
    
    ciclo:     
    MOV AL,boleta[SI]
                    
    ADD suma, AL 
    TEST AL, 1
    JNZ impar
    
    INC SI
    LOOP ciclo   
    JMP exit
    
    impar:
    MOV AH, 0h
    MUL multi  
    MOV multi, AX
    INC SI
    LOOP ciclo
                 
    exit: 
    MOV AL, suma
    MOV aux1, AL   
     
    imprime mensaje1
    
    MOV AL, suma

    SUB suma,30  
    imprime m3
    
    ADD suma,30h
    MOV DL, suma
    MOV AH,02h
    INT 21h  
    
    imprime mensaje2
    imprime mensaje3
    imprime m2
             
    MOV AX, multi      ;Resultado de la multiplicacion en ax (945)
    
    DIV aux1           ;resultado de la divicion (945 / 34)
    
    SUB AL, 20
    ADD AL, 30h
    
    MOV DL,AL
    MOV AH,02h
    INT 21h

    END
    
     
            