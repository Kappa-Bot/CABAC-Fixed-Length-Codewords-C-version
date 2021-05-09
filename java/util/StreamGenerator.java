import java.io.BufferedOutputStream;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.Random;

public final class StreamGenerator {

  public File dir;
  public String ext = ".txt";

  public StreamGenerator(String dir, String ext) {
     this.dir = new File(dir);
     this.ext = ext;
  }

  public void generate(int filesize, int count) {
    byte[] bytes = new byte[filesize];
    BufferedOutputStream bos = null;
    FileOutputStream fos = null;

    try {
      for (int i = 0; i < count; i++) {
        Random rand = new Random();
        File file = new File(dir, "file_" + i + ext);

        fos = new FileOutputStream(file);
        bos = new BufferedOutputStream(fos);

        rand.nextBytes(bytes); // Sets new data
        bos.write(bytes); // Also overwrites

        bos.close(); fos.flush(); fos.close();
      }
    }  catch (FileNotFoundException fnfe) {
            System.out.println("File not found" + fnfe);
        } catch (IOException ioe) {
            System.out.println("Error while writing to file" + ioe);
        } finally {
          try {
              if (bos != null) {
                  bos.flush();
                  bos.close();
              }
              if (fos != null) {
                  fos.flush();
                  fos.close();
              }
          } catch (Exception e) {
              System.out.println("Error while closing streams" + e);
          }
      }
  }
}
