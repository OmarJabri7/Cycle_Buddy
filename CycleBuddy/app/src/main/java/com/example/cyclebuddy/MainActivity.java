package com.example.cyclebuddy;

import androidx.appcompat.app.AppCompatActivity;
import android.os.Bundle;
import android.os.Handler;
import android.widget.TextView;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.ServerSocket;
import java.net.Socket;

public class MainActivity extends AppCompatActivity {

        private ServerSocket serverSocket;

        Handler updateConversationHandler;

        Thread serverThread = null;

        private TextView text;

        public static final int SERVERPORT = 8080;

        @Override
        public void onCreate(Bundle savedInstanceState) {

            super.onCreate(savedInstanceState);
            setContentView(R.layout.activity_main);

            text = (TextView) findViewById(R.id.text);

            updateConversationHandler = new Handler();

            this.serverThread = new Thread(new ServerThread());
            this.serverThread.start();

        }

        @Override
        protected void onStop() {
            super.onStop();
            try {
                serverSocket.close();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }

        class ServerThread implements Runnable {

            public void run() {
                Socket socket = null;
                try {
                    serverSocket = new ServerSocket(SERVERPORT);
                } catch (IOException e) {
                    e.printStackTrace();
                }
                while (!Thread.currentThread().isInterrupted()) {
                    try {

                        socket = serverSocket.accept();
                        BufferedReader input = new BufferedReader(new InputStreamReader(socket.getInputStream()));
                        String data = input.readLine();
//                        text.setText("Distance: " + data + "\n");
//                        CommunicationThread commThread = new CommunicationThread(socket);
//                        new Thread(commThread).start();
                        updateConversationHandler.post(new updateUIThread(data));
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                }
            }
        }

        class CommunicationThread implements Runnable {

            private Socket clientSocket;

            private BufferedReader input;

            public CommunicationThread(Socket clientSocket) {

                this.clientSocket = clientSocket;

                try {

                    this.input = new BufferedReader(new InputStreamReader(this.clientSocket.getInputStream()));

                } catch (IOException e) {
                    e.printStackTrace();
                }
            }

            public void run() {

                while (!Thread.currentThread().isInterrupted()) {

                    try {

                        String read = input.readLine();

                        updateConversationHandler.post(new updateUIThread(read));
//                        text.setText(text.getText().toString()+"Distance: "+ read + "\n");

                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                }
            }

        }

        class updateUIThread implements Runnable {
            private String msg;

            public updateUIThread(String str) {
                this.msg = str;
            }

            @Override
            public void run() {
                text.setText(text.getText().toString()+"Distance: "+ msg + "\n");
            }
        }
    }