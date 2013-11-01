package com.zju.smarthomebaidu;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.Window;
import android.view.WindowManager;
import android.widget.Button;

public class Add_Devices_Main extends Activity {

	public Add_Devices_Main() {
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
		setContentView(R.layout.add_devices);
		
		Button scan_btn = (Button)this.findViewById(R.id.btnScanQRCode);
		scan_btn.setOnClickListener(new OnClickListener(){
			@Override
			public void onClick(View v) {
				Intent it = new Intent(Add_Devices_Main.this,Add_Devices_Scan.class);
				startActivity(it);
			}
		});		
		Button input_btn = (Button)this.findViewById(R.id.btnInputBarCode);
		input_btn.setOnClickListener(new OnClickListener(){
			@Override
			public void onClick(View v) {
				Intent it = new Intent(Add_Devices_Main.this,InputBarcodeActivity.class);
				startActivity(it);
			}
		});		
		Button presse_btn = (Button)this.findViewById(R.id.btnTouchMatch);
		presse_btn.setOnClickListener(new OnClickListener(){
			@Override
			public void onClick(View v) {
				Intent it = new Intent(Add_Devices_Main.this,TouchMatchActivity.class);
				startActivity(it);
			}
		});
		
	}
}
