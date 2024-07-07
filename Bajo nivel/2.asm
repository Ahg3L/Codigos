;GARCIA GONZALEZ MIGUEL ANGEL       BOLETA : 2022350794


imprime MACRO cadena
    LEA DX, cadena
    MOV AH, 09h
    INT 21h
 ENDM 
 
scand   MACRO
    MOV AH, 01
    INT 21h
ENDM


.model small
.data
    mensaje  DB 'La suma de mi boleta "2022350794" es : $'      
    3        DB '3$'
    aux      DB 0   
    mensaje3 DB 10,13,'Multiplicacion entre la suma es : $'
    punto    DB '.$'
    resi     Db 0
.code        

    begin: 

    MOV AX, @data
    MOV DS, AX
    
    
    MOV AL, 00h
    
    ADD AL, 2h
    ADD AL, 0h
    ADD AL, 2h
    ADD AL, 2h
    ADD AL, 3h
    ADD AL, 5h
    ADD AL, 0h
    ADD AL, 7h
    ADD AL, 9h
    ADD AL, 4h     

    
    MOV BL, AL
    MOV aux, AL
    imprime mensaje
    imprime 3
    
    SUB BL, 30
    ADD BL, 30h
    
    MOV DL,BL
    MOV AH, 2h
    INT 21h 
    
    MOV AL, 1h
    MOV BL, 3h 
    MUL BL 
    MOV BL, 5h
    MUL BL  
    MOV BL, 7h
    MUL BL
    MOV BL, 9h
    MUL BL 
     
    MOV DX, AX ; EL RESULTADO DE LA MULTIPLICACION SE ENCUENTRA EN AX    
    
    DIV aux    ; EN ESTE PUNTO EL RESULTADO DE LA DIVICION ESTA EN AL Y EL RESIDUO EN DL
    
    MOV BX,AX
    MOV resi,DL 
              
    imprime mensaje3             
     
    ADD BL, 23     
    MOV DL, BL     
    MOV AH, 2      
    INT 21h      
    
    ADD BH, 28  
    MOV DL, BH     
    MOV AH, 2      
    INT 21h 
  
    imprime punto 
    MOV AX,0000h
      
    MOV AL, resi
    MOV BL,10
    DIV BL
    
    MOV resi,AL 
    
    ADD resi,32
    
    MOV DL,resi     
    MOV AH, 2      
    INT 21h 
    ADD resi,6
    MOV DL,resi     
    MOV AH, 2      
    INT 21h 
    
    
    END begin
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    