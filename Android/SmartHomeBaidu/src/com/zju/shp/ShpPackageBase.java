package com.zju.shp;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;

public class ShpPackageBase {
	public static final byte 	MAGIC1 = 'Q';
	public static final byte 	MAGIC2 = 'B';
	
	public Head			head;
	public short 		PID;
	public byte[]		MAC		= new byte[6];
	private ByteBuffer 	headBuf = null;
	
	public ShpPackageBase(int buffer_capacity){
		this.head = new Head();
		this.head.magic1 = ShpPackageBase.MAGIC1;
		this.head.magic2 = ShpPackageBase.MAGIC2;
		headBuf = ByteBuffer.allocate(buffer_capacity);
		headBuf.order(ByteOrder.LITTLE_ENDIAN);
	}
	public synchronized ByteBuffer getHeadByteBuffer(){
		headBuf.clear();
		headBuf.put(head.magic1);
		headBuf.putShort(head.pkg_len);
		headBuf.put(head.magic2);
		headBuf.put(head.content_type);
		headBuf.put(head.reserve);
		headBuf.mark();
		headBuf.flip();
		return headBuf;
	} 
	
	class Head{
		byte	magic1;
		short	pkg_len;
		byte	magic2;
		byte	content_type;
		byte	reserve;
	}
}
