package com.zju.shpEX;

import java.util.HashMap;
import java.util.List;
import java.util.Map;

import net.sf.json.JSONArray;
import net.sf.json.JSONObject;

import com.zju.shp.*; 

public class ShpRoom {
	public static ShpRoom bean =null;
	public String name;
	public String ID;
	public String bg_src;
	public int type;
	
	public List<ShpRoom> room_list;
	public List<ShpRoom> getRoom_list() {
		return room_list;
	}

	public void setRoom_list(List<ShpRoom> room_list) {
		this.room_list = room_list;
	}

	public static ShpRoom getShpRoom()
	{
		return bean;
	}
	
	private ShpRoom()
	{
		
	}
	public String getName() {
		return name;
	}
	public void setName(String name) {
		this.name = name;
	}
	public String getID() {
		return ID;
	}
	public void setID(String iD) {
		ID = iD;
	}
	public String getBg_src() {
		return bg_src;
	}
	public void setBg_src(String bg_src) {
		this.bg_src = bg_src;
	}
	public int getType() {
		return type;
	}
	public void setType(int type) {
		this.type = type;
	}
	
	public static void updata_ShpRoomList()
	{
		String url = "http://"+ShpConnection.IP+"/db/admin/room_infor";
		System.out.println(url);
		String jsonString = HttpRequest.sendGet(url,null);
		System.out.println(jsonString);

		Map classMap = new HashMap();  
		classMap.put("room_list", ShpRoom.class);  
		bean = (ShpRoom) JSONObject.toBean( JSONObject.fromObject(jsonString), ShpRoom.class, classMap );	
	}
}
