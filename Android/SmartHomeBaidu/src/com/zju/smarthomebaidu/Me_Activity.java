package com.zju.smarthomebaidu;

import android.app.Activity;
import android.content.Context;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;
import android.view.Window;
import android.view.WindowManager;
import android.widget.BaseAdapter;
import android.widget.ImageButton;
import android.widget.ListView;
import android.widget.RelativeLayout;

public class Me_Activity extends Activity {

	public Me_Activity() {
		// TODO Auto-generated constructor stub
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
		setContentView(R.layout.me_layout);
		
		/*
		ListView lv = (ListView)this.findViewById(R.id.me_listview);
		lv.setAdapter(new Me_List_Adapter(this));
		*/
		
		ImageButton exit_btn = (ImageButton)this.findViewById(R.id.exit_imageButton);
		exit_btn.setOnClickListener(new OnClickListener(){

			@Override
			public void onClick(View v) {
				// TODO Auto-generated method stub
				Me_Activity.this.finish();
			}
		});
		
		BottomBarControl bottombar_btns = new BottomBarControl(this);
		bottombar_btns.BindToActivity(this);
	}
	class Me_List_Adapter extends BaseAdapter
	{
		LayoutInflater layoutInflater = null;
		public Me_List_Adapter(Context context)
		{
			layoutInflater = LayoutInflater.from(context);
		}
		@Override
		public int getCount() {
			// TODO Auto-generated method stub
			return 4;
		}

		@Override
		public Object getItem(int position) {
			// TODO Auto-generated method stub
			return null;
		}

		@Override
		public long getItemId(int position) {
			// TODO Auto-generated method stub
			return 0;
		}

		@Override
		public View getView(int position, View convertView, ViewGroup parent) {
			ItemsHolder holder = null;
			// TODO Auto-generated method stub
			if(convertView == null){
				holder = new ItemsHolder(); 
				convertView = layoutInflater.inflate(R.layout.me_layout_list_items,null);
				holder.about = (RelativeLayout)convertView.findViewById(R.id.me_about_viewgroup);
				holder.server_setting = (RelativeLayout)convertView.findViewById(R.id.me_server_setting_viewgroup);
				holder.high_level_setting = (RelativeLayout)convertView.findViewById(R.id.me_high_level_setting_viewgroup);
				holder.user_infor = (RelativeLayout)convertView.findViewById(R.id.me_userinfor_viewgroup);
				
				convertView.setTag(holder);
			}else{
				holder = (ItemsHolder)convertView.getTag();
			}
			/*
			switch(position)
			{
			case 0:
				return holder.about;
			case 1:
				return holder.server_setting;
			case 2:
				return holder.high_level_setting;
			case 3:
				return holder.user_infor;
			}*/
			return convertView;
		}
		class ItemsHolder
		{
			public RelativeLayout about;
			public RelativeLayout server_setting;
			public RelativeLayout high_level_setting;
			public RelativeLayout user_infor;
		}
	}

}
