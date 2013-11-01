package com.zju.shp;

public interface ShpOperateable {
	public short getPID();
	public boolean operator(String path,short dev_id,short op_type,byte[] data);
}
