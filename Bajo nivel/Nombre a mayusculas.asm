;Este programa convierte una cadena de minisculas a mayusculas
;cambia tu nombre de letras minusculas utiliza un cilo loop

.model small                                                        ;Define modelo de lenguaje
.stack                                                              ;Define tamaño de la pila
.data                                                               ;seccion de datos
        nombre db 'Mi nombre en minusculas es miguel y en mayusculas es: $ ', 10,13  ;mensaje que se muestra en pantallla
        mayus   db 'miguel'                                         ;variable miguel

.code                                                               ;seccion de codigo
        main proc far                                               ;etiqueta de inicio del programa
                mov ax, @data                                       ;mueve los datos para
                mov ds, ax                                          ;asignarlos al ds
                mov dx, offset nombre                               ;asigna la direccion inicial de nombre
                mov ah, 9                                           ;y con la funcion 9 de la
                int 21h                                             ;int 21h para mostrar en pantalla
                
                mov bx, ds                                          ;moueve el ds hacia el registro bx
                lea bx, mayus                                       ;asigna la direccion de nombre -> bx
                mov cx, 5                                           ;repite loop 5 veces
        repite:                                                     ;etiqueta para realizar las repeticione
                mov al, [bx]                                        ;contenido de la direccion de bx ->ah
                and ah, 11011111 B                                  ;caracter de min a mayor -> ah

                mov dl, ah                                          ;letra mayuscula a dl 
                mov [bl], dl                                        ;contenido de dl -> direccion de bx
                mov ah, 02h                                          ;usa la funcion 2 de la
                int 21h                                             ;int 21h para mostrar en pantalla
                
                inc bx                                              ;incrementa bx a siguente caracter 
                loop repite                                          ;repite nuevamente todo
                mov ax, 4c00h                                       ;regresa el control  al DOS con la funcion 4c
                int 21h                                             ; de la int 21h 
main endp                                                           ;fin de procedimiento main
end main                                                           ; fin del program