package lantrans;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.MulticastSocket;
import java.net.UnknownHostException;
import java.util.Date;

import javax.swing.JTextArea;

public class ChatRoom
{
	private static final int BUFFER_SIZE = 256;
	private static final int SEND_PORT = 8990;
	private static final int RECEIVE_PORT = 8991;
	private static final String BROADCAST_ADDRESS = "230.1.1.1";
	
	private static InetAddress broadcastIP;
	
	private String userName;
	private String userIP;
	private JTextArea messageArea;
	
	public ChatRoom(String myName, String myIP, JTextArea textArea)
	{
		userName = myName;
		userIP = myIP;
		messageArea = textArea;
		try
		{
			broadcastIP = InetAddress.getByName(BROADCAST_ADDRESS);
		}
		catch(UnknownHostException e)
		{
			e.printStackTrace();
		}
	}
	
	public void send(String message)
	{
		new Thread(new Runnable()
		{				
			public void run()
			{
				try(MulticastSocket socket = new MulticastSocket(SEND_PORT))
				{	
					String formattedMessage = userName + "  :  " + userIP + "    " + new Date().toString() + 
							"\r\n    " + message + "\r\n";					
					byte b[] = formattedMessage.getBytes();
					socket.setTimeToLive(1);
					DatagramPacket packet = new DatagramPacket(b, b.length, broadcastIP, RECEIVE_PORT);
					socket.send(packet);
				}
				catch(IOException e)
				{
					e.printStackTrace();
				}
			}
		}).start();
	}
	
	public void listen()
	{
		new Thread(new Runnable() 
		{				
			public void run()
			{
				while(true)
				{
					try(MulticastSocket socket = new MulticastSocket(RECEIVE_PORT))
					{
						socket.joinGroup(broadcastIP);
						
						byte[] b = new byte[BUFFER_SIZE];
						DatagramPacket packet = new DatagramPacket(b, b.length);							
						socket.receive(packet);						
						String mes = new String(packet.getData(), 0, packet.getLength());
						messageArea.append(mes);
						socket.leaveGroup(broadcastIP);
					}
					catch(IOException e)
					{
						e.printStackTrace();
					}
				}				
			}
		}).start();
	}
}
