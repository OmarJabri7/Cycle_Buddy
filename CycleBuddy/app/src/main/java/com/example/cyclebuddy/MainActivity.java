package com.example.cyclebuddy;

import androidx.appcompat.app.AppCompatActivity;

import android.graphics.Color;
import android.os.AsyncTask;
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
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

public class MainActivity extends AppCompatActivity {

        private ServerSocket serverSocket;

        Handler updateConversationHandler;

        ServerThread serverThreadSonar = null;

        ServerThread serverThreadHall = null;

        private LineChart distance_chart;

        private LineChart vel_chart;

        public static final int SERVERPORT_SONAR = 8080;

        public static final int SERVERPORT_HALL = 6000;

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

            vel_chart = (LineChart) findViewById(R.id.vel_chart);

            vel_chart.getDescription().setEnabled(true);
            vel_chart.getDescription().setText("Real Time velocity data");
            vel_chart.setTouchEnabled(false);
            vel_chart.setDragEnabled(false);
            vel_chart.setScaleEnabled(false);
            YAxis leftYAxisHall = vel_chart.getAxisLeft();
            leftYAxis.setEnabled(false);
            vel_chart.setDrawGridBackground(false);
            vel_chart.setPinchZoom(false);
            vel_chart.setBackgroundColor(Color.WHITE);
            LineData vel_data = new LineData();
            distance_data.setValueTextColor(Color.WHITE);
            vel_chart.setData(vel_data);

//            updateConversationHandler = new Handler();

            try {
                this.serverThreadSonar = new ServerThread(SERVERPORT_SONAR,
                            "SONAR",
                            distance_chart,2);
            } catch (IOException ioException) {
                ioException.printStackTrace();
            }

            try {
                this.serverThreadHall = new ServerThread(SERVERPORT_HALL,
                                "HALL",
                                vel_chart,2);
            } catch (IOException ioException) {
                ioException.printStackTrace();
            }
            new Thread(this.serverThreadSonar).start();
            new Thread(this.serverThreadHall).start();
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

        private void addEntry(String readings, String sensorType, LineChart chart){
            LineData data = chart.getData();
            if(data != null){
                ILineDataSet set = data.getDataSetByIndex(0);
                if(set == null){
                    set = createSet(sensorType);
                    data.addDataSet(set);
                }
                data.addEntry(new Entry(set.getEntryCount(),Float.parseFloat(readings)),0);
                data.notifyDataChanged();
                chart.notifyDataSetChanged();
                chart.setMaxVisibleValueCount(150);
                chart.setData(data);
                chart.moveViewToX(set.getEntryCount());
                YAxis right = chart.getAxisRight();
                if(sensorType == "SONAR") {
                    right.setAxisMaximum(Float.parseFloat(readings) + 10);
                    right.setAxisMinimum(Float.parseFloat(readings) - 10);
                }
                else if(sensorType == "HALL"){
                    right.setAxisMaximum(Float.parseFloat(readings) + 20);
                    right.setAxisMinimum(Float.parseFloat(readings) - 20);
                }
            }
        }

        private LineDataSet createSet(String sensorType){
            LineDataSet set = null;
            if(sensorType.equals("SONAR")) {
                set = new LineDataSet(null, "Ultrasonic Data");
                set.setColor(Color.RED);
            }
            else {
                set = new LineDataSet(null, "Hall Effect Data");
                set.setColor(Color.GREEN);
            }
            set.setAxisDependency(YAxis.AxisDependency.RIGHT);
            set.setLineWidth(3);
            set.setMode(LineDataSet.Mode.CUBIC_BEZIER);
            set.setCubicIntensity(0.2f);
            return set;
        }



        class ServerThread implements Runnable {
            String sensorType;
            LineChart chart;
            ServerSocket serverSocket;
            private final ExecutorService pool;

           public ServerThread(int port, String sensorType, LineChart chart, int poolSize) throws IOException {
                this.sensorType = sensorType;
                this.chart = chart;
                this.serverSocket = new ServerSocket(port);
                pool = Executors.newFixedThreadPool(poolSize);
            }

            public void run() {
                Socket socket = null;
                try {
                    for(;;){
                        pool.execute(new Handler(serverSocket.accept(),sensorType,chart));
                    }
//                    serverSocket = new ServerSocket(portNo);
                } catch (IOException e) {
                    pool.shutdown();
                    e.printStackTrace();
                }

//                while (true) {
//                    try {
//                        socket = serverSocket.accept();
//                        BufferedReader input = new BufferedReader(new InputStreamReader(socket.getInputStream()));
//                        String data = input.readLine();
//                        addEntry(data, sensorType, chart);
//                    } catch (IOException e) {
//                        e.printStackTrace();
//                    }
//                }
            }
            class Handler implements Runnable {
                private final Socket socket;
                String sensorType;
                LineChart chart;
                Handler(Socket socket, String sensorType, LineChart chart) {
                    this.socket = socket;
                    this.sensorType = sensorType;
                    this.chart = chart;
                }
                public void run() {
                    // read and service request on socket
                    BufferedReader input = null;
                    try {
                        input = new BufferedReader(new InputStreamReader(socket.getInputStream()));
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                    String data = null;
                    try {
                        data = input.readLine();
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                    addEntry(data, this.sensorType, this.chart);
                }
            }
        }
    }