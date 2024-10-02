public class Main {
  public static void main(String[] args){

    int [] lista = {3,1,2,7,8,9,3,12,65,87,32};
    int i = lista.length - 1;  
    
    System.out.print("\033[H\033[2J");
    System.out.flush();
    System.out.println(resul(lista, i));  
  }

  public static int resul(int [] r,int j){
    if (j < 0){
      return 0;
    }else
    return r[j] + resul(r, j-1);
  }
}