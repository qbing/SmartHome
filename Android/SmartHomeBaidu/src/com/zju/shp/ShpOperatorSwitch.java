package com.zju.shp;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;

public class ShpOperatorSwitch extends ShpPackageBase implements ShpOperateable {
	
	public ShpOperatorSwitch() {
		super(256);
		this.head.content_type='B';
		this.PID = getPID();
	}

	public short getPID() {
		return Shp.SHP_SWITCH_PID;
	}

	byte tttttt_op_type = 0;
	public boolean operator(String path,short dev_id,short op_type,byte[] data) {
		ByteBuffer sendBuf = ByteBuffer.allocate(512); 
		ByteBuffer tempBuf = ByteBuffer.allocate(512); 
		String tempPath = (path==null)?"":path;
		sendBuf.order(ByteOrder.LITTLE_ENDIAN);
		tempBuf.order(ByteOrder.LITTLE_ENDIAN);

		tempBuf.putShort(PID);						//PID
		tempBuf.putShort((short) 0);				//DEV_ID
		tempBuf.put(tttttt_op_type);				//OP_TYPE
		tempBuf.putShort((short)tempPath.length());	//Path_size
		tempBuf.putShort((short)0);					//reserve
		tempBuf.put(ByteBuffer.wrap(tempPath.getBytes()));//Path
		tempBuf.mark();
		tempBuf.flip();		
		System.out.println(tempBuf.toString());
		
		this.head.pkg_len = (short) (tempBuf.limit() + 6);
		sendBuf.put(this.getHeadByteBuffer());
		sendBuf.put(tempBuf);
		sendBuf.mark();
		sendBuf.flip();
		Shp shp = Shp.getShp();
		shp.sendData(sendBuf);
		
		tttttt_op_type = (byte) ((0==tttttt_op_type)?1:0);
		return false;
	}
	
}
