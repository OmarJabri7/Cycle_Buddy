package com.example.cyclebuddy;

import androidx.appcompat.app.AppCompatActivity;
import androidx.core.content.res.TypedArrayUtils;

import android.content.Context;
import android.graphics.Color;
import android.net.ConnectivityManager;
import android.net.DhcpInfo;
import android.net.NetworkInfo;
import android.net.wifi.WifiInfo;
import android.net.wifi.WifiManager;
import android.os.AsyncTask;
import android.os.Bundle;
import android.os.Handler;
import android.os.StrictMode;
import android.provider.SyncStateContract;
import android.text.format.Formatter;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

import com.github.mikephil.charting.charts.LineChart;
import com.github.mikephil.charting.components.YAxis;
import com.github.mikephil.charting.data.Entry;
import com.github.mikephil.charting.data.LineData;
import com.github.mikephil.charting.data.LineDataSet;
import com.github.mikephil.charting.interfaces.datasets.ILineDataSet;
import com.github.mikephil.charting.utils.Utils;

import java.io.BufferedReader;
import java.io.DataOutputStream;
import java.io.FileReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.lang.ref.WeakReference;
import java.math.BigInteger;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.HttpURLConnection;
import java.net.InetAddress;
import java.net.ServerSocket;
import java.net.Socket;
import java.net.URL;
import java.net.UnknownHostException;
import java.nio.channels.FileLock;
import java.sql.Array;
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

        public static final int BROADCASTPORT = 8888;

//        public static TextView ip;
        public static Button connectBtn;

        @Override
        public void onCreate(Bundle savedInstanceState) {

            super.onCreate(savedInstanceState);
            setContentView(R.layout.activity_main);
            connectBtn = (Button) findViewById(R.id.connectBtn);
            distance_chart = (LineChart) findViewById(R.id.distance_chart);
//            ip = (TextView) findViewById(R.id.ip);
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
            WifiManager wifiManager = (WifiManager) getApplicationContext().getSystemService(WIFI_SERVICE);
            WifiInfo wifiinfo = wifiManager.getConnectionInfo();
            byte[] myIPAddress = BigInteger.valueOf(wifiinfo.getIpAddress()).toByteArray();
            byte[] IPAddrRev = reverse(myIPAddress);
            InetAddress myInetIP = null;
            try {
                myInetIP = InetAddress.getByAddress(IPAddrRev);
            } catch (UnknownHostException e) {
                e.printStackTrace();
            }
            String myIP = myInetIP.getHostAddress();

//            ip.setText("Host IP: " + myIP);
            connectBtn.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View v) {
                    sendBroadcast(myIP);
                }
            });
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
            sendBroadcast(myIP);
//            NetworkSniffTask sniff = new NetworkSniffTask(this);
//            sniff.execute();
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

    InetAddress getBroadcastAddress() throws IOException {
        WifiManager wifi = (WifiManager) getApplicationContext().getSystemService(Context.WIFI_SERVICE);
        DhcpInfo dhcp = wifi.getDhcpInfo();
        // handle null somehow

        int broadcast = (dhcp.ipAddress & dhcp.netmask) | ~dhcp.netmask;
        byte[] quads = new byte[4];
        for (int k = 0; k < 4; k++)
            quads[k] = (byte) ((broadcast >> k * 8) & 0xFF);
        return InetAddress.getByAddress(quads);
    }

    public void sendBroadcast(String messageStr) {
        // Hack Prevent crash (sending should be done using an async task)
        StrictMode.ThreadPolicy policy = new   StrictMode.ThreadPolicy.Builder().permitAll().build();
        StrictMode.setThreadPolicy(policy);
        try {
            //Open a random port to send the package
            DatagramSocket socket = new DatagramSocket();
            socket.setBroadcast(true);
            byte[] sendData = messageStr.getBytes();
            DatagramPacket sendPacket = new DatagramPacket(sendData, sendData.length, getBroadcastAddress(), BROADCASTPORT);
            socket.send(sendPacket);
            System.out.println(getClass().getName() + "Broadcast packet sent to: " + getBroadcastAddress().getHostAddress());
        } catch (IOException e) {
            Log.e("broadcastError", "IOException: " + e.getMessage());
        }
    }

    static class NetworkSniffTask extends AsyncTask<Void, Void, Void> {

        private static final String TAG = "nstask";

        private WeakReference<Context> mContextRef;

        public NetworkSniffTask(Context context) {
            mContextRef = new WeakReference<Context>(context);
        }

        @Override
        protected Void doInBackground(Void... voids) {
            Log.d(TAG, "Let's sniff the network");

            try {
                Context context = mContextRef.get();

                if (context != null) {

                    ConnectivityManager cm = (ConnectivityManager) context.getSystemService(Context.CONNECTIVITY_SERVICE);
                    NetworkInfo activeNetwork = cm.getActiveNetworkInfo();
                    WifiManager wm = (WifiManager) context.getApplicationContext().getSystemService(Context.WIFI_SERVICE);

                    WifiInfo connectionInfo = wm.getConnectionInfo();
                    int ipAddress = connectionInfo.getIpAddress();
                    String ipString = Formatter.formatIpAddress(ipAddress);


                    Log.d(TAG, "activeNetwork: " + String.valueOf(activeNetwork));
                    Log.d(TAG, "ipString: " + String.valueOf(ipString));

                    String prefix = ipString.substring(0, ipString.lastIndexOf(".") + 1);
                    Log.d(TAG, "prefix: " + prefix);

                    for (int i = 0; i < 255; i++) {
                        String testIp = prefix + String.valueOf(i);
                        InetAddress address = InetAddress.getByName(testIp);
                        boolean reachable = address.isReachable(1000);
                        String deviceName = address.getHostName();
                        String hostName = address.getCanonicalHostName();
//                        ip.setText(hostName);
                        if (reachable && hostName.equals("raspberrypi"))
                            Log.i(TAG, "Host: " + String.valueOf(hostName) + "(" + String.valueOf(testIp) + ") is reachable!");
//                            ip.setText(hostName);
                    }
                }
            } catch (UnknownHostException e) {
                e.printStackTrace();
            } catch (IOException ioException) {
                ioException.printStackTrace();
            }
            return null;
        }
    }

    public static byte[] reverse(byte[] array) {
        if (array == null) {
            return null;
        }
        int i = 0;
        int j = array.length - 1;
        byte tmp;
        while (j > i) {
            tmp = array[j];
            array[j] = array[i];
            array[i] = tmp;
            j--;
            i++;
        }
        return array;
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