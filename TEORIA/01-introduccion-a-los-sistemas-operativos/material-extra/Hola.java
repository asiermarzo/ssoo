/* Hola.java — fuente Java: se compila a bytecode (.class) y lo ejecuta la JVM.
 *   javac Hola.java      -> genera Hola.class (bytecode, no nativo)
 *   java Hola            -> la maquina virtual interpreta / compila JIT el bytecode
 *   javap -c Hola        -> muestra el bytecode desensamblado
 */
public class Hola {
    public static void main(String[] args) {
        System.out.println("Hola desde Java (bytecode + JVM)");
    }
}
