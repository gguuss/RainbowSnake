package com.gusclass.rainbowsnake;

import android.app.DownloadManager;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.widget.CompoundButton;
import android.widget.ToggleButton;

import com.loopj.android.http.*;

import org.json.JSONArray;
import org.json.JSONObject;

import cz.msebera.android.httpclient.Header;

public class MainActivity extends AppCompatActivity {

    static final String TAG = "RainbowSnakeMain";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        ToggleButton toggleOff = (ToggleButton) findViewById(R.id.toggleOffButton);
        toggleOff.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                if (isChecked) {
                    Log.d(TAG, "Toggle power to off");
                    RequestParams params = new RequestParams();
                    params.add("value", "1");
                    RainbowSnakeRestClient.post("power", params, new JsonHttpResponseHandler() {
                        @Override
                        public void onSuccess(int statusCode, Header[] headers, JSONObject response) {
                            // If the response is JSONObject instead of expected JSONArray
                        }

                        @Override
                        public void onSuccess(int statusCode, Header[] headers, JSONArray timeline) {
                            // Pull out the first event on the public timeline

                        }

                    });
                } else {
                    // The toggle is disabled
                    Log.d(TAG, "Toggle power to off");
                }
            }
        });

        ToggleButton toggle = (ToggleButton) findViewById(R.id.latlong);
        toggle.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                if (isChecked) {
                    // The toggle is enabled
                    Log.d(TAG, "Toggle send latlong on");
                } else {
                    // The toggle is disabled
                    Log.d(TAG, "Toggle send latlong off ");
                }
            }
        });
    }
}
