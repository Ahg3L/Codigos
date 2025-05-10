;GARCIA GONZALEZ MIGUEL ANGEL       BOLETA : 2022350794

.model small
.data
    msj      DB  'Escribe una cadena : $'
    mERROR   DB  'No es una letra $'
    contador DW 0030h
    uno      DB 1
    salto    DB 10, 13, 9, '$'
    resultado DB 'El número de letras en la cadena es: $'
.code
imprime MACRO cadena
    LEA DX, cadena
    MOV AH, 09h
    INT 21h
ENDM

scand MACRO
    MOV AH, 01
    INT 21h
ENDM

begin:
    MOV AX, @data
    MOV DS, AX

    imprime msj
  

do:
    MOV AL, 0  ; Limpia AL
    scand

    CMP AL, 0DH
    JE fin

    ; Verificar si el carácter es una letra (mayúscula o minúscula)
    CMP AL, 'A'
    JL error
    CMP AL, 'Z'
    JLE es_letra
    CMP AL, 'a'
    JL error
    CMP AL, 'z'
    JG error
es_letra:
    ADD contador,0001h
    JMP do
error:
    imprime mERROR
    JMP do
fin:
    imprime resultado     
    
    MOV DX, contador 
    MOV AH, 02h
    INT 21h

END begin
