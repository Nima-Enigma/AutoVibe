package com.autovibe;

import android.content.Intent;
import android.os.Bundle;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

import androidx.appcompat.app.AppCompatActivity;

import org.eclipse.paho.client.mqttv3.MqttException;


public class MQTTConnectActivity extends AppCompatActivity {
    private EditText editTextBrokerUrl, editTextPort, editTextClientId, editTextUsername, editTextPassword;
    private Button buttonConnect;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.mqtt_connection);

        editTextBrokerUrl = findViewById(R.id.editTextBrokerUrl);
        editTextPort = findViewById(R.id.editTextPort);
        editTextClientId = findViewById(R.id.editTextClientId);
        editTextUsername = findViewById(R.id.editTextUsername);
        editTextPassword = findViewById(R.id.editTextPassword);
        buttonConnect = findViewById(R.id.buttonConnect);

        buttonConnect.setOnClickListener(v -> connectToMqtt());
    }

    private void connectToMqtt() {
//        Intent intent = new Intent(this, MainActivity.class);
//        startActivity(intent);
        String brokerUrl = editTextBrokerUrl.getText().toString().trim();
        String portStr = editTextPort.getText().toString().trim();
        String clientId = editTextClientId.getText().toString().trim();
        String username = editTextUsername.getText().toString().trim();
        String password = editTextPassword.getText().toString().trim();

        if (brokerUrl.isEmpty() || portStr.isEmpty()) {
            Toast.makeText(this, "Broker URL and Port are required", Toast.LENGTH_SHORT).show();
            return;
        }

        String serverUri = "tcp://" + brokerUrl + ":" + portStr;

        MqttHandler mqttHandler = new MqttHandler();

        try {
            mqttHandler.connect(serverUri, clientId, username, password);

            // Pass mqttHandler instance via a singleton or shared class
            ConnectionHolder.setHandler(mqttHandler);

            // Switch activity
            Intent intent = new Intent(this, MainActivity.class);
            startActivity(intent);

        } catch (MqttException e) {
            e.printStackTrace();
            Toast.makeText(this, "MQTT connection failed: " + e.getMessage(), Toast.LENGTH_LONG).show();
        }

    }



}