package com.zju.smarthomebaidu;

import android.app.Activity;
import android.content.Intent;
import android.content.pm.ActivityInfo;
import android.os.Bundle;
import android.util.Log;
import android.view.MotionEvent;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.View.OnTouchListener;
import android.view.Window;
import android.view.WindowManager;
import android.widget.ImageButton;
import android.widget.ImageView;

public class HomeActivity extends Activity {
	public HomeActivity() {
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
		setContentView(R.layout.main_layout);

		Log.v("QBing","onCreate");

		ImageButton main_room_btn = (ImageButton)this.findViewById(R.id.enter_main_room_btn);
		main_room_btn.setOnClickListener(new OnClickListener(){
			@Override
			public void onClick(View v) {
				// TODO Auto-generated method stub
				Intent it = new Intent(HomeActivity.this,RoomDetailActivity.class);
				startActivity(it);
			}
		});		
		
		ImageButton room_1_btn = (ImageButton)this.findViewById(R.id.enter_room_1_btn);
		room_1_btn.setOnClickListener(new OnClickListener(){
			@Override
			public void onClick(View v) {
				// TODO Auto-generated method stub
				Intent it = new Intent(HomeActivity.this,RoomDetailActivity.class);
				startActivity(it);
			}
		});		
		
		ImageButton room_2_btn = (ImageButton)this.findViewById(R.id.enter_room_2_btn);
		room_2_btn.setOnClickListener(new OnClickListener(){
			@Override
			public void onClick(View v) {
				Log.v("QBing","room_2_btn onClick");
				// TODO Auto-generated method stub
				Intent it = new Intent(HomeActivity.this,RoomDetailActivity.class);
				startActivity(it);
			}
		});
		
		ImageButton add_room_btn = (ImageButton)this.findViewById(R.id.add_room_btn);
		add_room_btn.setOnClickListener(new OnClickListener(){
			@Override
			public void onClick(View v) {
				Log.v("QBing","room_2_btn onClick");
				// TODO Auto-generated method stub
				Intent it = new Intent(HomeActivity.this,AddRoomActivity.class);
				startActivity(it);
			}
		});
		
		BottomBarControl bottombar_btns = new BottomBarControl(this);
		bottombar_btns.BindToActivity(this);
	}
}
