package adsl.meee.bluelock;

import android.app.ActionBar;
import android.app.Activity;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.content.Intent;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.view.KeyEvent;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.TextView.OnEditorActionListener;
import android.widget.Toast;

public class BluetoothActivity extends Activity {

	// Debugging
	private static final String TAG = "Bluetooth";
	private static final boolean D = true;

	// Message types sent from the BluetoothService Handler
	public static final int MESSAGE_STATE_CHANGE = 1;
	public static final int MESSAGE_READ = 2;
	public static final int MESSAGE_WRITE = 3;
	public static final int MESSAGE_DEVICE_NAME = 4;
	public static final int MESSAGE_TOAST = 5;

	// Key names received from the BluetoothService Handler
	public static final String DEVICE_NAME = "device_name";
	public static final String TOAST = "toast";

	// Intent request codes
	private static final int REQUEST_CONNECT_DEVICE_SECURE = 1;
	private static final int REQUEST_ENABLE_BT = 3;

	// Layout Views
	private Button mLockButton;
	private Button mUnlockButton;
	private Button mSetButton;
	private EditText mOldPassword;
	private EditText mNewPassword;
	private Button mCancelButton;
	
	// Acts as input buffer to handle async communication
	private String inMsg;
	
	// Used for storing the password
	public static final String PREFS_NAME = "savedData";
	private String lockpw = "";

	// Name of the connected device
	private String mConnectedDeviceName = null;
	// String buffer for outgoing messages
	private StringBuffer mOutStringBuffer;
	// Local Bluetooth adapter
	private BluetoothAdapter mBluetoothAdapter = null;
	// Member object for the services
	private BluetoothService mService = null;

	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		if (D)
			Log.e(TAG, "+++ ON CREATE +++");

		// Set up the window layout
		setContentView(R.layout.activity_bluetooth);
		
		setRequestedOrientation(1);

		// Load password from data
		SharedPreferences saves = getSharedPreferences(PREFS_NAME, 0);
		lockpw = saves.getString("lpw", "");
		
		// Get local Bluetooth adapter
		mBluetoothAdapter = BluetoothAdapter.getDefaultAdapter();

		// If the adapter is null, then Bluetooth is not supported
		if (mBluetoothAdapter == null) {
			Toast.makeText(this, "Bluetooth is not available",
					Toast.LENGTH_LONG).show();
			finish();
			return;
		}
	}

	@Override
	public void onStart() {
		super.onStart();
		if (D)
			Log.e(TAG, "++ ON START ++");

		// If BT is not on, request that it be enabled.
		// setup() will then be called during onActivityResult
		if (!mBluetoothAdapter.isEnabled()) {
			Intent enableIntent = new Intent(
					BluetoothAdapter.ACTION_REQUEST_ENABLE);
			startActivityForResult(enableIntent, REQUEST_ENABLE_BT);
			// Otherwise, setup the session
		} else {
			if (mService == null)
				setup();
		}
	}

	@Override
	public synchronized void onResume() {
		super.onResume();
		if (D)
			Log.e(TAG, "+ ON RESUME +");

		// Performing this check in onResume() covers the case in which BT was
		// not enabled during onStart(), so we were paused to enable it...
		// onResume() will be called when ACTION_REQUEST_ENABLE activity
		// returns.
		if (mService != null) {
			// Only if the state is STATE_NONE, do we know that we haven't
			// started already
			if (mService.getState() == BluetoothService.STATE_NONE) {
				// Start the Bluetooth services
				mService.start();
			}
		}
	}

	private void setup() {
		Log.d(TAG, "setup()");
		
		// Lock button listener
		mLockButton = (Button) findViewById(R.id.button_lock);
		mLockButton.setOnClickListener(new OnClickListener() {
			public void onClick(View v) {
				// Send a message using content of the edit text widget
				String message;
				// check if there is a set password
				if(lockpw != null && !lockpw.contentEquals("")) {
					message = "close " + lockpw + "\r\n";
					System.out.println("SEND: close "+lockpw);
				} else {
					message = "close\r\n";
					System.out.println("SEND: close with no password");
				}
				sendMessage(message);
			}
		});
		// Unlock button listener
		mUnlockButton = (Button) findViewById(R.id.button_unlock);
		mUnlockButton.setOnClickListener(new OnClickListener() {
			public void onClick(View v) {
				// Send a message using content of the edit text widget
				String message;
				if(lockpw != null && !lockpw.contentEquals("")) {
					message = "open " + lockpw + "\r\n";
					System.out.println("SEND: open "+lockpw);
				} else {
					message = "open\r\n";
					System.out.println("SEND: open with no password");
				}
				sendMessage(message);
			}
		});
		
		// Test/Reset button listener
		Button mTestButton = (Button) findViewById(R.id.button_test);
		mTestButton.setOnClickListener(new OnClickListener() {
			public void onClick(View v) {
				// Send a message using content of the edit text widget
				// Reset the lock password -- remove from release
				String message = "test reset\r\n";
				System.out.println("SEND: test");
				sendMessage(message);
			}
		});
		
		// display the "set password" view
		mSetButton = (Button) findViewById(R.id.btnPassword);
		mSetButton.setOnClickListener(new OnClickListener() {
			public void onClick(View v) {
				findViewById(R.id.passwordLayout).setVisibility(View.VISIBLE);
				v.setVisibility(View.GONE);
			}
		});
		
		// Change password, give old pw then change to new pw
		// Does not catch bad passwords, handled in mHandler:MESSAGE_READ
		mOldPassword = (EditText) findViewById(R.id.oldPassword);
		mNewPassword = (EditText) findViewById(R.id.newPassword);
		mNewPassword.setOnEditorActionListener(new OnEditorActionListener() {
			@Override
			public boolean onEditorAction(TextView v, int actionId,
					KeyEvent event) {

				String oldPW = mOldPassword.getText().toString();
				String newPW = mNewPassword.getText().toString();
				
				// Build string to be sent in 2 parts
				String sendPWString;
				// Don't set an old pw if none is provided
				if(oldPW.isEmpty()) {
					sendPWString = "set " + newPW + "\r\n";
				} else {
					sendPWString = "set " + oldPW + "\r\n" + newPW + "\r\n";
				}
				// update stored password to new password
				lockpw = newPW;
				
				System.out.println("SEND: "+sendPWString);
				sendMessage(sendPWString);
				
				// Reset the visual fields for the passwords and hide them
				findViewById(R.id.passwordLayout).setVisibility(View.GONE);
				mSetButton.setVisibility(View.VISIBLE);
				mOldPassword.setText("");
				mNewPassword.setText("");
				
				return false;
			}
		});
		
		// Don't set the password: clear pw fields and 'gone' the pw view
		mCancelButton = (Button) findViewById(R.id.btnCancelPassword);
		mCancelButton.setOnClickListener(new OnClickListener() {
			public void onClick(View v) {
				findViewById(R.id.passwordLayout).setVisibility(View.GONE);
				mSetButton.setVisibility(View.VISIBLE);
				mOldPassword.setText("");
				mNewPassword.setText("");
			}
		});

		// Initialize the BluetoothService to perform bluetooth connections
		mService = new BluetoothService(this, mHandler);

		// Initialize the buffer for outgoing messages
		mOutStringBuffer = new StringBuffer("");
	}

	@Override
	public synchronized void onPause() {
		super.onPause();
		if (D)
			Log.e(TAG, "- ON PAUSE -");
	}

	@Override
	public void onStop() {
		super.onStop();
		if (D)
			Log.e(TAG, "-- ON STOP --");
		//Toast.makeText(this, lockpw, Toast.LENGTH_SHORT).show();
		// Store the password to memory
		SharedPreferences saves = getSharedPreferences(PREFS_NAME, 0);
		SharedPreferences.Editor editor = saves.edit();
		editor.putString("lpw", lockpw);
		editor.commit();
	}

	@Override
	public void onDestroy() {
		super.onDestroy();
		// Stop the Bluetooth services
		if (mService != null)
			mService.stop();
		if (D)
			Log.e(TAG, "--- ON DESTROY ---");
	}

	/**
	 * Sends a message.
	 * 
	 * @param message
	 *            A string of text to send.
	 */
	private void sendMessage(String message) {
		// Check that we're actually connected before trying anything
		if (mService.getState() != BluetoothService.STATE_CONNECTED) {
			Toast.makeText(this, R.string.not_connected, Toast.LENGTH_SHORT)
					.show();
			return;
		}

		// Check that there's actually something to send
		if (message.length() > 0) {
			// Get the message bytes and tell the BluetoothService to write
			byte[] send = message.getBytes();
			mService.write(send);

			// Reset out string buffer to zero and clear the edit text field
			mOutStringBuffer.setLength(0);
		}
	}

	private final void setStatus(int resId) {
		final ActionBar actionBar = getActionBar();
		actionBar.setSubtitle(resId);
	}

	private final void setStatus(CharSequence subTitle) {
		final ActionBar actionBar = getActionBar();
		actionBar.setSubtitle(subTitle);
	}

	// The Handler that gets information back from the BluetoothService
	private final Handler mHandler = new Handler() {
		@Override
		public void handleMessage(Message msg) {
			switch (msg.what) {
			case MESSAGE_STATE_CHANGE:
				if (D)
					Log.i(TAG, "MESSAGE_STATE_CHANGE: " + msg.arg1);
				switch (msg.arg1) {
				case BluetoothService.STATE_CONNECTED:
					setStatus(getString(R.string.title_connected_to,
							mConnectedDeviceName));
					break;
				case BluetoothService.STATE_CONNECTING:
					setStatus(R.string.title_connecting);
					break;
				case BluetoothService.STATE_LISTEN:
				case BluetoothService.STATE_NONE:
					setStatus(R.string.title_not_connected);
					break;
				}
				break;
			case MESSAGE_WRITE:
				byte[] writeBuf = (byte[]) msg.obj;
				// construct a string from the buffer
				String writeMessage = new String(writeBuf);
				break;
			case MESSAGE_READ:
				byte[] readBuf = (byte[]) msg.obj;
				// construct a string from the valid bytes in the buffer
				String readMessage = new String(readBuf, 0, msg.arg1);
				if (inMsg == null)
					inMsg = "";
				
				inMsg += readMessage;
				if(inMsg.contains("\r\n")) {
					int eol = inMsg.indexOf("\r\n")+1;
					String subMsg = inMsg.substring(eol);
					inMsg = inMsg.substring(0,eol);
					System.out.print(inMsg+'\n');
					
					// Define did not accept the password, so clear it
					if(inMsg.contentEquals("\npassword not recognized\r")) {
						System.out.println(inMsg.compareTo("password not recognized\r"));
						Toast.makeText(getApplicationContext(), "Password is wrong. Clearing saved password.", Toast.LENGTH_SHORT).show();
						lockpw = ""; // clear stored password
					}
					
					//Toast.makeText(getApplicationContext(), inMsg, Toast.LENGTH_SHORT).show();
					inMsg = subMsg;
					subMsg = "";
				}
				//System.out.print(inMsg);
				break;
			case MESSAGE_DEVICE_NAME:
				// save the connected device's name
				mConnectedDeviceName = msg.getData().getString(DEVICE_NAME);
				Toast.makeText(getApplicationContext(),
						"Connected to " + mConnectedDeviceName,
						Toast.LENGTH_SHORT).show();
				break;
			
			case MESSAGE_TOAST:
				// Display a toast image
				Toast.makeText(getApplicationContext(),
						msg.getData().getString(TOAST), Toast.LENGTH_SHORT)
						.show();
				break;
			}
		}
	};

	public void onActivityResult(int requestCode, int resultCode, Intent data) {
		if (D)
			Log.d(TAG, "onActivityResult " + resultCode);
		switch (requestCode) {
		case REQUEST_CONNECT_DEVICE_SECURE:
			// When DeviceListActivity returns with a device to connect
			if (resultCode == Activity.RESULT_OK) {
				connectDevice(data, true);
			}
			break;
		case REQUEST_ENABLE_BT:
			// When the request to enable Bluetooth returns
			if (resultCode == Activity.RESULT_OK) {
				// Bluetooth is now enabled, so set up a session
				setup();
			} else {
				// User did not enable Bluetooth or an error occurred
				Log.d(TAG, "BT not enabled");
				Toast.makeText(this, R.string.bt_not_enabled_leaving,
						Toast.LENGTH_SHORT).show();
				finish();
			}
		}
	}

	private void connectDevice(Intent data, boolean secure) {
		// Get the device's MAC address
		String address = data.getExtras().getString(
				DeviceListActivity.EXTRA_DEVICE_ADDRESS);
		// Get the BluetoothDevice object
		BluetoothDevice device = mBluetoothAdapter.getRemoteDevice(address);
		// Attempt to connect to the device
		mService.connect(device, secure);
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		MenuInflater inflater = getMenuInflater();
		inflater.inflate(R.menu.menu, menu);
		return true;
	}

	@Override
	public boolean onOptionsItemSelected(MenuItem item) {
		Intent serverIntent = null;
		// Launch the DeviceListActivity to see devices and do scan
		serverIntent = new Intent(this, DeviceListActivity.class);
		startActivityForResult(serverIntent, REQUEST_CONNECT_DEVICE_SECURE);
		return true;
	}

}