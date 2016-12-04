/*Copyright (c) <2012>, Intel Corporation

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

- Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
- Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
- Neither the name of Intel Corporation nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

package android.intel.sdp.WakeLock;

import android.app.AlertDialog;
import android.app.ListActivity;
import android.content.DialogInterface;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.widget.ListView;
import android.widget.SimpleAdapter;
import android.widget.TextView;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

/**
 * The LockList Activity is the first screen to load upon application launch.
 * It lists the different wakelock tests available: 
 *   Power Wake Lock Full, Power Wake Lock Bright, Power Wake Lock Dim
 *   Power Wake Lock Partial, Win Man Screen On
 */
public class LockList extends ListActivity {

	private List<String> mTestList;

	@Override
	/** called upon initial launch of this Activity
	 * @param savedInstanceState used for saving an application's state
	 */
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);

		mTestList = readWakeLockStateList();

		List<Map<String, String>> list = new ArrayList<Map<String, String>>();

		for (String str : mTestList) {
			String title = str.split("#")[0];
			Map<String, String> map = new HashMap<String, String>();
			map.put("title", title);

			list.add(map);
		}

		setListAdapter(new SimpleAdapter(this, list,
				android.R.layout.simple_list_item_1, new String[] { "title" },
				new int[] { android.R.id.text1 }));

	}

	/** Builds a List of menu items (taken from R.raw.power_list file)
	 * 
	 * @return a list that will populate this Activity's View  
	 */
	public List<String> readWakeLockStateList() {

		List<String> wakeLockStateList = new ArrayList<String>();

		InputStream is = this.getResources().openRawResource(R.raw.power_list);
		BufferedReader br = new BufferedReader(new InputStreamReader(is));

		String line = null;
		try {
			while ((line = br.readLine()) != null && line.trim().length() > 0) {
				wakeLockStateList.add(line);
			}
		} catch (IOException e) {
			e.printStackTrace();
		}

		return wakeLockStateList;
	}

	@Override
	/** onClick Listener for the ListItems in the Activity
	 * 
	 * @param l the ListView where the click happened
	 * @param v the view that was clicked within the ListView
	 * @param position the position of the view in the list
	 * @param id the row id of the item that was clicked 	 
	 * */
	protected void onListItemClick(ListView l, View v, final int position,
			long id) {

		AlertDialog.Builder builder = new AlertDialog.Builder(this);

		LayoutInflater factory = LayoutInflater.from(this);
		View view = factory.inflate(R.layout.wakelockdialog, null);

		TextView tipTextView = (TextView) view.findViewById(R.id.tip_help);
		tipTextView
				.setText(readHelpText(mTestList.get(position).split("#")[1]));

		DialogInterface.OnClickListener listener = new DialogInterface.OnClickListener() {

			public void onClick(DialogInterface dialog, int which) {
				if (which == DialogInterface.BUTTON_POSITIVE) {
					String configStr[] = mTestList.get(position).split("#");

					Intent intent = new Intent();
					intent.setClass(LockList.this, WakeLockActivity.class);

					intent.putExtra(WakeLockActivity.KEY_WAKE_LOCK_TYPE, configStr[1]);
					intent.putExtra(WakeLockActivity.KEY_TEST_MODE,
							WakeLockActivity.POWER_TEST_MANUALLY);

					startActivity(intent);
				}
			}
		};

		builder.setTitle("What will happened");
		builder.setPositiveButton("Confirm", listener);
		builder.setNegativeButton("Cancel", listener);

		AlertDialog createDialog = builder.create();

		createDialog.setView(view);
		createDialog.show();
	}

	/** Given a wakelock test name, this method returns a String of helpful instructions to be displayed before each test is run (taken from R.raw.wakelock)
	 * 
	 * @param tag a String representing a wakelock test (e.g. full, bright, dim, partial, winman)
	 * @return a string containing the help text that is displayed before that wakelock test is run
	 */
	public String readHelpText(String tag) {

		boolean start = false;

		StringBuffer textBuffer = new StringBuffer();

		InputStream is = this.getResources().openRawResource(R.raw.wakelock);
		BufferedReader br = new BufferedReader(new InputStreamReader(is));

		String line = null;
		try {
			while ((line = br.readLine()) != null) {

				if (line.indexOf("</" + tag + ">") != -1) {
					break;
				}

				if (start) {
					textBuffer.append(line + "\n");
				} else if (line.indexOf("<" + tag + ">") != -1) {
					start = true;
				}
			}

			br.close();
		} catch (IOException e) {
			Log.e("TestList", e.getMessage(), e);
		}

		return textBuffer.substring(0, textBuffer.length() - 1);
	}
}
