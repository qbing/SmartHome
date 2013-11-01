package com.zju.shpEX;

import java.util.HashMap;
import java.util.List;
import java.util.Map;

import net.sf.json.JSONArray;
import net.sf.json.JSONObject;

import com.zju.shp.*; 

public class ShpRoomDevice {
	private static ShpRoomDevice bean =null;

	public String name;
	public String room_id;
	public String type;
	public String state;
	public String dev_path;
	public int    dev_id;
	
	private ShpRoomDevice(){
		
	}
	
	public static ShpRoomDevice getShpRoomDevices()
	{
		return bean;
	}
	
	public String getName() {
		return name;
	}


	public void setName(String name) {
		this.name = name;
	}


	public String getRoom_id() {
		return room_id;
	}


	public void setRoom_id(String room_id) {
		this.room_id = room_id;
	}


	public String getType() {
		return type;
	}


	public void setType(String type) {
		this.type = type;
	}


	public String getState() {
		return state;
	}


	public void setState(String state) {
		this.state = state;
	}


	public String getDev_path() {
		return dev_path;
	}


	public void setDev_path(String dev_path) {
		this.dev_path = dev_path;
	}


	public int getDev_id() {
		return dev_id;
	}


	public void setDev_id(int dev_id) {
		this.dev_id = dev_id;
	}


	public List<ShpRoomDevice> getDevice_list() {
		return device_list;
	}

	public void setDevice_list(List<ShpRoomDevice> device_list) {
		this.device_list = device_list;
	}


	
	public List<ShpRoomDevice> device_list;

	
	public static void updata_ShpDeviceList()
	{
		String url = "http://"+ShpConnection.IP+"/db/admin/device_list";
		System.out.println(url);
		String jsonString = HttpRequest.sendGet(url,null);
		System.out.println(jsonString);
		
		Map classMap = new HashMap();  
		classMap.put( "device_list", ShpRoomDevice.class );  
		bean = (ShpRoomDevice)JSONObject.toBean( JSONObject.fromObject(jsonString), ShpRoomDevice.class, classMap );  
	}
}
