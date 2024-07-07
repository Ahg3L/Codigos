programa .- Escribs un programa que despliega el titulo del programa y los
            numeros hexadecimales correspondientes del 15 al 0 en forma de escalera.

.model small                              ;Define modelo de lenguaje
.stack                                    ;Define tamaño de la pila
.data                                     ;seccion de datos
        mensaje db 'Desplegando los numeros hexadecimales ', 10,13  ;mensaje que se muestra en pantallla
                db 'del 15 al 0'                                    ;mensaje para que aparesca en la siguente linea
                db 13, 10,'$'                                       ;salto de linea
        tabla   db '0123456789ABCDEF'                               ;cadena a desplegar

.code                                                               ;seccion de datos
        begin:                                                      ;etiqueta de inicio
                mov ax, @data                                       ;mueve los datos para
                mov ds, ax                                          ;asignarlos al ds
                mov dx, offset mensaje                              ;asigna la direccion inicial de mensaje
                mov ah, 9                                           ;y con la funcion 9 de la
                int 21h                                             ;int 21h para mostrar en pantalla
                
                mov bx, offset tabla-1                              ;acceso a la tabla equivalente
                mov cx, 16                                          ;asigna el contenido de cl a al

        Itera:                                                      ;etiqueta para realizar las iteraciones
                mov al, cl                                          ;asigna el contenido de cl hacia al
                XLAT                                                ;traduce el numero contenido de al en al

                mov dl, al                                          ;asigna el contenido de al  a dl
                mov, ah, 2                                          ;usa la funcion 2 de la
                int 21h                                             ;int 21h para mostrar en pantalla

                mov dl, 10                                          ;salta una linea dando el efecto de
                int 21h                                             ;escalera con la int 21h 
                loop Itera                                          ;cx debe quedar fuera de itera. si queda dentro 
                                                                    ;de itera se genera  un ciclo infinito 

                mov ax, 4c00h                                       ;regresa el control  al DOS  ncon la funcion 4c
                int 21h                                             ; de la int 21h 
end begin                                                           ; fin del programa
        