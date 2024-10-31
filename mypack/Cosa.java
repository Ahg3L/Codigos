package mypack ;
import java.util.Scanner;

class Cosa extends Persona {

    private int valor;
    Scanner precio = new Scanner(System.in);
    
    @Override
    public void setaños(){
        System.out.println("No tiene años");
    }

    public void setValor(){
        valor = precio.nextInt();
    }

    public int getValor(){
        return valor;
    }
    public static void main(String[] args) {
     
        Persona obj = new Persona();  
        Persona cosota = new Cosa();
        Cosa cosita = new Cosa();  

        System.out.print("\033[H\033[2J");
        System.out.flush();
        
        obj.setaños();
        System.out.println(obj.getaño());
        obj.setNombre();   
        System.out.println(obj.getNombre());
        cosita.setValor();
        System.out.println(cosita.getValor());
        cosita.setaños();
        cosota.setaños();
    }
}
