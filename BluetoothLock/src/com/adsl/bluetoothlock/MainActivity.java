package com.adsl.bluetoothlock;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.Set;
import java.util.UUID;

import android.app.Activity;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.view.Menu;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;

public class MainActivity extends Activity {
	protected static final int DISCOVERY_REQUEST = 0;

	private BluetoothAdapter btAdapter;

	private ConnectedThread connectedThread;
	
	public TextView statusUpdate;
	public Button connect;
	public Button disconnect;
	public ImageView icon;
	public String toastText = "";
	private String lockName = "linvor";
	private BluetoothDevice lockDevice;
	public static final UUID MY_UUID = UUID
			.fromString("00001101-0000-1000-8000-00805F9B34FB");

	protected static final int SUCCESS_CONNECT = 0;

	public static final int MESSAGE_READ = 1;

	public boolean stillLooking = true;

	Handler mHandler = new Handler() {
		public void handleMessage(Message msg) {
			super.handleMessage(msg);
			switch (msg.what) {
			case SUCCESS_CONNECT:
				// Do something
				connectedThread = new ConnectedThread((BluetoothSocket)msg.obj);
				Toast.makeText(getApplicationContext(), "CONNECT", 0).show();
				//String tempS = "locks";
				//connectedThread.write(tempS.getBytes());
				connectedThread.start();
				break;
			case MESSAGE_READ:
				//System.out.println("At Message_Read");
				byte[] readBuf = (byte[])msg.obj;
				String s =  new String(readBuf, 0, msg.arg1);
				System.out.print(s);
				//Toast.makeText(getApplicationContext(), s, Toast.LENGTH_SHORT).show();
				break;
			}
		}
	};

	// Create a BroadcastReceiver to receive state changes
	BroadcastReceiver bluetoothState = new BroadcastReceiver() {
		@Override
		public void onReceive(Context context, Intent intent) {
			// String prevStateExtra = BluetoothAdapter.EXTRA_PREVIOUS_STATE;
			String stateExtra = BluetoothAdapter.EXTRA_STATE;
			int state = intent.getIntExtra(stateExtra, -1);
			// int previousState = intent.getIntExtra(prevStateExtra, -1);
			switch (state) {
			case (BluetoothAdapter.STATE_TURNING_ON): {
				toastText = "Bluetooth turning on";
				Toast.makeText(MainActivity.this, toastText, Toast.LENGTH_SHORT)
						.show();
				break;
			}
			case (BluetoothAdapter.STATE_ON): {
				toastText = "Bluetooth on";
				Toast.makeText(MainActivity.this, toastText, Toast.LENGTH_SHORT)
						.show();
				setupUI();
				break;
			}
			case (BluetoothAdapter.STATE_TURNING_OFF): {
				toastText = "Bluetooth turning off";
				Toast.makeText(MainActivity.this, toastText, Toast.LENGTH_SHORT)
						.show();
				break;

			}

			case (BluetoothAdapter.STATE_OFF): {
				toastText = "Bluetooth off";
				Toast.makeText(MainActivity.this, toastText, Toast.LENGTH_SHORT)
						.show();
				setupUI();
				break;
			}

			/*default: {
				if (BluetoothAdapter.ACTION_DISCOVERY_FINISHED.equals(intent
						.getAction())) {
					System.out.println("DISCOVERY FINISHED");
					toastText = "Done discovering";
					Toast.makeText(MainActivity.this, toastText,
							Toast.LENGTH_SHORT).show();
					stillLooking = false;

				}
				break;
			}*/

			}
		}
	};

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		setupUI();
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.activity_main, menu);
		return true;
	}

	public void setupUI() {
		// get references to activity_main items
		final TextView statusUpdate = (TextView) findViewById(R.id.result);
		final Button connect = (Button) findViewById(R.id.btnConnect);
		final Button disconnect = (Button) findViewById(R.id.btnDisconnect);
		final ImageView icon = (ImageView) findViewById(R.id.blueIcon);
		final Button openLock = (Button) findViewById(R.id.btnOpen);
		final Button closeLock = (Button) findViewById(R.id.btnClose);

		// set display view
		disconnect.setVisibility(View.GONE);
		icon.setVisibility(View.GONE);
		openLock.setVisibility(View.GONE);
		closeLock.setVisibility(View.GONE);

		btAdapter = BluetoothAdapter.getDefaultAdapter();
		if (btAdapter.isEnabled()) {
			String address = btAdapter.getAddress();
			String name = btAdapter.getName();
			String statusText = name + " : " + address;
			statusUpdate.setText(statusText);
			disconnect.setVisibility(View.VISIBLE);
			icon.setVisibility(View.VISIBLE);
			openLock.setVisibility(View.VISIBLE);
			closeLock.setVisibility(View.VISIBLE);
			connect.setVisibility(View.GONE);

		} else {
			connect.setVisibility(View.VISIBLE);
			disconnect.setVisibility(View.GONE);
			icon.setVisibility(View.GONE);
			openLock.setVisibility(View.GONE);
			closeLock.setVisibility(View.GONE);
			statusUpdate.setText("Bluetooth is not on");
		}

		connect.setOnClickListener(new OnClickListener() {

			@Override
			public void onClick(View v) {
				String scanModeChanged = BluetoothAdapter.ACTION_SCAN_MODE_CHANGED;
				String scanAction = BluetoothAdapter.ACTION_DISCOVERY_FINISHED;
				String beDiscoverable = BluetoothAdapter.ACTION_REQUEST_DISCOVERABLE;
				IntentFilter filter = new IntentFilter(scanModeChanged);
				IntentFilter actionFilter = new IntentFilter(scanAction);
				registerReceiver(bluetoothState, filter);
				registerReceiver(bluetoothState, actionFilter);
				startActivityForResult(new Intent(beDiscoverable),
						DISCOVERY_REQUEST);

			}
		}); // end connect onClickListener

		disconnect.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View v) {
				btAdapter.disable();
				connect.setVisibility(View.VISIBLE);
				disconnect.setVisibility(View.GONE);
				icon.setVisibility(View.GONE);
				openLock.setVisibility(View.GONE);
				closeLock.setVisibility(View.GONE);
				statusUpdate.setText("Bluetooth Off");

			}
		}); // end disconnect onClickListener
		
		openLock.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View v) {
				String tempS = "open\r\n";
				connectedThread.write(tempS.getBytes());
			}
		});
		
		closeLock.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View v) {
				String tempS = "close\r\n";
				connectedThread.write(tempS.getBytes());
			}
		});
	} // end setupUI

	@Override
	protected void onActivityResult(int requestCode, int resultCode, Intent date) {
		if (requestCode == DISCOVERY_REQUEST) {
			Toast.makeText(MainActivity.this, "Discovery in progress",
					Toast.LENGTH_SHORT).show();
			setupUI();
			stillLooking = true;
			findDevices();
			
			while(btAdapter.isDiscovering()){}

			if (lockDevice != null) {
				launchConnection();
			} else {
				System.out.println("failed launchConnection: lock not found");
				Toast.makeText(MainActivity.this, "The lock was not found.",
						Toast.LENGTH_SHORT).show();
			}

		}
	}

	private void findDevices() {

		toastText = "Checking for known paired devices";
		Toast.makeText(MainActivity.this, toastText, Toast.LENGTH_SHORT).show();

		Set<BluetoothDevice> pairedDevices = btAdapter.getBondedDevices();

		if (pairedDevices.size() > 0) {
			for (BluetoothDevice pairedDevice : pairedDevices) {
				toastText = "Found device: " + pairedDevice.getName();
				System.out.println(pairedDevice.getName());
				if (lockName.equals(pairedDevice.getName())) {
					lockDevice = pairedDevice;
					toastText += " = Magic Lock";
					System.out.println("Found the lock, paired");
					stillLooking = false;
				}
				Toast.makeText(MainActivity.this, toastText, Toast.LENGTH_SHORT)
						.show();
			}
		}

		if (lockDevice == null) {
			toastText = "Starting discovery for remote devices...";
			Toast.makeText(MainActivity.this, toastText, Toast.LENGTH_SHORT)
					.show();
			// start discovery
			if (btAdapter.startDiscovery()) {
				toastText = "Discovery thread started... Scanning for Devices";
				Toast.makeText(MainActivity.this, toastText, Toast.LENGTH_SHORT)
						.show();
				registerReceiver(discoveryResult, new IntentFilter(
						BluetoothDevice.ACTION_FOUND));
			}
		}
	} // end find devices

	private void launchConnection() {
		ConnectThread connect = new ConnectThread(lockDevice);
		connect.start();
	}

	// Create BroadcastReceiver to receive device discovery
	BroadcastReceiver discoveryResult = new BroadcastReceiver() {
		@Override
		public void onReceive(Context context, Intent intent) {
			String remoteDeviceName = intent
					.getStringExtra(BluetoothDevice.EXTRA_NAME);
			BluetoothDevice remoteDevice;
			remoteDevice = intent
					.getParcelableExtra(BluetoothDevice.EXTRA_DEVICE);

			if (lockName.equals(remoteDeviceName)) {
				lockDevice = remoteDevice;
				System.out.println("Found the lock, unpaired");
				stillLooking = false;
			}
			toastText = "Discovered: " + remoteDeviceName;
			Toast.makeText(MainActivity.this, toastText, Toast.LENGTH_SHORT)
					.show();
			// statusUpdate.setText(statusText);
		}
	};

	private class ConnectThread extends Thread {
		private final BluetoothSocket mmSocket;
		private final BluetoothDevice mmDevice;

		public ConnectThread(BluetoothDevice device) {
			// Use a temporary object that is later assigned to mmSocket,
			// because mmSocket is final
			BluetoothSocket tmp = null;
			mmDevice = device;

			// Get a BluetoothSocket to connect with the given BluetoothDevice
			try {
				// MY_UUID is the app's UUID string, also used by the server
				// code
				tmp = device.createRfcommSocketToServiceRecord(MY_UUID);
			} catch (IOException e) {
			}
			System.out.println("Connection Thread");
			mmSocket = tmp;
		}

		public void run() {
			// Cancel discovery because it will slow down the connection
			btAdapter.cancelDiscovery();

			try {
				// Connect the device through the socket. This will block
				// until it succeeds or throws an exception
				mmSocket.connect();
			} catch (IOException connectException) {
				// Unable to connect; close the socket and get out
				try {
					mmSocket.close();
				} catch (IOException closeException) {
				}
				return;
			}

			// Do work to manage the connection (in a separate thread)
			mHandler.obtainMessage(SUCCESS_CONNECT, mmSocket).sendToTarget();
		}

		/** Will cancel an in-progress connection, and close the socket */
		public void cancel() {
			try {
				mmSocket.close();
			} catch (IOException e) {
			}
		}
	}
	
	private class ConnectedThread extends Thread {
	    private final BluetoothSocket mmSocket;
	    private final InputStream mmInStream;
	    private final OutputStream mmOutStream;
	 
	    public ConnectedThread(BluetoothSocket socket) {
	        mmSocket = socket;
	        InputStream tmpIn = null;
	        OutputStream tmpOut = null;
	 
	        // Get the input and output streams, using temp objects because
	        // member streams are final
	        try {
	            tmpIn = socket.getInputStream();
	            tmpOut = socket.getOutputStream();
	        } catch (IOException e) { }
	 
	        mmInStream = tmpIn;
	        mmOutStream = tmpOut;
	    }
	 
	    public void run() {
            //byte[] buffer = new byte[1024];
	    	byte[] buffer;
            int bytes; // bytes returned from read()
	 
	        // Keep listening to the InputStream until an exception occurs
	        while (true) {
	            try {
	                // Read from the InputStream
	            	buffer = new byte[1024];
	            	bytes = mmInStream.read(buffer);
	            	// Send the obtained bytes to the UI activity
	                mHandler.obtainMessage(MESSAGE_READ, bytes, -1, buffer)
	                        .sendToTarget();
	            } catch (IOException e) {
	                break;
	            }
	        }
	    }
	 
	    /* Call this from the main activity to send data to the remote device */
	    public void write(byte[] bytes) {
	        try {
	        	//System.out.println("" + ((char) bytes[3]) + ((char) bytes[4]+32));// + (char) bytes[5]+32);
	        	mmOutStream.write(bytes);

	        } catch (IOException e) { }
	    }
	 
	    /* Call this from the main activity to shutdown the connection */
	    public void cancel() {
	        try {
	            mmSocket.close();
	        } catch (IOException e) { }
	    }
	}
}
