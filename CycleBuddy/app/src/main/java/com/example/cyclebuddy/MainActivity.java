package com.example.cyclebuddy;

import androidx.appcompat.app.AppCompatActivity;

import android.graphics.Color;
import android.os.Bundle;
import android.os.Handler;
import android.widget.TextView;

import com.github.mikephil.charting.charts.LineChart;
import com.github.mikephil.charting.components.YAxis;
import com.github.mikephil.charting.data.Entry;
import com.github.mikephil.charting.data.LineData;
import com.github.mikephil.charting.data.LineDataSet;
import com.github.mikephil.charting.interfaces.datasets.ILineDataSet;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.ServerSocket;
import java.net.Socket;
import java.nio.channels.FileLock;

public class MainActivity extends AppCompatActivity {

        private ServerSocket serverSocket;

        Handler updateConversationHandler;

        Thread serverThread = null;

        private LineChart distance_chart;

        public static final int SERVERPORT = 8080;

        private boolean plot_data = true;

        @Override
        public void onCreate(Bundle savedInstanceState) {

            super.onCreate(savedInstanceState);
            setContentView(R.layout.activity_main);

            distance_chart = (LineChart) findViewById(R.id.distance_chart);

            distance_chart.getDescription().setEnabled(true);
            distance_chart.getDescription().setText("Real Time distance data");
            distance_chart.setTouchEnabled(false);
            distance_chart.setDragEnabled(false);
            distance_chart.setScaleEnabled(false);
            YAxis leftYAxis = distance_chart.getAxisLeft();
            leftYAxis.setEnabled(false);
            distance_chart.setDrawGridBackground(false);
            distance_chart.setPinchZoom(false);
            distance_chart.setBackgroundColor(Color.WHITE);
            LineData distance_data = new LineData();
            distance_data.setValueTextColor(Color.WHITE);
            distance_chart.setData(distance_data);
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

        private void addEntry(String readings){
            LineData data = distance_chart.getData();
            if(data != null){
                ILineDataSet set = data.getDataSetByIndex(0);
                if(set == null){
                    set = createSet();
                    data.addDataSet(set);
                }
                data.addEntry(new Entry(set.getEntryCount(),Float.parseFloat(readings)),0);
                data.notifyDataChanged();
                distance_chart.notifyDataSetChanged();
                distance_chart.setMaxVisibleValueCount(150);
                distance_chart.setData(data);
                distance_chart.moveViewToX(set.getEntryCount());
                YAxis right = distance_chart.getAxisRight();
                right.setAxisMaximum(Float.parseFloat(readings) + 10);
                right.setAxisMinimum(Float.parseFloat(readings) - 10);
            }
        }

        private LineDataSet createSet(){
            LineDataSet set = new LineDataSet(null, "Ultrasonic Data");
            set.setAxisDependency(YAxis.AxisDependency.RIGHT);
            set.setLineWidth(3);
            set.setColor(Color.MAGENTA);
            set.setMode(LineDataSet.Mode.CUBIC_BEZIER);
            set.setCubicIntensity(0.2f);
            return set;
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
                        addEntry(data);
//                        updateConversationHandler.post(new updateUIThread(data));
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
                addEntry(this.msg);
            }
        }
    }