package com.zju.shp;

import java.io.IOException;
import java.net.Socket;
import java.nio.ByteBuffer;
import java.util.HashMap;
import java.util.Map;

public class Shp {
	
	public static final short SHP_CONSULT_S0_PID      = (short) 0xfff0;
	public static final short SHP_CONSULT_S1_PID      = 0x0001;
	public static final short SHP_CONSULT_S2_PID      = 0x0002;
	public static final short SHP_CONSULT_S3_PID      = 0x0003;
	public static final short SHP_CONSULT_S4_PID      = 0x0004;
	public static final short SHP_SWITCH_PID          = 1001;
	public static final short SHP_T_SENSOR_PID        = 1002;
	public static final short SHP_G_SENSOR_PID        = 1003;
	public static final short SHP_VARIABLE_PID        = 1004;
	public static final short SHP_KEYBOARD_PID        = 1005;
	public static final short SHP_CAMERA_PID          = 1006;
	public static final short SHP_CUSTOM_PID          = 8000;
	public static final short SHP_DEV_TICK_PID        = 9999;
	
	public final static short SHP_OP_CLOSE	=0;
	public final static short SHP_OP_OPEN	=1;
	public final static short SHP_OP_READ	=2;
	public final static short SHP_OP_NOT	=3;
	public final static short SHP_OP_SET	=4;
	
	private static Shp 							shp= null;
	private static ShpConnection 				server_conn = null;
	private static Map<Short,ShpOperateable>	operatorMap = null;
	public static synchronized Shp getShp(){
		return shp;
	}
	
	public static void init(){
		shp = new Shp();
	}
	
	public static boolean register_opterator(short PID,ShpOperateable opertealbe)
	{
		Short pid = PID;
		ShpOperateable op=opertealbe;
		if(PID == op.getPID()){
			operatorMap.put(pid,op);
			return true;
		}else{
			return false;
		}
	}
	
	public void operate(String path,short dev_id,short op_type,byte[] data){
		
	}
	
	private Shp(){
		operatorMap = new HashMap<Short,ShpOperateable>();
		server_conn = new ShpConnection();
		ShpOperatorSwitch switch_op = new ShpOperatorSwitch();
		register_opterator(SHP_SWITCH_PID,switch_op);
	}
	
	public int sendData(ByteBuffer data){
		
		try {
			server_conn.sendData(data);
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		return data.position();
	}
	
	int parsePackage(ShpPackageBase pkg){
		short pid = pkg.PID;
		ShpOperateable opertealbe = operatorMap.get(pid);
		opertealbe.operator(null,(short)0,(short)0, null);
		return 0;
	}
}
