package mypack;
import java.util.Scanner;

class Persona {
    protected  int  año;
    protected  String Nombre;
    
    Scanner entrada = new Scanner (System.in);

    public void setaños(){
        System.out.println("Escribe un año : ");
        año = entrada.nextInt(); 
    }

    public int  getaño() {
         return año;
    }

    public void setNombre(){
        entrada.nextLine();
        System.out.println("Escribe un nombre");
        Nombre = entrada.nextLine(); 
    }
    
    public String getNombre(){
        return Nombre;
    }

}

