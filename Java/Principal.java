import java.util.Scanner;

abstract class Principal {
    private int  numero;
    private String Nombre;
    
    Scanner entrada = new Scanner (System.in);

    public void setNumero(){
        System.out.println("Escribe un numero : ");
        numero = entrada.nextInt(); 
    }

    public int  getNumero() {
         return numero;
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

