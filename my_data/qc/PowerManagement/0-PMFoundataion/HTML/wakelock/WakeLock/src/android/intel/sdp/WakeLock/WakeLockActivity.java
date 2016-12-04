/*Copyright (c) <2012>, Intel Corporation

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

- Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
- Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
- Neither the name of Intel Corporation nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

package android.intel.sdp.WakeLock;

import android.app.Activity;
import android.content.Context;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.os.PowerManager;
import android.os.PowerManager.WakeLock;
import android.provider.Settings;
import android.util.Log;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.WindowManager;
import android.widget.Button;
import android.widget.LinearLayout;
import android.widget.TextView;

import java.util.Timer;
import java.util.TimerTask;

/**
 * The WakeLockActivity handles an individual wakelock test. Currently implemented wakelock tests include: 
 *   Power Wake Lock Full, Power Wake Lock Bright, Power Wake Lock Dim
 *   Power Wake Lock Partial, Win Man Screen On
 */
public class WakeLockActivity extends Activity {

	private static final String TAG = "SDP_WAKE_LOCK";

	private final String FLAG_SUC = "SDP_WAKE_LOCK_SUCCESSFULLY";
	private final String FLAG_FAIL = "SDP_WAKE_LOCK_FAILED";

	private final int WAKE_LOCK_TIME = 10;// seconds
	private final int SETTING_TIME = 5;

	private PowerManager mPowerManager;
	private PowerManager.WakeLock mWakeLock;
	private int mWakeLockState = -1;
	private String mTestMode = POWER_TEST_MANUALLY;

	private int mTimeCount;

	private boolean mFailed = true;
	private boolean mIsDisplayLocked;

	private TextView mTextView;
	private TextView mTimeView;
	private LinearLayout mLayout;
	private Button mScreenLock;
	private TextView mLockView;

	public static final String KEY_WAKE_LOCK_TYPE = "wake_lock_type";
	public static final String KEY_TEST_MODE = "test_mode";

	public static final String POWER_TEST_AUTOMATIC = "automatic";
	public static final String POWER_TEST_MANUALLY = "manually";

	public static final int MESSAGE_WHAT_TIME = 0x01;

	/** called upon initial launch of this Activity
	 * @param savedInstanceState used for saving an application's state
	 */
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.wakelock);

		// this variable controls the state of the button and is only 
		//  used in the only non-Wake Lock example, "Window Man Screen On"
		mIsDisplayLocked=true;
		
		mTextView = (TextView) findViewById(R.id.textview);
		mTimeView = (TextView) findViewById(R.id.timerview);
		mLayout = (LinearLayout) findViewById(R.id.layout);

		mPowerManager = (PowerManager) getSystemService(Context.POWER_SERVICE);

		if (getIntent().getStringExtra(KEY_TEST_MODE) != null) {
			mTestMode = getIntent().getStringExtra(KEY_TEST_MODE);
		}

		// request a ScreenOff Intent to go off in SETTING_TIME seconds
		Settings.System.putInt(this.getContentResolver(),
				Settings.System.SCREEN_OFF_TIMEOUT, SETTING_TIME * 1000);

		// get this activity's wake lock type: e.g. full, bright, partial, dim, winman
		String strLockType = getIntent().getStringExtra(KEY_WAKE_LOCK_TYPE);

		mTextView.setText("Wake lock accquired, wake lock mode:" + strLockType);

		if (mTestMode != null && mTestMode.equals(POWER_TEST_AUTOMATIC)) {
			TestThread testThread = new TestThread();
			testThread.start();
		} else {
			if (strLockType.equalsIgnoreCase("winman")) {
				// we are NOT creating our own wakelock in this case. We are using the Window Manager to control display dimming.
				// Add a button to control enabling/disabling of locking the screen (brightness)
				mLockView = new TextView(this);
				mLockView.setText("Press Button to disable the Screen Lock, when you're ready");
				mScreenLock = new Button(this);
				
				mScreenLock.setOnClickListener(new OnClickListener() {
					public void onClick(View v) {
						// toggle state of isDisplayLocked
						mIsDisplayLocked = !mIsDisplayLocked;
						// update state of display
						screenLockUpdateState();
					}
				});
				
				screenLockUpdateState();

				mLayout.addView(mScreenLock);
				mLayout.addView(mLockView);
			} else { 
				// we are doing a wake lock. e.g. full, partial, dim, or bright wakelock was selected
				String strLockState = strLockType;
				mWakeLockState = getWakeLockType(strLockState);
				mWakeLock = mPowerManager.newWakeLock(mWakeLockState,
						"UMSE PowerTest");
				if (mWakeLock != null) {
					mWakeLock.acquire();
	
					Message msg = new Message();
					handler.sendMessage(msg);
				}
			}
		}

		Timer timer = new Timer();
		timer.schedule(new TimerTask() {

			@Override
			public void run() {
				Message msg = new Message();
				msg.what = MESSAGE_WHAT_TIME;
				mTimeCount++;

				handler.sendMessage(msg);
			}
		}, 1000, 1000);

	}

	/**
	 * TestThread is used for Automatic (not Manual) testing. Revert to its use in the onCreate method of this activity.
	 *
	 */
	class TestThread extends Thread {

		@Override
		public void run() {
			super.run();
			try {
				WakeLock fullWakeLock = mPowerManager.newWakeLock(
						PowerManager.FULL_WAKE_LOCK, "UMSE Full Lock");
				if (fullWakeLock != null) {
					Log.d(TAG, "Acquire full screen wake lock.");
					fullWakeLock.acquire();
				}

				Thread.sleep(WAKE_LOCK_TIME * 1000);

				if (mPowerManager.isScreenOn()) {
					Log.d(TAG, "Full screen wake lock ok.");
					mFailed = false;
				} else {
					Log.d(TAG, "Full screen wake lock fail.");
				}

				if (fullWakeLock != null) {
					fullWakeLock.release();
					fullWakeLock = null;
					Log.d(TAG, "Release full wake lock.");
				}

			} catch (InterruptedException e) {
				Log.d(TAG, e.getMessage(), e);
			}

			finish();
		}
	}

	/** This handler handles the message received from the TimerTask. This is used to display the time elapsed for this Activity.
	 * 
	 */
	protected Handler handler = new Handler() {
		public void handleMessage(android.os.Message msg) {

			switch (msg.what) {
			case MESSAGE_WHAT_TIME:
				mTimeView.setText("Time passed:" + mTimeCount + "s");
				break;

			default:
				break;
			}
		};
	};

	@Override
	/** called when destroying this Activity. Do resource cleanup here
	 * 
	 */
	protected void onDestroy() {

		if (mWakeLock != null) {
			mWakeLock.release();
			mWakeLock = null;
		}

		if (mTestMode != null && mTestMode.equals(POWER_TEST_AUTOMATIC)) {
			if (mFailed) {
				Log.d(TAG, FLAG_FAIL);
			} else {
				Log.d(TAG, FLAG_SUC);
			}
		}

		Settings.System.putInt(this.getContentResolver(),
				Settings.System.SCREEN_OFF_TIMEOUT, 30 * 60 * 1000);

		super.onDestroy();
	}

	/** This method returns the id associated with each wakelock
	 * 
	 * @param strType a String matching the name of a wakelock (e.g. full, bright, dim, partial)
	 * @return an integer matching the id for a desired wakelock
	 */
	public int getWakeLockType(String strType) {
		if (strType.toLowerCase().equals("full")) {
			return PowerManager.FULL_WAKE_LOCK;
		} else if (strType.toLowerCase().equals("bright")) {
			return PowerManager.SCREEN_BRIGHT_WAKE_LOCK;
		} else if (strType.toLowerCase().equals("dim")) {
			return PowerManager.SCREEN_DIM_WAKE_LOCK;
		} else if (strType.toLowerCase().equals("partial")) {
			return PowerManager.PARTIAL_WAKE_LOCK;
		}

		return PowerManager.FULL_WAKE_LOCK;
	}

	/** This method updates the state (and GUI) when the button to enable/disable the screen lock is pressed. This is only used in the onClickListener for the WinMan test.
	 * 
	 */
	public void screenLockUpdateState() {

		if (mIsDisplayLocked)
		{
			// update button text
			mScreenLock.setText("Screen is LOCKED");
			mLockView.setVisibility(View.VISIBLE);			
			// update display state
			getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
		} else 
		{
			// update button text
			mScreenLock.setText("Screen is UNLOCKED");
			mLockView.setVisibility(View.INVISIBLE);			
			getWindow().clearFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
		}		
	}
}