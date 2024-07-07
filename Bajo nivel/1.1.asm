imprime MACRO cadena
    LEA DX, cadena
    MOV AH, 09h
    INT 21h
ENDM

scand MACRO
    MOV AH, 01h
    INT 21h
ENDM

.model small
.data
    contador DB 00h
    mensaje1 DB 13, 10, 'Escribe una mensaje :$'
    mensaje2 DB 13, 10, 'No es una letra, vuelve a intentar $'  
    mensaje3DB 13, 10, 'El numero de letras son : $'
    salto    DB ,13,10,09,'$'
.code
    MOV AX, @data
    MOV DS, AX


    imprime mensaje1
    
ciclo:    
    scand
    CMP AL, 0Dh
    JE exit

    CMP AL, 7Bh
    JAE error

    CMP AL, 40h
    JBE error

    ADD contador, 01h
    JMP ciclo

error:
    imprime mensaje2
    JMP ciclo

exit:  
    imprime mensaje3  
    ADD contador, 30h
    MOV DL, contador 
    MOV AH,02h
    INT 21h
    END
