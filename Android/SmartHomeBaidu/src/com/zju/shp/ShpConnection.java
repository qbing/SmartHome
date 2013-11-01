package com.zju.shp;

import java.io.IOException;
import java.net.InetSocketAddress;
import java.net.Socket;
import java.net.SocketAddress;
import java.nio.ByteBuffer;
import java.nio.channels.SelectionKey;
import java.nio.channels.Selector;
import java.nio.channels.SocketChannel;
import java.util.Set;

public class ShpConnection {

	public static String 		IP 		= "127.0.0.1";//"192.168.0.232";//"10.239.204.168";// "192.168.2.3";
	public static final int	PORT	= 9596;
	private static Socket 		soc  		= null;
	private SocketChannel 		soc_channel = null;
	private boolean  			thread_close_condition 	= false;
	private ByteBuffer recvBuf = ByteBuffer.allocate(4096);
	private ByteBuffer sendBuf = ByteBuffer.allocate(1024);
	public ShpConnection()
	{
		SocThread thread = new SocThread();
		thread.start();
	}
	
	private void sendData(SocketChannel socketChannel, byte[] bytes) throws IOException 
	{

	} 
	
	public void sendData(ByteBuffer bytes) throws IOException
	{
		try {
			if(soc_channel != null && soc_channel.isConnected()){
				soc_channel.write(bytes);
			}else{
				System.out.println("soc_channel.isConnected == flase");
			}
		} catch (Exception e) {
			// TODO Auto-generated catch block
			thread_close_condition = true;
			e.printStackTrace();
		}
	}
	class SocThread extends Thread{
		public void run() {
			Selector selector;
			while(true){
				System.out.println("SocThread running...");
				try {
					thread_close_condition = false;

					selector = Selector.open();
					// 创建一个套接字通道，注意这里必须使用无参形式
					soc_channel = SocketChannel.open();
					// 设置为非阻塞模式，这个方法必须在实际连接之前调用(所以open的时候不能提供服务器地址，否则会自动连接)
					soc_channel.configureBlocking(false);
					// 连接服务器，由于是非阻塞模式，这个方法会发起连接请求，并直接返回false(阻塞模式是一直等到链接成功并返回是否成功)
					soc_channel.connect(new InetSocketAddress(IP, PORT));
					// 注册关联链接状态
					soc_channel.register(selector, SelectionKey.OP_READ);
					
					if(soc_channel.finishConnect()){
						while (!thread_close_condition) {
							Set<SelectionKey> keys = selector.selectedKeys();
							for (SelectionKey key : keys) {
								// OP_READ 有数据可读
								if (key.isReadable()) {
									SocketChannel ch = (SocketChannel) key.channel();
									// 得到附件，就是上面SocketChannel进行register的时候的第三个参数,可为随意Object
									ByteBuffer buffer = (ByteBuffer) key.attachment();
									// 读数据 这里就简单写一下，实际上应该还是循环读取到读不出来为止的
									ch.read(buffer);
									recvBuf.put(buffer);
									int start = 0;
									while(start<recvBuf.position()){
										if(recvBuf.get(start)==ShpPackageBase.MAGIC1 && recvBuf.get(start+3)==ShpPackageBase.MAGIC2){
											
										}
									}
									recvBuf.clear();
									// 改变自身关注事件，可以用位或操作|组合时间
									key.interestOps(SelectionKey.OP_READ|SelectionKey.OP_WRITE);
								}
							}
						}
					}else{
						System.out.println("Socket connect failed!!!");
						sleep(3000);
					}
				} catch (IOException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
					break;
				} catch (InterruptedException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
			}
		}
	}
	
	
}
