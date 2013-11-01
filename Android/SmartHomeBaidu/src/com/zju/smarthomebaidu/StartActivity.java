package com.zju.smarthomebaidu;

import com.baidu.api.Baidu;
import com.baidu.api.BaiduDialogError;
import com.baidu.api.BaiduException;
import com.baidu.api.Util;
import com.baidu.api.BaiduDialog.BaiduDialogListener;
import com.zju.smarthomebaidu.BottomBarControl.ActType;

import android.annotation.TargetApi;
import android.app.Activity;
import android.content.Intent;
import android.content.pm.ActivityInfo;
import android.os.Build;
import android.os.Bundle;
import android.os.Handler;
import android.view.MotionEvent;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

public class StartActivity extends Activity {
    private Baidu baidu = null;
	StartActivity  act = null;
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		act = this;
        ///////////////full screen & landscape/////////begin/////////
		getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN);//设置成全屏模式
		//setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);//强制为横屏
		requestWindowFeature(Window.FEATURE_NO_TITLE);//无标题
        ///////////////full screen & landscape/////////end/////////
		
		BottomBarControl.current_type = ActType.ActType_HOME;
		setContentView(R.layout.activity_start);
		Button login_btn = (Button)this.findViewById(R.id.login_button);
		login_btn.setOnClickListener(new OnClickListener(){
			@Override
			public void onClick(View v) {
				// TODO Auto-generated method stub
				EditText username = (EditText)act.findViewById(R.id.username_editText);
				EditText userpwd = (EditText)act.findViewById(R.id.userpwd_editText);
				if(username.getText().toString().equals("admin") && userpwd.getText().toString().equals("123"))
				{
					Intent it = new Intent(StartActivity.this,HomeActivity.class);
					startActivity(it);
					act.finish();
				}else{
					Toast t = Toast.makeText(act,"用户密码错误",Toast.LENGTH_SHORT);
					t.show();
				}
			}
		});		
		
		Button login_by_baidu_btn = (Button)this.findViewById(R.id.login_by_baidu_button);
		login_by_baidu_btn.setOnClickListener(new OnClickListener(){
			@Override
			public void onClick(View v) {
			    //String clientId = "0trswTLaGB6hN820M30Brbhx";
			    String clientId = "DZY6INXluih7mrXkAjBsChUh";
			    boolean isForceLogin = false;
			    boolean isConfirmLogin = true;
			    
                baidu = new Baidu(clientId, StartActivity.this);
                baidu.authorize(StartActivity.this, isForceLogin,isConfirmLogin,new BaiduDialogListener() {

                    @Override
                    public void onComplete(Bundle values) {
        				Intent it = new Intent(StartActivity.this,HomeActivity.class);
        				startActivity(it);
        				act.finish();
                    }
                    @Override
                    public void onBaiduException(BaiduException e) {

                    }
                    @Override
                    public void onError(BaiduDialogError e) {

                    }
                    @Override
                    public void onCancel() {
	                    Util.logd("cancle","I am back");
                    }
                });
            }
		});
	}
}
