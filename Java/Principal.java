abstract class Principal {
    public abstract void funcion(int a);
}

class Cprincipal extends Principal {
    private int  numero;

    @Override
    public void funcion(int a) {
        for (int i = a; i > 0; i--) {
            System.out.println("hola");
        }
    }

    public void setNumero(int newNumero){
        numero = newNumero;
    }
    public int  getNumero() {
         return numero;
    }

}

