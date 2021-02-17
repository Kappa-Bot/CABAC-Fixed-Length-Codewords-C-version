public class Test {

    static int filesize = 16 * 1024;
    static int count = 1000;

    static String dir = "../files/";
    static String ext = ".txt";

    public static void main(String[] args) {
      StreamGenerator fileGen = new StreamGenerator(dir, ext);
      fileGen.generate(filesize, count);

      // ByteStream BS = new ByteStream(¿¿¿FileChannel???); // :(
      // ArithmeticCoderFLW AC = new ArithmeticCoderFLW();
      // AC.changeStream(BS);
    }
}
