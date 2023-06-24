import java.io.*;
import java.net.*;
import java.util.Random;
import java.io.FileWriter;

public class Producer {
    public static void main(String[] args) {

        //Generates integers between 0 (inclusive) and 100 (exclusive
        Random rand = new Random();
        int[] numbers = new int[100];
        for(int i=0; i < numbers.length; i++){
            numbers[i] = rand.nextInt(100);
        }

        try{
            //Save the array to a file
            BufferedWriter writer = new BufferedWriter(new FileWriter("Producer_numbers.txt"));

            //Write each number in the array on a new line in the file
            for(int num : numbers){
                writer.write(Integer.toString(num));
                writer.newLine();
            }
            writer.close();


            //Create a server socket at localhost:9000
            ServerSocket serverSocket = new ServerSocket(8888);

            //Waits for a client to connect
            Socket socket = serverSocket.accept();

            //write formatted textual data
            PrintWriter printWriter = new PrintWriter(socket.getOutputStream(), true);

            //Sending array elements to the client
            for(int nums : numbers){
                  printWriter.println(nums); // Writes each number in the array to the output stream
            }
            socket.close();
            serverSocket.close();

        }catch(IOException e){
            e.printStackTrace();
        }



    }
}