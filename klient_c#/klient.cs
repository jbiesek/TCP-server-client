using System;
using System.Threading;
using System.Text;
using System.Net;
using System.Net.Sockets;
using System.IO;
using System.Collections.Generic;
using System.Linq;

public class Client
{

    class Program
    {
        static void Main(string[] args)
        {
        	Console.Write("Podaj numer ip: ");
        	string ip = Console.ReadLine();
			Console.Write("Podaj numer portu: ");
        	int port = Convert.ToInt32(Console.ReadLine());
            TcpClient client = new TcpClient(ip, port);
            NetworkStream nwStream = client.GetStream();
            Console.Write("Lacze sie z ");
            Console.Write(ip);
            Console.Write(":");
            Console.WriteLine(port);
            Console.WriteLine("Polaczenie nawiazane!");
            Console.Write("Podaj login: ");
			String login = Console.ReadLine();
			Console.Write("Podaj hasło: ");
			ConsoleColor origFG = Console.ForegroundColor;
			Console.ForegroundColor = ConsoleColor.Black;
			string password = Console.ReadLine();
			Console.ForegroundColor= origFG;
			String loginpass = login + " " + password;
        	byte[] sendBuffer = Encoding.ASCII.GetBytes(loginpass);
            nwStream.Write(sendBuffer, 0, sendBuffer.Length);
            byte[] bytesToRead = new byte[30];
            int bytesRead = nwStream.Read(bytesToRead, 0, 30);
            char[] message = new char[30];
            for (int i = 0; i < bytesRead; i++)
			{
   				message[i] = Convert.ToChar(bytesToRead[i]);
			}
            Console.Write(Encoding.ASCII.GetString(bytesToRead, 0, bytesRead));
            if (message[0] == 'B') {
            	Environment.Exit(0);
            }
            bytesToRead = new byte[1024];
            bytesRead = nwStream.Read(bytesToRead, 0, 1024);
            Console.Write(Encoding.ASCII.GetString(bytesToRead, 0, bytesRead));
            char[] bufor = new char[1024];
            for (int i = 0; i < bytesRead; i++)
			{
   				bufor[i] = Convert.ToChar(bytesToRead[i]);
			}
            Console.Write("Wybierz plik z listy: ");
            String choice = Console.ReadLine();
            int k = Convert.ToInt32(choice);
            sendBuffer = Encoding.ASCII.GetBytes(choice);
            char[] sstr = new char[5];
            if (k<10){
            	sstr[0] = Convert.ToChar(k+48);
            	sstr[1]='.';
            	sstr[2]=' ';
            }
            if (k>=10 && k<100){
            	sstr[0] = Convert.ToChar((k - k % 10) / 10+48);
            	sstr[1] = Convert.ToChar(k % 10 +48);
            	sstr[2] = '.';
            	sstr[3] = ' ';
            }
            char[] filename = new char[50];
            int j = 0;
            for(int i=0; i<1024;i++){
        		if (bufor[i] == sstr[0] && bufor[i+1] == sstr[1] && bufor[i+2] == sstr[2] && k < 10 ||
        			bufor[i] == sstr[0] && bufor[i+1] == sstr[1] && bufor[i+2] == sstr[2] && bufor[i+3] == sstr[3] && k >= 10 && k <100){
            		i+=3;
            		while (bufor[i] != '\n'){
                		filename[j] = bufor[i];
                		i++;
                		j++;
            		}
        		}
    		}
    		string fname = new string(filename);
			foreach (var invalidChar in Path.GetInvalidFileNameChars())
    			fname = fname.Trim(invalidChar);
    		Console.WriteLine("Wybrano plik: " + fname);
            nwStream.Write(sendBuffer, 0, sendBuffer.Length);
            bytesToRead = new byte[16];
            bytesRead = nwStream.Read(bytesToRead, 0, 16);
            char[] chars = new char[16];
            char[] size = new char[16];
            for (int i = 0; i < bytesRead; i++)
			{
   				chars[i] = Convert.ToChar(bytesToRead[i]);
			}
			j = 0;
			foreach (char ch in chars)
			{
    			if (!char.IsDigit(ch))
    			{
         			continue;
    			} else {
    				size[j] = ch;
    				j++;
    			}
			}
			int fsize = int.Parse(Encoding.ASCII.GetString(Encoding.ASCII. GetBytes(size)));
			Console.WriteLine("Dlugosc pliku: " + fsize);
			bool w0 = false;
			bool w1 = false;
			bool w2 = false;
			bool w3 = false;
			bool w4 = false;
			bool w5 = false;
			bool w6 = false;
			bool w7 = false;
			bool w8 = false;
			bool w9 = false;
			using (var fileStream = new FileStream(fname, FileMode.Append, FileAccess.Write, FileShare.None))
			using (var bw = new BinaryWriter(fileStream)) {
				int bytesRead1 = 0;
				while(bytesRead1 < fsize) {
					bytesToRead = new byte[1024];
					bytesRead = nwStream.Read(bytesToRead, 0, 1024);
					bytesRead1 += bytesRead;
					bw.Write(bytesToRead,0,bytesRead);
					if ((0 * fsize) < bytesRead1 && bytesRead1 < (0.1 * fsize) && w0 == false) {
						Console.WriteLine("[__________] 0%");
						w0 = true;
					} 
					if ((0.1 * fsize) < bytesRead1 && bytesRead1 < (0.2 * fsize) && w1 == false) {
						Console.WriteLine("[*_________] 10%");
						w1 = true;
					} 
					if (0.2 * fsize < bytesRead1 && bytesRead1 < 0.3 * fsize && w2 == false) {
						Console.WriteLine("[**________] 20%");
						w2 = true;
					} 
					if (0.3 * fsize < bytesRead1 && bytesRead1 < 0.4 * fsize && w3 == false) {
						Console.WriteLine("[***_______] 30%");
						w3 = true;
					} 
					if (0.4 * fsize < bytesRead1 && bytesRead1 < 0.5 * fsize && w4 == false) {
						Console.WriteLine("[****______] 40%");
						w4 = true;
					} 
					if (0.5 * fsize < bytesRead1 && bytesRead1 < 0.6 * fsize && w5 == false) {
						Console.WriteLine("[*****_____] 50%");
						w5 = true;
					} 
					if (0.6 * fsize < bytesRead1 && bytesRead1 < 0.7 * fsize && w6 == false) {
						Console.WriteLine("[******____] 60%");
						w6 = true;
					} 
					if (0.7 * fsize < bytesRead1 && bytesRead1 < 0.8 * fsize && w7 == false) {
						Console.WriteLine("[*******___] 70%");
						w7 = true;
					} 
					if (0.8 * fsize < bytesRead1 && bytesRead1 < 0.9 * fsize && w8 == false) {
						Console.WriteLine("[********__] 80%");
						w8 = true;	
					}
					if (0.9 * fsize < bytesRead1 && bytesRead1 < fsize && w9 == false) {
						Console.WriteLine("[*********_] 90%");
						w9 = true;
					}  
				}
				if (fsize != bytesRead1){
					Console.WriteLine("Blad w odbiorze pliku!");
				}
				else {
					Console.WriteLine("[**********] 100%\nPlik odebrany poprawnie!");
				}
				bw.Close();
				fileStream.Close();
			}
            client.Close();
            Environment.Exit(0);
        }
    }
}

