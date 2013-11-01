package com.zju.smarthomebaidu;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.view.Window;
import android.view.WindowManager;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemClickListener;
import android.widget.BaseAdapter;
import android.widget.ImageView;
import android.widget.ListView;
import android.widget.TextView;

public class RoomDetailActivity extends Activity {

	RoomDetailActivity act = null;
	public RoomDetailActivity() {
		// TODO Auto-generated constructor stub
		act = this;
	}
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
        ///////////////full screen & landscape/////////begin/////////
		getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN);//设置成全屏模式
		//setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);//强制为横屏
		requestWindowFeature(Window.FEATURE_NO_TITLE);//无标题
        ///////////////full screen & landscape/////////end/////////
		setContentView(R.layout.room_detail);
		
		ListView lv = (ListView)this.findViewById(R.id.room_detail_device_items_list);
		lv.setAdapter(new RoomDetail_device_list_adapter(act));
		lv.setOnItemClickListener(new OnItemClickListener()
		{
			@Override
			public void onItemClick(AdapterView<?> arg0, View arg1, int position,
					long arg3) {
				// TODO Auto-generated method stub
				Log.v("QBing","position="+position);
				if(position == RoomDetail_device_list_adapter.device_name.length-1){
					Intent monitor_it = new  Intent(act,Add_Devices_Main.class);
					act.startActivity(monitor_it);
				}else if(position == 0){
					Intent it = new  Intent(act,Device_HairAirconditionor_Activity.class);
					act.startActivity(it);
				}else if(position == 1){
					Intent it = new  Intent(act,Device_NormalLight_Activity.class);
					act.startActivity(it);
				}
			}
		});
		
	}
	
	static class RoomDetail_device_list_adapter extends BaseAdapter
	{
		public  final static String device_name[]={
				"海尔空调",
				"台灯1",
				"添加设备"
				};
		LayoutInflater layoutInflater = null;
		public RoomDetail_device_list_adapter(Context context)
		{
			layoutInflater = LayoutInflater.from(context);
		}
		@Override
		public int getCount() {
			// TODO Auto-generated method stub
			return device_name.length;
		}

		@Override
		public Object getItem(int position) {
			// TODO Auto-generated method stub
			return null;
		}

		@Override
		public long getItemId(int position) {
			// TODO Auto-generated method stub
			return position;
		}

		@Override
		public View getView(int position, View convertView, ViewGroup parent) {
			// TODO Auto-generated method stub
			ItemHolder holder = null;
			if(convertView == null)
			{
				holder = new ItemHolder();
				convertView = layoutInflater.inflate(R.layout.room_detail_listitem,null);
				holder.img = (ImageView)convertView.findViewById(R.id.device_state_img);
				holder.name = (TextView)convertView.findViewById(R.id.device_name_tv);
				convertView.setTag(holder);
			}else{
				holder = (ItemHolder)convertView.getTag();
			}
			holder.name.setText(device_name[position]);
			switch(position)
			{
			case 0:
				holder.img.setBackgroundResource(R.drawable.dev_icon_airconditionor_on);
				break;
			case 1:
				holder.img.setBackgroundResource(R.drawable.dev_icon_normal_light_off);
				break;
			case 2:
				holder.img.setBackgroundResource(R.drawable.dev_icon_add_device);
				break;
			}
			return convertView;
		}
		
		class ItemHolder
		{
			ImageView img;
			TextView  name;
		}
	}
}
