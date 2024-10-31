

class Segundo {
    public static void main(String[] args) {
        Principal obj = new Principal() {};

        System.out.print("\033[H\033[2J");
        System.out.flush();
        
        obj.setNumero();
        System.out.println(obj.getNumero());
        
        obj.setNombre();   
        System.out.println(obj.getNombre());
    }
}
