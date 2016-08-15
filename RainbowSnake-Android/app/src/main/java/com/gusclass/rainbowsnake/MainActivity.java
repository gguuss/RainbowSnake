package com.gusclass.rainbowsnake;

import android.Manifest;
import android.app.DownloadManager;
import android.content.Context;
import android.content.pm.PackageManager;
import android.location.Location;
import android.location.LocationListener;
import android.location.LocationManager;
import android.media.AudioFormat;
import android.media.AudioRecord;
import android.media.MediaRecorder;
import android.os.AsyncTask;
import android.support.v4.app.ActivityCompat;
import android.support.v4.content.ContextCompat;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.CompoundButton;
import android.widget.ToggleButton;

import com.loopj.android.http.*;

import org.json.JSONArray;
import org.json.JSONObject;

import java.io.IOException;
import java.util.Timer;
import java.util.TimerTask;

import static java.lang.Math.*;

import cz.msebera.android.httpclient.Header;

public class MainActivity extends AppCompatActivity {

    static final String TAG = "RainbowSnakeMain";

    LocationManager lm;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        lm = (LocationManager)getSystemService(Context.LOCATION_SERVICE);

        Button buttonSavePos = (Button) findViewById(R.id.savePositionButton);
        buttonSavePos.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                storeCurrentLocation();
            }
        });

        Button buttonCompass = (Button) findViewById(R.id.compassButton);
        buttonCompass.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                Log.d(TAG, "Compass clicked");
                setPattern(27);
            }
        });

        Button buttonDist = (Button) findViewById(R.id.distButton);
        buttonDist.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                Log.d(TAG, "Dist clicked");
                setPattern(28);
            }
        });

        ToggleButton toggleVu = (ToggleButton) findViewById(R.id.vuButton);
        toggleVu.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                if (isChecked) {
                    Log.d(TAG, "Toggle VU to on");
                    try {
                        startVu();
                    }catch (IOException ioe) {
                        Log.e(TAG, "Couldn't start VU", ioe);
                    }
                } else {
                    Log.d(TAG, "Toggle VU to off");
                    stopVu();
                }
            }
        });


        ToggleButton toggleOff = (ToggleButton) findViewById(R.id.toggleOffButton);
        toggleOff.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                if (isChecked) {
                    Log.d(TAG, "Toggle power to on");
                    RequestParams params = new RequestParams();
                    params.add("value", "1");
                    RainbowSnakeRestClient.post("power", params, new JsonHttpResponseHandler() {
                        @Override
                        public void onSuccess(int statusCode, Header[] headers, JSONObject response) {
                            // If the response is JSONObject instead of expected JSONArray
                            Log.d(TAG, "Success, JSON OBJECT receieved");
                        }

                        @Override
                        public void onSuccess(int statusCode, Header[] headers, JSONArray timeline) {
                            // Pull out the first event on the public timeline
                            Log.d(TAG, "Success, JSON Array receieved");
                        }

                        @Override
                        public void onFailure(int statusCode, Header[] headers, String responseString, Throwable throwable) {
                            Log.d(TAG, "Failure: " + statusCode + " / " + responseString);
                        }
                    });
                } else {
                    // The toggle is disabled
                    Log.d(TAG, "Toggle power to off");
                    RequestParams params = new RequestParams();
                    params.add("value", "0");
                    RainbowSnakeRestClient.post("power", params, new JsonHttpResponseHandler() {
                        @Override
                        public void onSuccess(int statusCode, Header[] headers, JSONObject response) {
                            // If the response is JSONObject instead of expected JSONArray
                            Log.d(TAG, "Success, JSON OBJECT receieved");
                        }

                        @Override
                        public void onSuccess(int statusCode, Header[] headers, JSONArray timeline) {
                            // Pull out the first event on the public timeline
                            Log.d(TAG, "Success, JSON Array receieved");
                        }

                        @Override
                        public void onFailure(int statusCode, Header[] headers, String responseString, Throwable throwable) {
                            Log.d(TAG, "Failure: " + statusCode + " / " + responseString);
                        }
                    });
                }
            }
        });

        ToggleButton toggleLatLong = (ToggleButton) findViewById(R.id.latlong);
        toggleLatLong.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                Log.d(TAG, "onCheckedChanged : " + isChecked);
                toggleLocationUpdates(isChecked);
            }
        });
    }

    void setPattern(int patternIndex) {
        Log.d(TAG, "Set pattern to: " + patternIndex);
        RequestParams params = new RequestParams();
        params.add("value", String.valueOf(patternIndex));
        RainbowSnakeRestClient.post("pattern", params, new JsonHttpResponseHandler() {
            @Override
            public void onSuccess(int statusCode, Header[] headers, JSONObject response) {
                // If the response is JSONObject instead of expected JSONArray
                Log.d(TAG, "Success, JSON OBJECT receieved");
            }

            @Override
            public void onSuccess(int statusCode, Header[] headers, JSONArray timeline) {
                // Pull out the first event on the public timeline
                Log.d(TAG, "Success, JSON Array receieved");
            }

            @Override
            public void onFailure(int statusCode, Header[] headers, String responseString, Throwable throwable) {
                Log.d(TAG, "Failure: " + statusCode + " / " + responseString);
            }
        });
    }

    private AudioRecord mRecorder = null;
    AsyncTask<Integer, Integer, Void> apoll;
    void startVu() throws IOException{
        if (true) { //validateMicAvailability()) {

            int mRecorderBufSize = AudioRecord.getMinBufferSize(44100,
                    AudioFormat.CHANNEL_IN_MONO,
                    AudioFormat.ENCODING_PCM_16BIT);
            mRecorder = new AudioRecord(MediaRecorder.AudioSource.MIC,
                    44100,
                    AudioFormat.CHANNEL_IN_MONO,
                    AudioFormat.ENCODING_PCM_16BIT,
                    mRecorderBufSize);
            mRecorder.startRecording();

            apoll = new AudioPoll().execute(mRecorderBufSize);;
        } else {
            Log.d(TAG, "No mic available");
        }
    }
    private class AudioPoll extends AsyncTask<Integer, Integer, Void> {
        protected Void doInBackground(Integer... sizeInBytes) {
            short[] buffer = new short[sizeInBytes[0]/2];
            boolean status = true;
            int shortsRead;

            while (status && mRecorder.getRecordingState() == AudioRecord.RECORDSTATE_RECORDING) {
                if ((shortsRead = mRecorder.read(buffer, 0, buffer.length)) > 0) {
                    int pmax = 1;
                    int nmin = 0;
                    final double LOG_2 = log(2.0);
                    int i;

                    // find the sample with the largest excursion in this slice
                    for (i = 0; i < shortsRead; ++i) {
                        if (buffer[i] > 0) {
                            pmax = max(pmax, (int)buffer[i]);
                        } else {
                            nmin = min(nmin, (int)buffer[i]);
                        }
                    }
                    nmin = -max(nmin, -((int)Short.MAX_VALUE)); // make sure nmin is in [0..Short.MAX_VALUE]
                    pmax = max(pmax, nmin);

                    double lb_pmax = log((double)pmax)/LOG_2; // lb_pmax [0..15)
                    double c_lb_pmax = ceil(lb_pmax); // c_lb_pmax [0..15]
                    publishProgress(Integer.valueOf(((int)c_lb_pmax+1)*2 - (int)round(c_lb_pmax - lb_pmax)));
                } else if (shortsRead < 0) {
                    // error
                    publishProgress(Integer.valueOf(0));
                    status = false;
                } else {
                    publishProgress(Integer.valueOf(0));
                    status = false;
                }
            }
            mRecorder = null;
            return null;
        }

        int mThrottleCounter = 0;
        static final int throttleLimit = 10;
        protected void onProgressUpdate(Integer... progress) {
            // Post it
            RequestParams params = new RequestParams();
            int vuValue = (progress[0] * 100) / 32; // scale between 0 ... 100
            if (vuValue > 60) {
                vuValue = (vuValue - 60); // squash to 0 ... 40
                vuValue *= 100 / 30;
            } else {
                vuValue = 0;
            }
            params.add("value", String.valueOf(vuValue));
            if (mThrottleCounter % throttleLimit == 0) {
                RainbowSnakeRestClient.post("vumeter", params, new JsonHttpResponseHandler() {
                    @Override
                    public void onSuccess(int statusCode, Header[] headers, JSONObject response) {
                        // If the response is JSONObject instead of expected JSONArray
                        //Log.d(TAG, "Success, JSON OBJECT receieved");
                    }

                    @Override
                    public void onSuccess(int statusCode, Header[] headers, JSONArray timeline) {
                        // Pull out the first event on the public timeline
                        //Log.d(TAG, "Success, JSON Array receieved");
                    }

                    @Override
                    public void onFailure(int statusCode, Header[] headers, String responseString, Throwable throwable) {
                        //Log.d(TAG, "Failure: " + statusCode + " / " + responseString);
                    }
                });
            }
            mThrottleCounter++;
        }
    }

    void stopVu() {
        apoll.cancel(true);
        mRecorder.stop();
    }



    /* GPS Constant Permission */
    private static final int MY_PERMISSION_ACCESS_FINE_LOCATION = 12;
    public void toggleLocationUpdates(boolean isChecked) {
        // API 23: we have to check if ACCESS_FINE_LOCATION and/or ACCESS_COARSE_LOCATION permission are granted
        if (ContextCompat.checkSelfPermission(this, android.Manifest.permission.ACCESS_FINE_LOCATION) == PackageManager.PERMISSION_GRANTED) {
            if (isChecked) {
                // The toggle is enabled
                Log.d(TAG, "Toggle send latlong on");
                lm.requestLocationUpdates(LocationManager.GPS_PROVIDER, 2000, 10, locationListener);
                Location location = lm.getLastKnownLocation(LocationManager.GPS_PROVIDER);
                double longitude = location.getLongitude();
                double latitude = location.getLatitude();
                float bearing = location.getBearing();
                // Send last location
                sendLocation(latitude, longitude, bearing);
            } else {
                // The toggle is disabled
                Log.d(TAG, "Toggle send latlong off ");
                lm.removeUpdates(locationListener);
            }
        } else {         // One or both permissions are denied.
            // The ACCESS_FINE_LOCATION is denied, then I request it and manage the result in
            // onRequestPermissionsResult() using the constant MY_PERMISSION_ACCESS_FINE_LOCATION
            if (ContextCompat.checkSelfPermission(this, Manifest.permission.ACCESS_FINE_LOCATION) != PackageManager.PERMISSION_GRANTED ) {
                ActivityCompat.requestPermissions(this,
                        new String[] { Manifest.permission.ACCESS_FINE_LOCATION },
                        MY_PERMISSION_ACCESS_FINE_LOCATION);
            }

        }
    }

    public void storeCurrentLocation(){
        // API 23: we have to check if ACCESS_FINE_LOCATION and/or ACCESS_COARSE_LOCATION permission are granted
        if (ContextCompat.checkSelfPermission(this, android.Manifest.permission.ACCESS_FINE_LOCATION) == PackageManager.PERMISSION_GRANTED) {
            // The toggle is enabled
            Log.d(TAG, "Storing current location");
            lm.requestLocationUpdates(LocationManager.GPS_PROVIDER, 2000, 10, locationListener);
            Location location = lm.getLastKnownLocation(LocationManager.GPS_PROVIDER);
            double longitude = location.getLongitude();
            double latitude = location.getLatitude();
            // store last location
            storeLocation(latitude, longitude);
        } else {         // One or both permissions are denied.
            // The ACCESS_FINE_LOCATION is denied, then I request it and manage the result in
            // onRequestPermissionsResult() using the constant MY_PERMISSION_ACCESS_FINE_LOCATION
            if (ContextCompat.checkSelfPermission(this, Manifest.permission.ACCESS_FINE_LOCATION) != PackageManager.PERMISSION_GRANTED ) {
                ActivityCompat.requestPermissions(this,
                        new String[] { Manifest.permission.ACCESS_FINE_LOCATION },
                        MY_PERMISSION_ACCESS_FINE_LOCATION);
            }

        }
    }

    public void storeLocation(double latitude, double longitude) {
        RequestParams params = new RequestParams();
        params.add("lat", String.valueOf(latitude));
        params.add("long", String.valueOf(longitude));
        RainbowSnakeRestClient.post("latlong", params, new JsonHttpResponseHandler() {
            @Override
            public void onSuccess(int statusCode, Header[] headers, JSONObject response) {
                // If the response is JSONObject instead of expected JSONArray
                Log.d(TAG, "Success, JSON OBJECT receieved");
            }

            @Override
            public void onSuccess(int statusCode, Header[] headers, JSONArray timeline) {
                // Pull out the first event on the public timeline
                Log.d(TAG, "Success, JSON Array receieved");
            }

            @Override
            public void onFailure(int statusCode, Header[] headers, String responseString, Throwable throwable) {
                Log.d(TAG, "Failure: " + statusCode + " / " + responseString);
            }
        });
    }

    public void sendLocation(double latitude, double longitude, float bearing){
        RequestParams params = new RequestParams();
        params.add("lat", String.valueOf(latitude));
        params.add("long", String.valueOf(longitude));
        params.add("heading", String.valueOf(bearing));
        RainbowSnakeRestClient.post("currposition", params, new JsonHttpResponseHandler() {
            @Override
            public void onSuccess(int statusCode, Header[] headers, JSONObject response) {
                // If the response is JSONObject instead of expected JSONArray
                Log.d(TAG, "Success, JSON OBJECT receieved");
            }

            @Override
            public void onSuccess(int statusCode, Header[] headers, JSONArray timeline) {
                // Pull out the first event on the public timeline
                Log.d(TAG, "Success, JSON Array receieved");
            }

            @Override
            public void onFailure(int statusCode, Header[] headers, String responseString, Throwable throwable) {
                Log.d(TAG, "Failure: " + statusCode + " / " + responseString);
            }
        });
    }

    private final LocationListener locationListener = new LocationListener() {
        public void onLocationChanged(Location location) {
            Log.d(TAG, "Location changed");
            double longitude = location.getLongitude();
            double latitude = location.getLatitude();
            float bearing = location.getBearing();
            sendLocation(latitude, longitude, bearing);
        }

        @Override
        public void onStatusChanged(String provider, int status, Bundle extras) {

        }

        @Override
        public void onProviderDisabled(String provider) {

        }

        @Override
        public void onProviderEnabled(String provider) {

        }
    };
}
