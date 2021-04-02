using System;
using System.Net;
using System.Net.Sockets;
using System.Text;

namespace socket
{
    class Program
    {
        static void Main(string[] args)
        {
			byte[] bytes = new byte[1024];
			try {
				IPHostEntry ipHostInfo = Dns.GetHostEntry("127.1.0.0");
				IPAddress ipAddress = ipHostInfo.AddressList[0];
				IPEndPoint remoteEP = new IPEndPoint(ipAddress, 12345);
				Socket sender = new Socket(ipAddress.AddressFamily, SocketType.Stream, ProtocolType.Tcp );
				try {
					sender.Connect(remoteEP);  
					Console.WriteLine($"Socket connected to {sender.RemoteEndPoint.ToString()}");
					Console.WriteLine("Enter message");
					string message = Console.ReadLine();
					Console.WriteLine("Your message");
					Console.WriteLine(message);
					byte[] mess = Encoding.ASCII.GetBytes(message);
					sender.Send(mess);					
					sender.Shutdown(SocketShutdown.Both);  
					sender.Close();
				} catch (Exception e) {
					Console.WriteLine($"Unexpected exception : {e.ToString()}");
				}
			} catch (Exception e) {
				Console.WriteLine( e.ToString());
			}
        }
    }
}
