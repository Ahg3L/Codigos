ESCANEAR    MACRO                       ; Macro ESCANEA (lee datos de entrada)
            MOV AH, 01                  ; Funcion 01h: Leer carácter de entrada
            INT 21h                     ; Interrupcion 21h: DOS servicios
ENDM                                    ; Fin de macro

LPANTALLA   MACRO                       ; Macro LPANTALLA (limpia pantalla)
            MOV AH, 0Fh                 ; Funcion 0Fh: Obtener modo de video actual
            INT 10h                     ; Interrupcion 10h: Servicios de video BIOS
            MOV AH, 0                   ; Funcion 00h: Establecer modo de video
            INT 10h                     ; Interrupcion 10h: Servicios de video BIOS
ENDM                                    ; Fin de macro

IMPRIME     MACRO cadena                ; Macro que imprime mensajes
            LEA DX, cadena              ; Cargar direccion de cadena en DX
            MOV AH, 09                  ; Funcion 09h: Imprimir cadena de caracteres
            INT 21h                     ; Interrupcion 21h: DOS servicios
ENDM                                    ; Fin de macro

RESREG      MACRO                       ; Macro restituye los valores
            SUB AX, AX                  ; Restituir AX (AX = 0)
            SUB BX, BX                  ; Restituir BX (BX = 0)
            SUB CX, CX                  ; Restituir CX (CX = 0)
            SUB DX, DX                  ; Restituir DX (DX = 0)
ENDM                                    ; Fin de macro

RESVAL      MACRO                       ; Macro restituye valores
           
            MOV alto1A, 0               ; Restituir alto1A
            MOV alto2A, 0               ; Restituir alto2A
            MOV bajo1A, 0               ; Restituir bajo1A
            MOV bajo2A, 0               ; Restituir bajo2A
            MOV signoA, 0               ; Restituir signoA 
            
            
            MOV alto1B, 0               ; Restituir alto1B
            MOV alto2B, 0               ; Restituir alto2B
            MOV bajo1B, 0               ; Restituir bajo1B
            MOV bajo2B, 0               ; Restituir bajo2B
            MOV signoB, 0               ; Restituir signoB
            
            MOV carry, 0                ; Restituir carry 
            MOV dive1, 0                      ;Restituye Decena auxiliar   \
            MOV dive2, 0                      ;Restituye Unidad auxiliar   |   
            MOV divd1, 0                      ;Restituye Decima auxiliar   |   
            MOV divd2, 0                      ;Restituye Unidad auxiliar    > para division 
                                ;                            |
            MOV residuo, 0                    ;Restituye residuo           |  
            MOV divisor, 0                    ;Restituye divisor           /
           
                  
            MOV V1, 0                   ; Restituir V1
            MOV V2, 0                   ; Restituir V2
            MOV V3, 0                   ; Restituir V3
            MOV V4, 0                   ; Restituir V4
            MOV V5, 0                   ; Restituir V5
            MOV V6, 0                   ; Restituir V6
            MOV V7, 0                   ; Restituir V7
            MOV V8, 0                   ; Restituir V8
            MOV V9, 0                   ; Restituir V9
            MOV V10, 0                  ; Restituir V10
            MOV V11, 0                  ; Restituir V11
            MOV V12, 0                  ; Restituir V12
            MOV V13, 0                  ; Restituir V13
            MOV V14, 0                  ; Restituir V14
            MOV V15, 0                  ; Restituir V15
            MOV V16, 0                  ; Restituir V16
            MOV V17, 0                  ; Restituir V17
            MOV V18, 0                  ; Restituir V18
            MOV V19, 0                  ; Restituir V19
            MOV V20, 0                  ; Restituir V20
ENDM                                    ; Fin de macro

SUMAR       MACRO SNA,SNB               ; Macro SUMA
            SUB SNA, 30h                ; Ajuste decimal para SNA
            SUB SNB, 30h                ; Ajuste decimal para SNB
            MOV AL, SNA                 ; Mover SNA a AL
            MOV AH, 00
            ADD AL, carry               ; Restituir AH
            ADD AL, SNB                 ; Sumar SNB a AL
                                        ; Sumar acarreo a AL
            AAA                         ; Ajustar resultado a decimal ASCII
            MOV carry, AH               ; Guardar acarreo en carry
            MOV AH, 00                  ; Restituir AH
ENDM                                    ; Fin de macro

RESTAR      MACRO RNA,RNB              ; Macro RESTAR
            MOV AL, RNA                       ;RNA -> AL
            MOV AH, 00                        ;Restituye AH
            SUB AL, RNB                       ;Resta AL de RNB
            AAS                               ;Ajuste 
            ADD AL, carry                     ;Suma acarreo a AL
            MOV carry, AH                     ;Guarda acarreo
            MOV AH, 00 
           
           
                            
ENDM                                    ; Fin de macro

MULTIPLI    MACRO MNA,MNB               ; Macro Multiplicar
            MOV AH, 00                  ; Restituir AH
            MOV AL, MNA                 ; Mover MNA a AL
            SUB AL, 30h                 ; Ajuste decimal para AL
            MOV CH, MNB                 ; Mover MNB a CH
            SUB CH, 30h                 ; Ajuste decimal para CH
            MUL CH                      ; Multiplicar CH x AL
            AAM                         ; Desempacar y ajustar
            MOV carry, AH               ; Guardar acarreo en carry
ENDM                                    ; Fin de macro

AJUSTEMUL   MACRO  V1, V2               ; Macro de ajuste de multiplicacion
            ADD AL, V1                  ; Sumar V1 a AL
            ADD V2, AH                  ; Sumar AH a V2
            MOV AH, 00                  ; Restituir AH
            AAM                         ; Desempacar y ajustar
            MOV V1, AL                  ; Mover AL a V1
            ADD V2, AH                  ; Sumar AH a V2
ENDM                                    ; Fin de macro

SUMAMULC    MACRO SMCA,SMCB             ; Macro de suma de la multiplicacion
            MOV AH, 0                   ; Restituir AH
            MOV AL, SMCA                ; Mover SMCA a AL
            ADD AL, SMCB                ; Sumar SMCA y SMCB
            AAM                         ; Desempacar y ajustar
            MOV DL, AH                  ; Mover AH a DL
            MOV AH, 00                  ; Restituir AH
ENDM                                    ; Fin de macro


ENMASC      MACRO                       ; Macro que enmascara los datos
            OR alto1A, 30h              ; Alto1A a ASCII
            OR alto2A, 30h              ; Alto2A a ASCII
            OR bajo1A, 30h              ; Bajo1A a ASCII
            OR bajo2A, 30h              ; Bajo2A a ASCII
            
            OR alto1B, 30h              ; Alto1B a ASCII
            OR alto2B, 30h              ; Alto2B a ASCII
            OR bajo1B, 30h              ; Bajo1B a ASCII
            OR bajo2B, 30h              ; Bajo2B a ASCII
ENDM                                    ; Fin de macro

anddivi     MACRO                             ;Macro ajuste a Decimal
                                              ;    0000 1111
                                              ;and xxxx xxxx
                                              ;   ___________
                                              
            AND dive1, 0Fh                    ;Ajusta dive1 a Decimal
            AND dive2, 0Fh                    ;Ajusta dive2 a Decimal
            AND divd1, 0Fh                    ;Ajusta divd1 a Decimal
            AND divd2, 0Fh                    ;Ajusta divd2 a Decimal
            ENDM                              ;Fin de macro

moverV      MACRO valor                       ;Macro quita decimal 
            SUB AH, AH                        ;Restituye AH
            MOV AL, dive1                     ;dive1 -> AL
            MUL mil                           ;Convierte a millar
            MOV valor, AX                     ;AX -> valor
            
            SUB AH, AH                        ;Restituye AH
            MOV AL, dive2                     ;dive2 -> AL
            MUL cien                          ;Convierte a centena
            ADD valor, AX                     ;valor + AX
                                              
            SUB AH, AH                        ;Restituye AH
            MOV AL, divd1                     ;divd1 -> AL
            MUL diez                          ;Convierte a decena
            ADD valor, AX                     ;valor + AX
            
            SUB AH, AH                        ;Restituye AH
            MOV AL, divd2                     ;divd2 -> AL
            ADD valor, AX                     ;valor + AX
            ENDM                              ;Fin de macro

enmascd     MACRO                             ;Define macro 
            OR dive1, 30h                     ;Ajusta dive1 a ASCII
            OR dive2, 30h                     ;Ajusta dive2 a ASCII
            OR divd1, 30h                     ;Ajusta divd1 a ASCII
            OR divd2, 30h                     ;Ajusta divd2 a ASCII
            
            OR alto1B, 30h                    ;Ajusta a ASCII
            OR alto2B, 30h                    ;Ajusta a ASCII
            OR bajo1B, 30h                    ;Ajusta a ASCII
            OR bajo2B, 30h                    ;Ajusta a ASCII
            ENDM 


            .model small                ;Define modelo de lenguaje          
            .stack 128                  ;Define seccion de pila y tamano   
            .data                       ;seccion de datos
            
menumsj     DB 13, 10, 09, 'Menu', 13, 10   
            DB 13, 10, ' Calculadora de +, -, *'
            DB 13, 10, ' 1) Suma '
            DB 13, 10, ' 2) Resta '
            DB 13, 10, ' 3) Multiplicacion '  
            DB 13, 10, ' 4) Divison $'
pdatoA      DB 13, 10, 'Introduzca el valor A: $'  
pdatoB      DB 13, 10, 'Introduzca el valor B: $'  
opmenu      DB 13, 10, 'Que operacion desea hacer?: $'  
retmenu     DB 13, 10, 10, ' Deseas continuar?'
            DB 13, 10, '1)Si'
            DB 13, 10, '2)No'
            DB 13, 10, 'Ingrese una opcion: $'  
erro        DB 13, 10, 10, 10, 09, 09, 'OPCION INCORRECTA, INTENTELO DE NUEVO$', 13, 10 
smsj        DB 13, 10, '1.SUMA$' 
rmsj        DB 13, 10, '2.RESTA$' 
mmsj        DB 13, 10, '3.MULTIPLICACION$' 
dmsj        DB 13, 10, '4.DIVISION$'  
resul       DB 13, 10, 10, '  El resultado es:    $'  
despe       DB 13, 10, 09, ' Fin del programa. Adios.$' 
indeter     DB 'ERROR. DIVISION NO VALIDA$'  
negativo    DB '-$'  
punto       DB '.$'  
cup         DB 0

alto1A      DB 00                               ; Decenas de A
alto2A      DB 00                               ; Unidades de A
bajo1A      DB 00                               ; Decimas de A
bajo2A      DB 00                               ; Centésimas de A
signoA      DB 00                               ; Signo para A

alto1B      DB 00                               ; Decenas de B
alto2B      DB 00                               ; Unidades de B
bajo1B      DB 00                               ; Decimas de B
bajo2B      DB 00                               ; Centésimas de B
signoB      DB 00                               ; Signo para B

residuo     DW 0000                             ; Residuo para decimales
diezw       DW 0010                             ; Multiplicador para decimales
mil         DW 1000                             ; Multiplicador para mil
cien        DB 100                              ; Multiplicador para cien
diez        DB 10                               ; Multiplicador para diez
carry       DB 00                               ; Variable para acarreo
auxRD       DW 00                               ; variable auxiliarRD
auxRD2      DW 00                               ; variable auxiliarRD2
auxSD       DW 00                               ; variable auxiliarSD
auxSD2      DW 00                               ; variable auxiliarSD2
auxMD       DW 00                               ; variable auxiliarMD
auxMD2      DW 00                               ; variable auxiliarMD2

dive1       DB 00                             ;Decena auxiliar --
dive2       DB 00                             ;Unidad auxiliar   \ Para
divd1       DB 00                             ;Decima auxiliar   / division
divd2       DB 00                             ;Unidad auxiliar --  
divisor     DW 0000                           ;Residuo decimales



v1          DB 00                               ; Variable auxiliar 1
v2          DB 00                               ; Variable auxiliar 2
v3          DB 00                               ; Variable auxiliar 3
v4          DB 00                               ; Variable auxiliar 4
v5          DB 00                               ; Variable auxiliar 5
v6          DB 00                               ; Variable auxiliar 6
v7          DB 00                               ; Variable auxiliar 7
v8          DB 00                               ; Variable auxiliar 8
v9          DB 00                               ; Variable auxiliar 9
v10         DB 00                               ; Variable auxiliar 10
v11         DB 00                               ; Variable auxiliar 11
v12         DB 00                               ; Variable auxiliar 12
v13         DB 00                               ; Variable auxiliar 13
v14         DB 00                               ; Variable auxiliar 14
v15         DB 00                               ; Variable auxiliar 15
v16         DB 00                               ; Variable auxiliar 16
v17         DB 00                               ; Variable auxiliar 17
v18         DB 00                               ; Variable auxiliar 18
v19         DB 00                               ; Variable auxiliar 19
v20         DB 00                               ; Variable auxiliar 20
aux         DB 00                               ; Variable auxiliar general

.code
        inicia:                                 ; Inicio del programa
            MOV AX, @data                       ; Carga la direccion de los datos en AX
            MOV DS, AX                          ; Carga DS con la direccion de los datos
            
            CALL MENU                           ; Llama a la rutina MENU
            
        fin:                                    ; Fin del programa
            MOV AX, 4Ch                         ; Codigo de salida para DOS
            INT 21h                             ; Llama a la interrupcion de DOS
        
        piddatoA PROC
            RESREG                              ; Restaura registros
        
        enteroA:
            ESCANEAR                            ; Macro ESCANEAR
            
            CMP AL, 2Eh                         ; Comprueba si es punto decimal (.)
            JE decimal1A                        ; Salta a decimal1A si es punto
            
            CMP CX, 3                           ; Comprueba si hay 2 digitos
            JE unidA                            ; Salta a unidA si hay dos digitos
            
            CMP AL, 2Dh                         ; Comprueba si es signo negativo (-)
            JE sigA                             ; Salta a sigA si es signo negativo
            
            CMP AL, 0Dh                         ; Comprueba si es Enter
            JE unidA                            ; Salta a unidA si es Enter
            
            CMP AL, 30H                         ; Comprueba rango de numeros (0-9)
            JB ERROR                            ; Salta a ERROR si no esta en el rango
            CMP AL, 39H                         ; Comprueba rango de numeros (0-9)
            JA ERROR                            ; Salta a ERROR si no esta en el rango
            
            PUSH AX                             ; Guarda el valor en la pila
            INC CX                              ; Incrementa contador CX
            JMP enteroA                         ; Salta a enteroA
            
        decimal1A:
            ESCANEAR                            ; Macro ESCANEAR
            
            CMP AL, 0Dh                         ; Comprueba si es Enter
            JE unidA                            ; Salta a unidA si es Enter
            
            CMP AL, 30H                         ; Comprueba rango de numeros (0-9)
            JB ERROR                            ; Salta a ERROR si no esta en el rango
            CMP AL, 39H                         ; Comprueba rango de numeros (0-9)
            JA ERROR                            ; Salta a ERROR si no esta en el rango
            
            CMP DL, 01                          ; Verifica el contador
            JE decimal2A                        ; Salta a decimal2A si DL = 01
            
            MOV bajo1A, AL                      ; AL --> bajo1A
            INC DL                              ; Incrementa contador DL
            JMP decimal1A                       ; Salta a decimal1A
        
        decimal2A:
            MOV bajo2A, AL                      ; AL --> bajo2A
            JMP unidA                           ; Salta a unidA
        
        sigA:
            INC signoA                          ; Incrementa signoA para determinar negativo
            JMP enteroA                         ; Salta a enteroA
        
        unidA:
            CMP CX, 0                           ; Comprueba CX
            JE pfinA                            ; Salta a pfinA si CX = 0
            
            POP DX                              ; Saca valor de la pila a DX
            MOV alto2A, DL                      ; DL --> alto2A
            
            DEC CX                              ; Decrementa CX
            
            CMP CX, 0                           ; Comprueba CX
            JE pfinA                            ; Salta a pfinA si CX = 0
            
            POP DX                              ; Saca valor de la pila a DX
            MOV alto1A, DL                      ; DL --> alto1A
        
        pfinA:
            RESREG                              ; Restituye registros
            RET                                 ; Retorna
            ENDP                                    ; Fin de piddatoA
                
        piddatoB PROC
            RESREG                              ; Restaura registros
            
        enteroB:
            ESCANEAR                            ; Macro ESCANEAR
            
            CMP AL, 2Eh                         ; Comprueba si es punto decimal (.)
            JE decimal1B                        ; Salta a decimal1B si es punto
            
            CMP CX, 3                           ; Comprueba si hay 2 dígitos
            JE unidB                            ; Salta a unidB si hay dos dígitos
            
            CMP AL, 2Dh                         ; Comprueba si es signo negativo (-)
            JE sigB                             ; Salta a sigB si es signo negativo
            
            CMP AL, 0Dh                         ; Comprueba si es Enter
            JE unidB                            ; Salta a unidB si es Enter
            
            CMP AL, 30H                         ; Comprueba rango de números (0-9)
            JB ERROR                            ; Salta a ERROR si no está en el rango
            CMP AL, 39H                         ; Comprueba rango de números (0-9)
            JA ERROR                            ; Salta a ERROR si no está en el rango
            
            PUSH AX                             ; Guarda el valor en la pila
            INC CX                              ; Incrementa contador CX
            JMP enteroB                         ; Salta a enteroB
            
        decimal1B:
            ESCANEAR                            ; Macro ESCANEAR
            
            CMP AL, 0Dh                         ; Comprueba si es Enter
            JE unidB                            ; Salta a unidB si es Enter
            
            CMP AL, 30H                         ; Comprueba rango de numeros (0-9)
            JB ERROR                            ; Salta a ERROR si no esta en el rango
            CMP AL, 39H                         ; Comprueba rango de numeros (0-9)
            JA ERROR                            ; Salta a ERROR si no esta en el rango
            
            CMP DL, 01                          ; Verifica el contador
            JE decimal2B                        ; Salta a decimal2B si DL = 01
            
            MOV bajo1B, AL                      ; AL --> bajo1B
            INC DL                              ; Incrementa contador DL
            JMP decimal1B                       ; Salta a decimal1B
        
        decimal2B:
            MOV bajo2B, AL                      ; AL --> bajo2B
            JMP unidB                           ; Salta a unidB
        
        sigB:
            INC signoB                          ; Incrementa signoB para determinar negativo
            JMP enteroB                         ; Salta a enteroB
        
        unidB:
            CMP CX, 0                           ; Comprueba CX
            JE pfinB                            ; Salta a pfinB si CX = 0
            
            POP DX                              ; Saca valor de la pila a DX
            MOV alto2B, DL                      ; DL --> alto2B
            
            DEC CX                              ; Decrementa CX
            
            CMP CX, 0                           ; Comprueba CX
            JE pfinB                            ; Salta a pfinB si CX = 0
            
            POP DX                              ; Saca valor de la pila a DX
            MOV alto1B, DL                      ; DL --> alto1B
        
        pfinB:
            RESREG                              ; Restituye registros
            RET                                 ; Retorna
            ENDP                                ; Fin de piddatoB

;-------------------------------------------------------------------------------------------                          
MENU        PROC                                ;incia procedimiento menu
            
            LPANTALLA                           ;Limpia pantalla  
            IMPRIME menumsj                     ;Despliega el menu
            IMPRIME opmenu                      ;Pide la operacion a realizar
            ESCANEAR                            ;inicia macro ESCANEAR 
            
            MOV cup,AL    
            
            CMP AL, 30H                         ;Compara si esta en el rango de numeros 
            JB ERROR                            ;Si es asi manda a error
            CMP AL, 39H                         ;Compara si esta en el rango de numeros 
            JA ERROR                            ;Si es asi manda a error
            
            LPANTALLA                           ;Limpia pantalla
 	        ENMASC			                    ;Enmascara A y B          
                        
            CMP cup, '1'                        ;Compara AL con '1'
            JE  SUMA                            ;Salta a SUMA
            CMP cup, '2'                        ;Compara AL con '2'
            JE  RESTA                           ;Salta a RESTA 
            CMP cup, '3'
            JE  MULTI                               
            CMP cup, '4'                        ;Compara AL con '5'
            JE  DIVI                            ;Salta a SALIR
            CMP cup, '5'                        ;Compara AL con '5'
            JE  SALIR                           ;Salta a SALIR

            JMP ERROR                           ;Salta a error                               
            RET                                 ;Regresa a call MENU                        
MENU        ENDP                                ;Fin de procedimiento          
                             
;------------------------------------------------------------------------------------------------                             
SUMA PROC                                       ; Inia proc de RESTA
            IMPRIME smsj                        ; Imprime "SUMA"
        
            IMPRIME pdatoA                      ; Pide el primer número
            CALL piddatoA                       ; Llama a la macro piddatoA para guardar el valor
        
            IMPRIME pdatoB                      ; Pide el segundo número
            CALL piddatoB                       ; Llama a la macro piddatoB para guardar el valor
        
            IMPRIME resul                       ; Imprime "El resultado es:"
        
            CMP signoA, 01                      ; Comprueba si numA es negativo
            JE SAneg                            ; Salta a SAneg si es negativo
        
            CMP signoB, 01                      ; Comprueba si numB es negativo
            JE SBneg                            ; Salta a SBneg si es negativo
        
            JMP su                              ; Salta a su para continuar
        
        SAneg:                                  ; Si numA es negativo
            CMP signoB, 01                      ; Comprueba si numB es negativo
            JE SABneg                           ; Salta a SABneg si es negativo
        
            JMP RABneg                          ; Salta a RABneg para manejar -xx.xx + xx.xx
        
        SABneg:                                 ; Si numB es negativo
            IMPRIME negativo                    ; Imprime "-"
            JMP su                              ; Salta a su para continuar
        
        SBneg:                                  ; Si numB es negativo
            JMP re                              ; Salta a re para manejar xx.xx + -xx.xx
        
        su:                                     ; Inicia su
            sumar bajo2A, bajo2B                ; Suma (bajo2A + bajo2B)
            PUSH AX                             ; Guarda resultado en la pila
            sumar bajo1A, bajo1B                ; Suma (bajo1A + bajo1B)
            PUSH AX                             ; Guarda resultado en la pila
            sumar alto2A, alto2B                ; Suma (alto2A + alto2B)
            PUSH AX                             ; Guarda resultado en la pila
            sumar alto1A, alto1B                ; Suma (alto1A + alto1B)
            PUSH AX                             ; Guarda resultado en la pila
        
            MOV CX, 02                          ; Inicializa contador antes del punto decimal
        
        sufinp:                                 ; Etiqueta sufinp
            POP DX                              ; Extrae valor de la pila a DX
            ADD DL, 30h                         ; Convierte a caracter ASCII
        
            CMP CL, 01h                         ; Comprueba si es el primer digito decimal
            JE impriS                           ; Salta a impriS si es el primer digito
        
            CMP DL, 30h                         ; Comprueba si es cero
            JE Sceros                           ; Salta a Sceros si es cero
        
        impriS:
            MOV AH, 02                          ; Funcion 02 - Imprimir caracter
            INT 21h                             ; Interrupcion de DOS para imprimir caracter
        Sceros:            
            LOOP sufinp                         ; Bucle para continuar imprimiendo decimales
        
            POP DX                              ; Extrae último valor de la pila
            CMP DL, 00h                         ; Comprueba si es cero
            JE QSceros                          ; Salta a QSceros si es cero
        
            PUSH DX                             ; Guarda DX nuevamente para manejar decimales
            JMP impriPS                         ; Salta a impriPS para imprimir decimales
        
        QSceros:
            MOV auxSD, DX                       ; Guarda DX en auxSD para manejar decimales
            POP DX                              ; Extrae valor de la pila a DX
            MOV auxSD2, DX                      ; Guarda DX en auxSD2 para manejar decimales
        
            CMP DL, 00                          ; Comprueba si es cero
            JE finS                             ; Salta a finS si es cero
        
            PUSH DX                             ; Guarda DX para manejar decimales
            JMP impriPS                         ; Salta a impriPS para imprimir decimales
        
        impriPS:
            IMPRIME punto                       ; Imprime "."
            POP DX                              ; Extrae valor de la pila a DX
            ADD DL, 30h                         ; Convierte a caracter ASCII
        
            MOV AH, 02                          ; Funcion 02 - Imprimir caracter
            INT 21h                             ; Interrupcion de DOS para imprimir caracter
        
            POP DX                              ; Extrae último valor de la pila a DX
            ADD DL, 30h                         ; Convierte a caracter ASCII
            CMP DL, 30h                         ; Comprueba si es cero
            JE finS                             ; Salta a finS si es cero
        
            MOV AH, 02                          ; Funcion 02 - Imprimir caracter
            INT 21h                             ; Interrupcion de DOS para imprimir caracter
        
        finS:
            RESVAL                              ; Restablece variables
            RESREG                              ; Restablece registros
        
            IMPRIME retmenu                     ; Imprime mensaje de retorno al menú
            ESCANEAR                            ; Espera la opción del usuario
        
            CMP AL, '1'                         ; Comprueba si la opcion es '1'
            JE MENU                             ; Salta a MENU si es '1'
        
            CMP AL, '2'                         ; Comprueba si la opcion es '2'
            JE SALIR                            ; Salta a SALIR si es '2'
        
            JMP ERROR                           ; Salta a ERROR si la opción no es valida
SUMA ENDP                                       ; Fin de la rutina SUMA


;--------------------------------------------------------------------------------------------------         
 RESTA       PROC                              ;Inicia procedimiento RESTA
            imprime rmsj                      ;Imprime mensaje 'RESTA'.     
            imprime pdatoA                    ;Pide primer numero 
            CALL piddatoA                     ;Macro guarda valores                 
            imprime pdatoB                    ;Pide segundo numero 
            CALL piddatoB
                                 ;Macro guarda valores     
            imprime resul 
                                ;imprime resul.  
            CMP signoA, 01                    ;Analiza si numA es negativo.
            JE  RAneg                         ;Si lo es, salta a RAneg.
            CMP signoB, 01                    ;Analiza si numB es negativo.
            JE  RBneg                         ;Si lo es, salta a RBneg.   
ree:                                          ;Etiqueta ree
	        JMP re                            ;Salta a re
RAneg:                                        ;Inicia RAneg
            CMP signoB, 01                    ;Analiza si numB es negativo.
            JE  SABneg                        ;Si lo es, salta SABneg 
            JMP RABneg                        ;Salta a RABneg
SABnege:                                      ;Etiqueta SABnege
            JMP SABneg                        ;Salta a SABneg
RBneg:                                        ;Etiqueta RBneg
            JMP re                            ;Salta a re
RABneg:                                       ;Etiqueta RABneg
            MOV DL, alto1B                    ;Asigna alto1b a DL.
            CMP alto1A, DL                    ;Compara alto1A con alto1B.
            JA  mayorA                        ;Salta a resta mayorA.
            JB  mayorBe                       ;Salta a resta mayorB.
            MOV DL, alto2B                    ;Asigna alto2B a DL.
            CMP alto2A, DL                    ;Compara alto1A con alto1B.
            JA  mayorA                        ;Salta a resta mayorA.
            JB  mayorB                        ;Salta a resta mayorB.
            MOV DL, bajo1B                    ;Asigna bajo1B a DL.
            CMP bajo1A, DL                    ;Compara bajo1A con bajo1B.
            JA  mayorA                        ;Salta a resta mayorA.
            JB  mayorB                        ;Salta a resta mayorB.
	        MOV DL, bajo2B                    ;Asigna bajo2B a DL
	        CMP bajo2A, DL                    ;Compara DL con bajo2A
            JA  mayorA                        ;Salta a resta mayorA.
            JB  mayorB                        ;Salta a resta mayorB.           
            JMP mayorB                        ;Salta a procedimeinto mayorB.
mayorBe:                                      ;Etiqueta mayorBe
	        JMP mayorB                        ;Salta a mayorB
mayorA:                                       ;Inicia operacion A-B. 
            imprime negativo                  ;imprime signo "-". 
            restar bajo2A, bajo2B             ;inicia la macro restar.
            PUSH AX                           ;Resultado a pila.
            restar bajo1A, bajo1B             ;inicia la macro restar.
            PUSH AX                           ;Resultado a pila.
            restar alto2A, alto2B             ;inicia la macro restar.
            PUSH AX                           ;Resultado a pila.
            restar alto1A, alto1B             ;inicia la macro restar.
            PUSH AX                           ;Resultado a pila
            JMP resf                          ;Salta a resf.
mayorB:                                       ;inicia resta B-A.
            restar bajo2B, bajo2A             ;inicia macro restar.
            PUSH AX                           ;Resultado a pila.
            restar bajo1B, bajo1A             ;inicia macro restar.
            PUSH AX                           ;Resultado a pila.
            restar alto2B, alto2A             ;inicia macro restar.
            PUSH AX                           ;Resultado a pila.
            restar alto1B, alto1A             ;inicia macro restar.
            PUSH AX                           ;Resultado a pila.
            JMP resf                          ;salta a resf.
re:                                           ;Etiqueta A>B o B>A.
	        MOV DL, alto1B                    ;Asigna alto1B a DL.   
            CMP alto1A, DL                    ;compara Alto1A con Alto1B.
            JA  mayorAA                       ;Salta a resta mayorAA. 
            JB  mayorBB                       ;Salta a resta mayorBB    
            MOV DL, alto2B                    ;Asigna alto2B a DL.
            CMP alto2A, DL                    ;Compara alto2B con alto2A.
            JA  mayorAA                       ;salta a resta mayorAA.
            JB  mayorBB                       ;salta a resta mayorBB.
            MOV DL, bajo1B                    ;Asigna bajo1B a DL.
            CMP bajo1A, DL                    ;Compara bajo1A con bajo1B.
            JA  mayorAA                       ;Salta a resta mayor AA.
            JB  mayorBB                       ;Salta a resta mayor BB.
	        JMP MAYORAA                       ;Salta a resta mayorAA.   
mayorAA:                                      ;Etiqueta mayorAA.
            restar bajo2A, bajo2B             ;Inicia macro restar.
            PUSH AX                           ;Resultado a pila.
            restar bajo1A, bajo1B             ;Inicia macro restar.
            PUSH AX                           ;Resultado a pila.
            restar alto2A, alto2B             ;Inicia macro restar.
            PUSH AX                           ;Resultado a pila.
            restar alto1A, alto1B             ;Inicia macro restar.
            PUSH AX                           ;Resultado a pila.
            JMP resf                          ;Salta a resf. 
mayorBB:                                      ;Etiqueta mayorBB.
	        imprime negativo                  ;Imprime "-".
            restar bajo2B, bajo2A             ;Inicia macro restar.
            PUSH AX                           ;Resultado a pila.
            restar bajo1B, bajo1A             ;Inicia macro restar.
            PUSH AX                           ;Resultado a pila.
            restar alto2B, alto2A             ;Inicia macro restar.
            PUSH AX                           ;Resultado a pila.
            restar alto1B, alto1A             ;Inicia macro restar.
            PUSH AX                           ;Resultado a pila.
            JMP resf                          ;Salta a resf
resf:                                         ;Etiqueta resf
            MOV  CX, 02                       ;Contador de variables
refinp:                                       ;Etiqueta refinp
            POP DX                            ;Extrae valor a DX 
            ADD DL, 30h                       ;Restituye DL
            CMP CL, 01h                       ;Compara CL con 01h 
            JE impriR                         ;Salta a la etiqueta impriR             
            CMP DL, 30h                       ;Compara DL con 30h
            JE Rceros                         ;Salta a la etiqueta Rceros
impriR:                                       ;Etiqueta impriR
            MOV AH, 02                        ;Imprime un caracter
            INT 21h                           ;Con la INT 21H
Rceros:                                       ;Etiqueta Rceros
            LOOP refinp                       ;Decrementa el registro CX y salta a refinp  
            POP DX                            ;Extrae el valor de la pila y almacena en DX
            CMP DL, 00                        ;Compara DL con 00
            JE QRceros                        ;Si DL es igual a 00, salta a QRceros
            PUSH DX                           ;Empuja el valor de DX de nuevo a la pila
            JMP impriPR                       ;Salta a la etiqueta impriPR
QRceros:                                      ;Etiqueta QRceros
            MOV auxRD, DX                     ;Mueve el valor de DX a la variable auxRD
            POP DX                            ;Extrae el siguiente valor de la pila
            MOV auxRD2, DX                    ;Mueve el valor de DX a la variable auxRD2
            CMP DL, 00                        ;Compara el contenido del registro DL con 00
            JE finD                           ;Si DL es igual a 00, salta a la etiqueta finD
            MOV AX, auxRD2                    ;Mueve el valor de auxRD2 al registro AX
            PUSH AX                           ;Empuja el valor de AX a la pila
            MOV AX, auxRD                     ;Mueve el valor de auxRD al registro AX
            PUSH AX                           ;Empuja el valor de AX a la pila
impriPR:                                      ;Etiqueta impriPR
            imprime punto                     ;Imprime punto  XX.
            POP DX                            ;Imprime decimales .XX
            ADD DL, 30h                       ;Restituye DL            
            MOV AH, 02                        ;Funcion 02
            INT 21h                           ;Imprime valor
            POP DX                            ;Saca el valor de la pila
            CMP DL, 00                        ;Compara DL con 00
            JE finD                           ;Si DL es 00, salta a la etiqueta finD
            ADD DL, 30h                       ;Agrega 30h a DL
            MOV AH, 02                        ;Funcion 02
            INT 21h                           ;Imprime valor
finD:                                         ;Etiqueta finD
            RESVAL                             ;Restituye variables
            RESREG                             ;Restituye registros            
            imprime retmenu                   ;Imprime retmenu
            ESCANEAR                             ;Pide dato de opcion
            CMP AL, '1'                       ;Compara AL con '1'
            JE  MENUe2                        ;Salta a MENU
            CMP AL, '2'                       ;Compara AL con '2'
            JE  SALIRe3                       ;Salta a SALIR
menue2:                                       ;etiqueta menu2
		    JMP MENU                          ;salta a menu
salire3:                                      ;etiqueta 
		    JMP SALIR                         ;SALTA A LA ETIQUETA SALIR
RESTA       ENDP                              ;Finaliza procedimiento RESTA   
;--------------------------------------------------------------------------------------

MULTI PROC
            IMPRIME mmsj                        ; IMPRIME mensaje 'MULTIPLICACION'
            
            IMPRIME pdatoA                      ; Pide primer numero 
            CALL piddatoA                       ; Macro guarda valores 
        
            IMPRIME pdatoB                      ; Pide segundo numero 
            CALL piddatoB                       ; Macro guarda valores 
                    
            IMPRIME resul                       ; IMPRIME resultado
            
            RESREG                              ; Restituye registros 
            ENMASC                              ; Enmascara el resultado
            
            CMP signoA, 01                      ; Verifica si numA es negativo
            JE MAneg                            ; Si lo es, salta a MAneg 
            CMP signoB, 01                      ; Verifica si numB es negativo
            JE Mresulneg                        ; Si lo es, salta a Mresulneg
            
            JMP mu                              ; Salta a multiplicacion
        
        MAneg:
            CMP signoB, 01                      ; Verifica si numB es negativo
            JE mu                               ; Si lo es, salta a multiplicacion
            JMP Mresulneg                       ; Salta a imprimir signo '-'
        
        Mresulneg:
            IMPRIME negativo                    ; IMPRIME '-'
            JMP mu                              ; Salta a multiplicacion
        
        mu:                                     ; Etiqueta MULTIPLI 
            MULTIPLI bajo2a, bajo2b             ; Multiplica bajo2a por bajo2b
            MOV v5, AL                          ; Guarda el resultado bajo en v5 
            ADD v4, AH                          ; Suma el resultado alto en d (multiplica paso a paso)
            
            MULTIPLI bajo1a, bajo2b             ; Multiplica bajo1a por bajo2b
            AJUSTEMUL v4, v3                    ; Ajusta el acarreo de d en c
            
            MULTIPLI alto2a, bajo2b             ; Multiplica alto2a por bajo2b
            AJUSTEMUL v3, v2                    ; Ajusta el acarreo de c en b
            
            MULTIPLI alto1a, bajo2b             ; Multiplica alto1a por bajo2b
            AJUSTEMUL v2, v1                    ; Ajusta el acarreo de b en a
            
            MULTIPLI bajo2a, bajo1b             ; Multiplica bajo2a por bajo1b
            MOV v10, AL                         ; Guarda el resultado bajo en j
            ADD v9, AH                          ; Suma el resultado alto en i
            
            MULTIPLI bajo1a, bajo1b             ; Multiplica bajo1a por bajo1b
            AJUSTEMUL v9, v8                    ; Ajusta el acarreo de i en h
            
            MULTIPLI alto2a, bajo1b             ; Multiplica alto2a por bajo1b
            AJUSTEMUL v8, v7                    ; Ajusta el acarreo de h en g
            
            MULTIPLI alto1a, bajo1b             ; Multiplica alto1a por bajo1b
            AJUSTEMUL v7, v6                    ; Ajusta el acarreo de g en f
            
            MULTIPLI bajo2a, alto2b             ; Multiplica bajo2a por alto2b
            MOV v15, AL                         ; Guarda el resultado bajo en o
            ADD v14, AH                         ; Suma el resultado alto en n
            
            MULTIPLI bajo1a, alto2b             ; Multiplica bajo1a por alto2b
            AJUSTEMUL v14, v13                  ; Ajusta el acarreo de n en m
            
            MULTIPLI alto2a, alto2b             ; Multiplica alto2a por alto2b
            AJUSTEMUL v13, v12                  ; Ajusta el acarreo de m en l
            
            MULTIPLI alto1a, alto2b             ; Multiplica alto1a por alto2b
            AJUSTEMUL v12, v11                  ; Ajusta el acarreo de l en k
            
            MULTIPLI bajo2a, alto1b             ; Multiplica bajo2a por alto1b
            MOV v20, AL                         ; Guarda el resultado bajo en t
            ADD v19, AH                         ; Suma el resultado alto en s
            
            MULTIPLI bajo1a, alto1b             ; Multiplica bajo1a por alto1b
            AJUSTEMUL v19, v18                  ; Ajusta el acarreo de s en r
            
            MULTIPLI alto2a, alto1b             ; Multiplica alto2a por alto1b
            AJUSTEMUL v18, v17                  ; Ajusta el acarreo de r en q
            
            MULTIPLI alto1a, alto1b             ; Multiplica alto1a por alto1b
            AJUSTEMUL v17, v16                  ; Ajusta el acarreo de q en p
            
            MOV AH, 00                          ; Limpia AH
            MOV AL, v5                          ; Guarda el resultado en AX
            PUSH AX                             ; Empuja AX a la pila
            
            SUMAMULC v4, v10                    ; Suma con acarreo D y J
            PUSH AX                             ; Empuja AX a la pila
            
            ADD v3, DL                          ; Suma DL con c
            MOV CL, v3                          ; Mueve c a CL
            ADD v9, CL                          ; Suma CL con i
            SUMAMULC v9, v15                    ; Suma con acarreo i y o
            PUSH AX                             ; Empuja AX a la pila
            
            ADD v2, DL                          ; Suma DL con b
            MOV CL, v2                          ; Mueve b a CL
            ADD v8, CL                          ; Suma CL con h
            MOV CL, v8                          ; Mueve h a CL
            ADD v14, CL                         ; Suma CL con n
            SUMAMULC v14, v20                   ; Suma con acarreo n y t
            PUSH AX                             ; Empuja AX a la pila
            
            ADD v1, DL                          ; Suma DL con a
            MOV CL, v1                          ; Mueve a a CL
            ADD v7, CL                          ; Suma CL con g
            MOV CL, v7                          ; Mueve g a CL
            ADD v13, CL                         ; Suma CL con m
            SUMAMULC v13, v19                   ; Suma con acarreo m y s
            PUSH AX                             ; Empuja AX a la pila
            
            ADD v6, DL                          ; Suma DL con f
            MOV CL, v6                          ; Mueve f a CL
            ADD v12, CL                         ; Suma CL con l
            SUMAMULC v12, v18                   ; Suma con acarreo l y r
            PUSH AX                             ; Empuja AX a la pila
            
            ADD DL, DL                          ; Suma DL con DL
            SUMAMULC v11, v17                   ; Suma con acarreo k y q
            PUSH AX                             ; Empuja AX a la pila
            
            ADD v16, DL                         ; Suma DL con p
            MOV AH, 00                          ; Limpia AH
            MOV AL, v16                         ; Mueve p a AL
            PUSH AX                             ; Empuja AX a la pila
        
            POP DX                              ; Extrae de la pila a DX
            POP DX                              ; Extrae de la pila a DX 
            POP DX
            ADD DL, 30h                         ; Convierte DL a ASCII   
            CMP DL, '0'
            JE mcero
            
            MOV AH, 02                          ; Función para imprimir carácter
            INT 21h                             ; Interrupción DOS para imprimir
        
        mcero:            
            POP DX                              ; Extrae de la pila a DX     
            ADD DL, 30h                         ; Convierte DL a ASCII     
            MOV AH, 02                          ; Función para imprimir carácter
            INT 21h                             ; Interrupción DOS para imprimir
            
            POP DX                              ; Extrae de la pila a DX
            MOV auxMD, DX                       ; Guarda DX en auxMD
            POP DX                              ; Extrae de la pila a DX nuevamente
            MOV auxMD2, DX                      ; Guarda DX en auxMD2
                    
            CMP auxMD, 0                        ; Compara auxMD con 0
            JE mcero2                           ; Si es igual a 0, salta a mcero2
            JMP m                               ; Si no, salta a m
                    
        mcero2:
            CMP auxMD2, 0                       ; Compara auxMD2 con 0
            JE finmd                            ; Si es igual a 0, salta a finmd
                    
        m:                                      ; Etiqueta m
            PUSH auxMD2                         ; Empuja auxMD2 a la pila
            PUSH auxMD                          ; Empuja auxMD a la pila
                    
            MOV DL, 2Eh                         ; Mueve '.' (punto decimal) a DL 
            MOV AH, 02                          ; Función para imprimir carácter
            INT 21h                             ; Interrupción DOS para imprimir
                    
            POP DX                              ; Extrae de la pila a DX 
            ADD DL, 30h                         ; Convierte DL a ASCII 
            MOV AH, 02                          ; Función para imprimir carácter
            INT 21h                             ; Interrupción DOS para imprimir
                    
            POP DX                              ; Extrae de la pila a DX 
            ADD DL, 30h                         ; Convierte DL a ASCII 
            MOV AH, 02                          ; Función para imprimir carácter
            INT 21h                             ; Interrupción DOS para imprimir 
            
        finmd:                                  ; Etiqueta finmd
        
            RESVAL                              ; Restaura los valores de los registros
            RESREG                              ; Restituye registros
            IMPRIME retmenu                     ; IMPRIME retorno al menú
            ESCANEAR                            ; Escanea entrada del usuario
        
            CMP AL, '1'                         ; Compara AL con '1'
            JE menu                             ; Si es igual, salta a menu
            CMP AL, '2'                         ; Compara AL con '2'
            JE salir                            ; Si es igual, salta a salir
            JMP ERROR                           ; Salta a ERROR si no se cumple ninguna condición

MULTI ENDP                                      ; Finaliza el procedimiento MULTI
;---------------------------------------------------------------------------------------------------                                   

DIVI        PROC                              ;Inicia procedimiento DIVI
            IMPRIME dmsj                      ;IMPRIME mensaje 'MULTIPLICACION'
            IMPRIME pdatoA                    ;Pide primer numero 
            CALL piddatoA                     ;Macro guarda valores 
            IMPRIME pdatoB                    ;Pide segundo numero 
            CALL piddatoB                     ;Macro guarda valores
            IMPRIME resul                     ;IMPRIME resultado
            RESREG                             ;Restituye registros 
            ENMASC                           ;Enmascara
            MOV AL, alto1B                    ;Mueve el valor de alto1B al registro AL
            MOV dive1, AL                     ;Almacena el valor de AL en dive1
            MOV AL, alto2B                    ;Mueve el valor de alto2B al registro AL
            MOV dive2, AL                     ;Almacena el valor de AL en dive2
            MOV AL, bajo1B                    ;Mueve el valor de bajo1B al registro AL
            MOV divd1, AL                     ;Almacena el valor de AL en divd1
            MOV AL, bajo2B                    ;Mueve el valor de bajo2B al registro AL
            MOV divd2, AL                     ;Almacena el valor de AL en divd2
            anddivi                           ;Convierte a decimal
            moverV divisor                    ;xx.xx --> xxxx -> divisor
            CMP divisor, 0                    ;Compara si divisor = 0
            JE diviindet                      ;Salta a diviindet
            JMP diviinic                      ;Salta inicio de la division
diviindet:                                    ;Etiqueta diviindet
            JMP divifin                       ;Salta fin de la division
diviinic:                                     ;Inicia division 
            CMP signoA, 01                    ;Verifica si numA es negativo
            JE  DAneg                         ;Si lo es, salta a DAneg 
            CMP signoB, 01                    ;Verifica si numB es negativo
            JE  Dresulneg                     ;Si lo es, salta a Dresulneg
            JMP indiv                         ;Salta a division      
DAneg:                                        ;Etiqueta DAneg
            CMP signoB, 01                    ;Verifica si numB es negativo
            JE  indiv                         ;Si lo es, salta a division
            JMP Dresulneg                     ;Salta a etiqueta Dresulneg
Dresulneg:                                    ;Etiqueta Dresulneg
            IMPRIME negativo                  ;Imprime el signo negativo
            JMP indiv                         ;Salta a la etiqueta indiv
indiv:                                        ;Etiqueta indiv
            SUB CX, CX                        ;Restituye el registro CX
            MOV AL, alto1A                    ;Mueve el valor de alto1A al registro AL
            MOV dive1, AL                     ;Almacena el valor de AL en dive1
            MOV AL, alto2A                    ;Mueve el valor de alto2A al registro AL
            MOV dive2, AL                     ;Almacena el valor de AL en dive2
            MOV AL, bajo1A                    ;Mueve el valor de bajo1A al registro AL
            MOV divd1, AL                     ;Almacena el valor de AL en divd1
            MOV AL, bajo2A                    ;Mueve el valor de bajo2A al registro AL
            MOV divd2,AL                      ;Almacena el valor de AL en divd2
restadiv:                                     ;Etiqueta restadiv
            ENMASC                           ;Variables aux a ASCII 
            MOV BL, alto1B                    ;Asigna BL a bajo1B
            CMP dive1, BL                     ;Compara dive1 con BL
            JA  restad                        ;Salta a restad
            JB  menore                        ;Salta a menore
            MOV BL, alto2B                    ;Asigna BL a bajo2B
            CMP dive2, BL                     ;Compara dive2 con BL
            JA  restad                        ;Salta a etiqueta restad
            JB  menor                         ;Si = salta a menor
            MOV BL, bajo1B                    ;No = salta a restad
            CMP divd1, BL                     ;Compara divd1 con BL
            JA  restad                        ;Salta a etiqueta restad
            JB  menor                         ;Salta a etiqueta menor
            MOV BL, bajo2B                    ;Asigna bajo2B a BL
            CMP divd2, BL                     ;Compara divd2 con BL
            JA  restad                        ;Salta a etiqueta restad
            JB  menor                         ;Salta a etiqueta menor
            JMP restad                        ;Salta a etiqueta restad
menore:                                       ;Etiqueta menore
            JMP menor                         ;Salta a etiqueta menor
restad:                                       ;Division por restas
            restar divd2, bajo2B              ;Dividendo - Divisor
            MOV divd2, AL                     ;Dividendo - Divisor
            restar divd1, bajo1B              ;Dividendo - Divisor
            MOV divd1, AL                     ;Dividendo - Divisor
            restar dive2, alto2B              ;Dividendo - Divisor
            MOV dive2, AL                     ;Dividendo - Divisor
            restar dive1, alto1B              ;Dividendo - Divisor
            MOV dive1, AL                     ;Dividendo - Divisor
            
            INC CX                            ;Incrementa cociente
            JMP restadiv                      ;Salta a restadiv
menor:                                        ;Etiqueta menor
            MOV AX, CX                        ;Cociente -> AX
            AAM                               ;Desempaca Cociente (0X | 0X)
            ADD AX, 3030h                     ;Convierte a ASCII
            MOV DH, 0                         ;Restituye DH
            MOV DL, AH                        ;Decena -> DL
            PUSH DX                           ;Decena a pila
            MOV DL, AL                        ;Unidad -> DL
            PUSH DX                           ;Unidad a pila
            anddivi                           ;Etiqueta anddivi
            moverV residuo                    ;xx.xx --> xxxx -> residuo                 
                                              ;Division para primer decimal
            MOV AX, residuo                   ;residuo -> AX
            MUL diezw                         ;residuo x 10
            SUB DX, DX                        ;sub DX, DX
            DIV divisor                       ;residuo / divisor
            ADD AL, 30h                       ;primer decimal --> ASCII
            PUSH AX                           ;primer decimal a pila            
                                              ;Division para segundo decimal
            MOV AX, DX                        ;residuo -> AX
            SUB DX, DX                        ;restituye DX
            MUL diezw                         ;residuo x 10
            SUB DX, DX                        ;restituye DX
            DIV divisor                       ;residuo / divisor
            ADD AL, 30h                       ;segundo decimal --> ASCII
            PUSH AX                           ;segundo decimaal a pila   
            POP AX                            ;Extrae de la pila
            MOV divd2, AL                     ;Segundo decimal -> aux
            POP AX                            ;Extrae de la pila
            MOV divd1, AL                     ;Primer decimal -> aux
            POP AX                            ;Extrae de la pila
            MOV dive2, AL                     ;Unidad -> aux
            POP AX                            ;Extrae de la pila
            MOV dive1, AL                     ;Decena -> aux
            MOV AH, 02                        ;Funcion 02 -> AH 
            MOV DL, dive1                     ;dive1 -> DL
            CMP DL,30h                        ;Compara DL con 30h
            JZ  divdig2                       ;Si es 0, salta a etiqueta divdig2
            INT 21h                           ;Imprime DL
divdig2:                                      ;Etiqueta divdig2
            MOV DL, dive2                     ;dive2 -> DL
            INT 21h                           ;Imprime DL
            MOV DL, divd1                     ;Asigna divd1 a DL
            CMP DL,30h                        ;Compara DL con 30h
            JZ  divifin                       ;Si es 0, salta a etiqueta divifin
            IMPRIME punto                     ;Imprime '.'
            MOV AH, 02                        ;Funcion 02 -> AH
            MOV DL, divd1                     ;divd1 -> DL
            INT 21h                           ;Imprime DL
            MOV DL, divd2                     ;divd2 -> DL
            CMP DL,30h                        ;Compara DL con 30h
            JZ  divifin                       ;Si es 0, salta a etiqueta divifin 
            INT 21h                           ;Interrupcion 21h
         
divifin:                                      ;Etiqueta divifin
            RESREG                             ;Restituye registros
            RESVAL                             ;Restituye variables
            IMPRIME retmenu                   ;Imprime retmenu
            ESCANEAR                             ;Pide dato de opcion
            CMP AL, '1'                       ;Compara AL con '1'
            JE  MENUe4                        ;Salta a MENU
            CMP AL, '2'                       ;Compara AL con '2'
            JE  SALIRe4                       ;Salta a SALIR 
menue4:                                       ;Etiqueta menue4
		    JMP MENU                          ;Salta a etiqueta MENU
salire4:                                      ;Etiqueta salire4
		    JMP SALIR                         ;Salta a etiqueta SALIR                        
DIVI        ENDP                              ;Fin del procedimiento DIVI
;-------------------------------------------------------------------------------

           
SALIR       PROC near                           ; Inicio de proc salir
    
            LPANTALLA                           ; Limpia pantalla
            IMPRIME despe                       ; IMPRIME despedida
            ESCANEAR                            ; Hace una pausa

            JMP fin                             ; Salta a fin
            
SALIR       ENDP                                ; Finaliza procedimiento SALIR
;-----------------------------------------------------------------------------------            
                       
ERROR       PROC                                ; Inicio de proc ERROR
    
            LPANTALLA                           ; Limpia pantalla
            IMPRIME erro                        ; Avisa de un error
            ESCANEAR                            ; Hace una pausa
            RESVAL                              ; Restituye todas las variables
            JMP MENU                            ; Salta a MENU
            
ERROR       ENDP                                ; Finaliza procedimiento ERROR
               
            END inicia                          ; Finaliza el programa y define el punto de inicio
 
            
            