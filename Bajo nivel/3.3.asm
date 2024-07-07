imprime MACRO cadena
    LEA DX, cadena
    MOV AH, 09h
    INT 21h
ENDM

.model small
.data
    lista    DB 2,3 ,2,1,1,1 
    multi    DW 01 
    divi     DW 00
    par      DB '  El numero es par ',13,10,'$'
    impar    DB '  El numero es impar',13,10,'$' 
    mensaje1 DB 'El resultado de la multiplicacion es : $'
    mensaje2 DB ,13,10,'Multiplicacion entre el numero :  $'
    aux1     DB 00
    signo    DB 00
.code
                         
    MOV AX, @data
    MOV DS, AX
    
    MOV CX, 6
    MOV SI, 0
    
    ciclo:    
    
    MOV AL, lista[SI] 
    MOV aux1, AL 
    MOV AH, 00 
    ADD AL, 30h
    
    MOV DL, AL
    MOV AH, 02
    INT 21h
    
    CMP aux1,1
    JE uno1
    
    sig: 
    TEST aux1, 1
    JNZ imparm
    
    imprime par
    s:      
    
    MOV AH, 00
    MOV AL, aux1
    
    MUL multi  
    MOV multi, AX  ;resultado en la variable   = 
    
    
    INC SI  
    LOOP ciclo                     
    
    JMP exit
    
    imparm:
    imprime impar
    JMP s
    
    uno1:     
    
    CMP signo, 1
    JE quitar
    ADD signo, 1  
    JMP sigue
    quitar:
    SUB signo, 1
    sigue:
    JMP sig
    
      
    
    exit:
    imprime mensaje1 
    
    MOV DX, multi 
    MOV AH, 02h
    INT 21h
    
    imprime mensaje2
    
    MOV AX, multi
    MOV BL, 2
    DIV BL    
    
    MOV divi, AX 
    MOV DX, divi
    MOV AH, 02h
    INT 21h   
 
    
END
