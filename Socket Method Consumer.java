import java.io.*;
import java.net.*;
import java.util.Random;
import java.io.FileWriter;

public class Consumer {
    public static void main(String[] args) {

        int [] receivedNumbers = new int[100];


        try{
            Socket socket = new Socket("localhost",8888);

            //Read from this socket.
            BufferedReader reader = new BufferedReader(new InputStreamReader(socket.getInputStream()));

            String line;
            int index = 0;

            //write to the local array
            while((line = reader.readLine()) != null && index < receivedNumbers.length){
                receivedNumbers[index++] = Integer.parseInt(line);
            }


            //Store each line in a txt file
            BufferedWriter writer = new BufferedWriter(new FileWriter("Consumer_numbers.txt"));
            for(int num : receivedNumbers){
                writer.write(Integer.toString(num));
                writer.newLine();
            }
            writer.close();
//
//
            //Read from Consumer_numbers.txt, and print each line on screen
            BufferedReader reader2 = new BufferedReader(new FileReader("Consumer_numbers.txt"));
            String line2;
            while(( line2 = reader2.readLine()) !=null){
                System.out.println(line2);
            }

            socket.close();
//



        }catch(IOException e){
            e.printStackTrace();
        }
    }}