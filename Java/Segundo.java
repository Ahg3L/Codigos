class Segundo {
    public static void main(String[] args) {
        Cprincipal obj = new Cprincipal();

        System.out.print("\033[H\033[2J");
        System.out.flush();

        obj.setNumero(1);
        System.out.println(obj.getNumero());
        obj.funcion(6 );

    }
}
