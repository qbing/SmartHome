package com.qbryan.smarthome;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.InetSocketAddress;
import java.net.Socket;
import java.net.UnknownHostException;
import java.util.Timer;
import java.util.TimerTask;
import java.util.concurrent.Semaphore;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import android.os.Handler;
import android.os.Looper;
import android.os.Message;
import android.util.Log;

public class MyIOServer extends Thread {
	private static final String TAG = "QBryan SmartHome IOServer";
	private static final byte MAGIC1 = 'Q';
	private static final byte MAGIC2 = 'B';
	static String IP = "192.168.0.232";//"10.239.204.168";// "192.168.2.3";
	static int Port = 5000;
	static Socket s = null;
	static OutputStream os = null;
	static InputStream ins = null;
	
	public int	   delay_count = 0;
	public boolean isLamb1On = false;
	public boolean isLamb2On = false;
	public boolean isCDPlayerOn = false;
	public boolean isConnected = false;
	public int current_temp = 0;

	private Semaphore mutex = new Semaphore(1);
	private Thread	recvThread = null;

	Object lock = new Object();

	public enum DEVICE_ID {
		ID_LAMB1, ID_LAMB2, ID_CDPLAYER, ID_HEATER, ID_INDICATOR// 连接指示灯
	}

	public enum OP_TYPE {
		OP_OPEN, OP_CLOSE, OP_NOT, OP_SET
	}

	public enum OP_RESULT {
		SUCCESS, FAIL
	}

	MainActivity main_activity;

	public MyIOServer(MainActivity main_activity) {
		this.main_activity = main_activity;
	}

	public void setServerAddress(String ip, int port) {
		try {
			mutex.acquire();
			ip.trim();
			if (ip.isEmpty()==false) {
				IP = ip;
			}

			if (port > 1024 && port < 65535) {
				Port = port;
			}

			// restart
			if (s != null) {
				try {
					s.close();
				} catch (IOException e) {
					e.printStackTrace();
				}
			}

		} catch (InterruptedException e1) {
			// TODO Auto-generated catch block
			e1.printStackTrace();
		}finally{
			s = null;
			//start_base_server();
			mutex.release();
		}
	}


	public void startReadThread() {
		// ////处理终端发过来的消息///////
		recvThread = new Thread() {
			@Override
			public void run() {
				Looper.prepare();

				byte[] recv_buffer = new byte[1024];
				int recived_len = 0;
				while (true) {

					if (s != null && s.isConnected()) {
						try {
							mutex.acquire();
							s.setSoTimeout(10000);
							recived_len += ins.read(recv_buffer, recived_len,
									recv_buffer.length - recived_len);
							Log.v(TAG, "data arrive");
							int usedLen = Parser_Terminator_Infro(recv_buffer,
									recived_len);
							while (usedLen >= 0) {
								if (usedLen < recived_len) {
									recived_len -= usedLen;
									System.arraycopy(recv_buffer, usedLen,
											recv_buffer, 0, recived_len);
								} else {
									recived_len = 0;
									break;
								}
								usedLen = Parser_Terminator_Infro(recv_buffer,
										recived_len);
							}
						} catch (Exception e) {
							// TODO Auto-generated catch block
							connect_be_closed();
							recived_len = 0;
							e.printStackTrace();
						}finally{
							mutex.release();
						}
					}

					try {
						sleep(1000);
					} catch (InterruptedException e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					}
				}
			}
		};
		recvThread.start();
	}

	private byte[] sendBuffer = new byte[1024];
	private int size_wait_send = 0;

	@Override
	public void run() {
		Looper.prepare();
		// //////////
		feedWatchdog();

		// ////处理终端发过来的消息///////
		startReadThread();

		while (true) {

			start_base_server();

			try {
				sleep(1000);
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}

			// ////////send data////////begin/////////
			if (size_wait_send > 0) {
				synchronized (lock) {
					try {
						os.write(sendBuffer, 0, size_wait_send);
					} catch (IOException e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					}
					size_wait_send = 0;
				}
			}
			// ////////send data////////end///////////
		}

	}

	private synchronized void start_base_server() {
		if (s == null || s.isClosed() /* */) {
			Log.v(TAG, "try to connect server...");
			try {
				s = new Socket();
				s.connect(new InetSocketAddress(IP, Port), 4000);
				os = s.getOutputStream();
				ins = s.getInputStream();
			} catch (UnknownHostException e) {
				// TODO Auto-generated catch block
				connect_be_closed();
				e.printStackTrace();
			} catch (IOException e) {
				// TODO Auto-generated catch block
				connect_be_closed();
				e.printStackTrace();
			}
		} else {
		}
	}

	// /////////////////////////////////////////////////////
	public OP_RESULT operate(DEVICE_ID dev_id, OP_TYPE op_type, byte[] data) {
		start_base_server();
		final int HEAD_LEN = 6;
		byte b[] = new byte[32];
		b[0] = MAGIC1;
		b[1] = HEAD_LEN;
		b[2] = 0;
		b[3] = MAGIC2;
		b[4] = 'B';
		b[5] = 0;
		b[6] = (byte) op_type.ordinal();
		b[7] = (byte) dev_id.ordinal();
		System.arraycopy(data, 0, b, HEAD_LEN+2, data.length);
		b[1] = (byte) (HEAD_LEN+ 2 + data.length);

		{// //////test//
			Log.v(TAG, bytesToHexString(b, b[0]));
		}// /////test//

		synchronized (lock) {
			System.arraycopy(b, 0, sendBuffer, size_wait_send, b[1]);
			size_wait_send += b[1];
		}

		return OP_RESULT.SUCCESS;
	}

	/**
	 * byte数组转换成16进制字符串
	 * 
	 * @param src
	 * @return
	 */
	public static String bytesToHexString(byte[] src) {
		return bytesToHexString(src, 0);
	}

	public static String bytesToHexString(byte[] src, int len) {
		StringBuilder stringBuilder = new StringBuilder();
		if (src == null || src.length <= 0) {
			return null;
		}

		if (len > src.length || len <= 0)
			len = src.length;

		for (int i = 0; i < len; i++) {
			int v = src[i] & 0xFF;
			String hv = Integer.toHexString(v);
			stringBuilder.append("0x");
			if (hv.length() < 2) {
				stringBuilder.append(0);
			}
			stringBuilder.append(hv);
			stringBuilder.append(" ");
		}
		return stringBuilder.toString();
	}
 
	/**
	 * * 注释：short到字节数组的转换！
	 * 
	 * @param s
	 * @return
	 */
	public static byte[] shortToByte(short number) {
		int temp = number;
		byte[] b = new byte[2];

		for (int i = 0; i < b.length; i++) {
			b[i] = new Integer(temp & 0xff).byteValue();
			// 将最低位保存在最低位
			temp = temp >> 8; // 向右移8位
		}
		return b;
	}

	/**
	 ** 注释：字节数组到short的转换！ *
	 ** 
	 * @param b
	 ** @return
	 **/
	public static short byteToShort(byte[] b) {
		short s = 0;
		short s0 = (short) (b[0] & 0xff);// 最低位
		short s1 = (short) (b[1] & 0xff);
		s1 <<= 8;
		s = (short) (s0 | s1);
		return s;
	}

	// ////////////////////////////////////////////////

	// /////////////////////////////////////////
	private int Parser_Terminator_Infro(byte[] recived_data, int recived_len) {
		byte[] current_temp_bytes = new byte[2];
		if (recived_len > 3)
		// 最小的头长度为3
		{
			short r_len =0;
			current_temp_bytes[0] = recived_data[1];
			current_temp_bytes[1] = recived_data[2];
			if (recived_data[0] == MAGIC1 && recived_data[3] == MAGIC2) {
				if (recived_len >= (r_len=byteToShort(current_temp_bytes))) {
					/*
					 * int jsonStr_Len = recived_data[1]-3; byte tempBytes[]=new
					 * byte[jsonStr_Len+1]; System.arraycopy(recived_data, 3,
					 * tempBytes, 0, jsonStr_Len); tempBytes[jsonStr_Len] =
					 * '\0'; String jsonStr = new String(tempBytes);
					 * Parser_Terminator_Infro(jsonStr); return jsonStr_Len+1;
					 */
					Log.v(TAG, bytesToHexString(recived_data, byteToShort(current_temp_bytes)));
					current_temp_bytes[0] = recived_data[6];
					current_temp_bytes[1] = recived_data[7];
					current_temp = byteToShort(current_temp_bytes);
					
					////delay and set device state
					delay_count=delay_count-1;
					if(delay_count<=0){
						isLamb1On = (0 != (recived_data[8] & 0x01));
						isLamb2On = (0 != (recived_data[8] & 0x02));
						isCDPlayerOn = (0 != (recived_data[8] & 0x04));
						delay_count=0;
					}
					isConnected = true;
					feedWatchdog();
					return r_len;
				}
				return 0;
			}
			// !!!something error!!skip some char!!
			int i = 1;
			while (i < recived_len - 2
					&& (recived_data[i] != MAGIC1 || recived_data[i + 2] != MAGIC2))
				i++;
			return i;
		} else {
			return -1;
		}
	}

	private void Parser_Terminator_Infro(String jSonStr) {
		Log.v(TAG, jSonStr);
		String tempStr = null;
		tempStr = Parser_Terminator_Infro_Find_Target_Str(jSonStr, "CMD");
		isConnected = (tempStr.equals("TICK")) ? true : false;

		if (isConnected) {
			try {
				tempStr = Parser_Terminator_Infro_Find_Target_Str(jSonStr,
						"TEMP");
				current_temp = Integer.valueOf(tempStr);

				tempStr = Parser_Terminator_Infro_Find_Target_Str(jSonStr,
						"LAMB1");
				isLamb1On = (tempStr.equals("ON")) ? true : false;

				tempStr = Parser_Terminator_Infro_Find_Target_Str(jSonStr,
						"LAMB2");
				isLamb2On = (tempStr.equals("ON")) ? true : false;

				tempStr = Parser_Terminator_Infro_Find_Target_Str(jSonStr,
						"CDPLAYER");
				isCDPlayerOn = (tempStr.equals("ON")) ? true : false;
			} catch (NumberFormatException e) {
				// TODO Auto-generated catch block
				// e.printStackTrace();
			}

			// test
			// main_activity.updateViewState();
		}

	}

	private String Parser_Terminator_Infro_Find_Target_Str(String jsonStr,
			String name) {

		String regexp = name + ":(.*?),";
		Pattern p = Pattern.compile(regexp);
		Matcher m = p.matcher(jsonStr);
		if (m.find()) {
			return m.group(1);
		} else {
			return "";
		}
	}

	// ///////////////////////for watch
	// dog//////////////////begin///////////////////////
	Timer timer = null;
	Handler handler = null;
	TimerTask task = null;
	final int wait_ms = 3000;

	public void feedWatchdog() {
		Log.v(TAG, "feed watchdog");
		// ///stop precious timer
		if (task != null) {
			task.cancel();
			task = null;
		}
		if (timer != null) {
			timer.cancel();
			timer = null;
		}

		handler = new Handler() {
			@Override
			public void handleMessage(Message msg) {
				switch (msg.what) {
				case 1:
					connect_be_closed();
					break;
				}
			}
		};
		task = new TimerTask() {
			@Override
			public void run() {
				Message message = new Message();
				message.what = 1;
				handler.sendMessage(message);
			}
		};
		timer = new Timer();
		timer.schedule(task, wait_ms, wait_ms);
	}

	void connect_be_closed() {
		try {
			s = null;
			isConnected = false;
			Message message = new Message();
			message.what = 1;
			main_activity.handler.sendMessage(message);
		} catch (Exception e) {
			// TODO: handle exception
			e.printStackTrace();
		}
	}
	
	void close()
	{
		if (s != null) {
			try {
				s.close();
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
		connect_be_closed();
	}
	// ///////////////////////for watch
	// dog//////////////////end/////////////////////////

	// /////////////////////////////////////////
	/*
	 * public static class MyPackage{
	 * 
	 * public byte len; public static final byte magic = MAGIC1; public OP_TYPE
	 * op_type; public DEVICE_ID dev_id; public byte op_data; public MyPackage()
	 * { len = 5; } }
	 */
}
