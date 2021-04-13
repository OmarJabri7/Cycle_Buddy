package com.example.cyclebuddy;

import androidx.annotation.NonNull;
import androidx.annotation.RequiresApi;
import androidx.appcompat.app.AppCompatActivity;

import android.content.Context;
import android.content.SharedPreferences;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.drawable.Drawable;
import android.media.Image;
import android.net.DhcpInfo;
import android.net.Uri;
import android.net.wifi.WifiInfo;
import android.net.wifi.WifiManager;
import android.os.Build;
import android.os.Bundle;
import android.os.Handler;
import android.os.StrictMode;
import android.text.format.DateFormat;
import android.text.method.LinkMovementMethod;
import android.util.Base64;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;

import com.bumptech.glide.Glide;
import com.github.mikephil.charting.charts.Chart;
import com.github.mikephil.charting.charts.LineChart;
import com.github.mikephil.charting.components.AxisBase;
import com.github.mikephil.charting.components.XAxis;
import com.github.mikephil.charting.components.YAxis;
import com.github.mikephil.charting.data.Entry;
import com.github.mikephil.charting.data.LineData;
import com.github.mikephil.charting.data.LineDataSet;
import com.github.mikephil.charting.formatter.IAxisValueFormatter;
import com.github.mikephil.charting.interfaces.datasets.ILineDataSet;
import com.google.android.gms.tasks.OnFailureListener;
import com.google.android.gms.tasks.OnSuccessListener;
import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;
import com.google.firebase.storage.FirebaseStorage;
import com.google.firebase.storage.StorageReference;

import org.json.JSONException;
import org.json.JSONObject;
import org.w3c.dom.Text;

import java.io.BufferedReader;
import java.io.ByteArrayInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.math.BigInteger;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.ServerSocket;
import java.net.Socket;
import java.net.URL;
import java.net.UnknownHostException;
import java.nio.charset.StandardCharsets;
import java.text.SimpleDateFormat;
import java.util.Calendar;
import java.util.Date;
import java.util.HashMap;
import java.util.Locale;
import java.util.Map;
import java.util.Random;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

    public class MainActivity<StorageReference> extends AppCompatActivity {

        public class MyXAxisValueFormatter implements IAxisValueFormatter {

            @Override
            public String getFormattedValue(float value, AxisBase axis) {
                axis.setLabelCount(2,true);
                SimpleDateFormat sdf = new SimpleDateFormat("d");
                return sdf.format(new Date(Long.parseLong(String.valueOf(value))));
            }
        }

        public class Car {

            public String carPlate;
            public double car_distance;
            public double car_velocity;
            public double bike_velocity;

            public Car() {
                // Default constructor required for calls to DataSnapshot.getValue(User.class)
            }

            public Car(String car_plate, double cd, double cv, double bv) {
                this.carPlate = car_plate;
                this.car_distance = cd;
                this.car_velocity = cv;
                this.bike_velocity = bv;
            }

        }


        private ServerSocket serverSocket;

        Handler updateConversationHandler;

        SharedPreferences prefs;

        ServerThread serverThreadSonarOne = null;

        ServerThread serverThreadHall = null;

        com.google.firebase.storage.StorageReference storageRef;

        ServerThread serverThreadSonarTwo = null;

        ResultsThread resultsThread = null;

//        ImageThread imgThread = null;

        private LineChart distance_sonar_chart;

        private LineChart vel_chart;

        private LineChart vel_sonar_chart;

        TextView carPlate;
        TextView carDistance;
        TextView carVelocity;
        TextView bikeVelocity;
        ImageView plateImg;

        public static final int SERVERPORT_SONAR_DIST = 8080;

        public static final int FILESIZE = 3686416;

        public static final int SERVERPORT_HALL = 6000;

        public static final int RESULTS_PORT = 4070;

        public static final int BROADCASTPORT = 8888;

        FirebaseStorage fb_storage;

        public static final int IMG_PORT = 6969;

        public static DatabaseReference mDatabase;
        FirebaseStorage storage;

        public static Button connectBtn;

        TextView imgLink;

        @Override
        protected void onPause() {
            super.onPause();
        }

        @Override
        protected void onResume() {
            super.onResume();
            prefs = getSharedPreferences("data",0);
            if(prefs.contains("car_plate")){
                imgLink.setText(prefs.getString("img_link",""));
                carPlate.setText("Car Plate: " + prefs.getString("car_plate",""));
                carDistance.setText("Car Distance: " + prefs.getFloat("car_distance",0));
                carVelocity.setText("Car Velocity " + prefs.getFloat("car_velocity",0));
                bikeVelocity.setText("Bike Velocity: " + prefs.getFloat("bike_velocity",0));
            }
        }

        @Override
        public void onCreate(Bundle savedInstanceState) {

            super.onCreate(savedInstanceState);
            setContentView(R.layout.activity_main);
            String myUrl = "https://github.com/bumptech/glide/raw/master/static/glide_logo.png";
            connectBtn = (Button) findViewById(R.id.connectBtn);
            // Create a storage reference from our app
            storage = FirebaseStorage.getInstance("gs://cyclebuddy-17c84.appspot.com");
            storageRef = storage.getReference();
            carPlate = (TextView) findViewById(R.id.carPlate);
            carDistance = (TextView) findViewById(R.id.carDistance);
//            imgLink = (TextView) findViewById(R.id.img_link);
            prefs = getSharedPreferences("data",0);
            if(prefs.contains("car_plate")){
                imgLink.setText(prefs.getString("img_link",""));
                carPlate.setText("Car Plate: " + prefs.getString("car_plate",""));
                carDistance.setText("Car Distance: " + prefs.getFloat("car_distance",0));
                carVelocity.setText("Car Velocity " + prefs.getFloat("car_velocity",0));
                bikeVelocity.setText("Bike Velocity: " + prefs.getFloat("bike_velocity",0));
            }
            //TextView link = (TextView) findViewById(R.id.link);
            //link.setMovementMethod(LinkMovementMethod.getInstance());
            carVelocity = (TextView) findViewById(R.id.carVelocity);
            bikeVelocity = (TextView) findViewById(R.id.bikeVelocity);
            plateImg = (ImageView) findViewById(R.id.plateImg);
//            Glide.with(this).load(myUrl).placeholder(R.mipmap.ic_launcher).into(plateImg);
            //Sonar 1s
            distance_sonar_chart = (LineChart) findViewById(R.id.distance_chart);
//            distance_sonar_chart.getDescription().setEnabled(true);
            distance_sonar_chart.setTouchEnabled(false);
            distance_sonar_chart.setDragEnabled(false);
            distance_sonar_chart.setScaleEnabled(false);
            YAxis leftYAxisSonarOne = distance_sonar_chart.getAxisLeft();
            leftYAxisSonarOne.setEnabled(false);
            distance_sonar_chart.setDrawGridBackground(false);
            distance_sonar_chart.setPinchZoom(false);
            distance_sonar_chart.setBackgroundColor(Color.WHITE);
            LineData distance_data = new LineData();
            distance_data.setValueTextColor(Color.WHITE);
            distance_sonar_chart.setData(distance_data);
            //Send broadcast message
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
            connectBtn.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View v) {
                    sendBroadcast(myIP);
                }
            });

            //Hall
            vel_chart = (LineChart) findViewById(R.id.vel_chart);
//            vel_chart.getDescription().setEnabled(true);
            vel_chart.setTouchEnabled(false);
            vel_chart.setDragEnabled(false);
            vel_chart.setScaleEnabled(false);
            YAxis leftYAxisHall = vel_chart.getAxisLeft();
            leftYAxisHall.setEnabled(false);
            vel_chart.setDrawGridBackground(false);
            vel_chart.setPinchZoom(false);
            vel_chart.setBackgroundColor(Color.WHITE);
            LineData vel_data = new LineData();
            distance_data.setValueTextColor(Color.WHITE);
            vel_chart.setData(vel_data);

            //Sonar 2
            vel_sonar_chart = (LineChart) findViewById(R.id.vel_chart2);
//            vel_sonar_chart.getDescription().setEnabled(true);
            vel_sonar_chart.setTouchEnabled(false);
            vel_sonar_chart.setDragEnabled(false);
            vel_sonar_chart.setScaleEnabled(false);
            YAxis leftYAxisSonarTwo = vel_sonar_chart.getAxisLeft();
            leftYAxisSonarTwo.setEnabled(false);
            vel_sonar_chart.setDrawGridBackground(false);
            vel_sonar_chart.setPinchZoom(false);
            vel_sonar_chart.setBackgroundColor(Color.WHITE);
            LineData vel_sonar_data = new LineData();
            vel_sonar_data.setValueTextColor(Color.WHITE);
            vel_sonar_chart.setData(vel_sonar_data);
            //Start threads
            try {
                this.serverThreadSonarOne = new ServerThread(SERVERPORT_SONAR_DIST,
                        "SONAR", 2);
            } catch (IOException ioException) {
                ioException.printStackTrace();
            }
            try {
                this.serverThreadHall = new ServerThread(SERVERPORT_HALL,
                        "HALL", 2);
            } catch (IOException ioException) {
                ioException.printStackTrace();
            }
            try {
                this.resultsThread = new ResultsThread(RESULTS_PORT,2);
            } catch (IOException ioException) {
                ioException.printStackTrace();
            }
//            try {
//                this.imgThread = new ImageThread(IMG_PORT, 2);
//            } catch (IOException ioException) {
//                ioException.printStackTrace();
//            }
            mDatabase = FirebaseDatabase.getInstance().getReference();

            new Thread(this.serverThreadSonarOne).start();
            new Thread(this.serverThreadHall).start();
            new Thread(this.serverThreadSonarTwo).start();
            new Thread(this.resultsThread).start();
            //new Thread(this.imgThread).start();
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
            StrictMode.ThreadPolicy policy = new StrictMode.ThreadPolicy.Builder().permitAll().build();
            StrictMode.setThreadPolicy(policy);
            try {
                //Open a random port to send the package
                DatagramSocket socket = new DatagramSocket();
                socket.setBroadcast(true);
                byte[] sendData = messageStr.getBytes();
                DatagramPacket sendPacket = new DatagramPacket(sendData, sendData.length, getBroadcastAddress(), BROADCASTPORT);
                socket.send(sendPacket);
                System.out.println(getClass().getName() + "Broadcast packet sent to: " + getBroadcastAddress().getHostAddress());
//                connectionStatus.setText("Connected");
            } catch (IOException e) {
//                connectionStatus.setText("Not connected");
                Log.e("broadcastError", "IOException: " + e.getMessage());
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

        public String formatDate(long time) {
//            SimpleDateFormat sdf = new SimpleDateFormat("HH:MM:SS");
            Calendar cal = Calendar.getInstance(Locale.ENGLISH);
            cal.setTimeInMillis(time * 1000);
            String date = DateFormat.format("HH:MM:SS", cal).toString();
            return date;
//            return String.valueOf(sdf.format(new Date(Long.parseLong(String.valueOf(value)))));
        }

        public void writeNewCar(String cp,double cd, double cv, double bv) {
            Car car = new Car(cp, cd,cv, bv);
            final int min = 0;
            final int max = 1000;
            final int random = new Random().nextInt((max - min) + 1) + min;
            Map<String, String> map = new HashMap<>();
            map.put("Car Plate",car.carPlate);
            map.put("Car Distance",String.valueOf(car.car_distance));
            map.put("Car Velocity",String.valueOf(car.car_velocity));
            map.put("Bike Velocity",String.valueOf(car.bike_velocity));
            mDatabase.child("Cars").push().setValue(map);
        }

        private void addEntry(String readings, String vel,String timestamp, String sensorType) {
            if(!readings.equals("-1.0")){
            LineChart chart1 = distance_sonar_chart;
            LineChart chart2 = vel_sonar_chart;
            LineData data = chart1.getData();
            if (data != null) {
                ILineDataSet set = data.getDataSetByIndex(0);
                if (set == null) {
                    set = createSet(sensorType);
                    data.addDataSet(set);
                }
                Entry entry = new Entry(set.getEntryCount(), Float.parseFloat(readings));
                data.addEntry(entry, 0);
                data.notifyDataChanged();
                chart1.notifyDataSetChanged();
                chart1.setMaxVisibleValueCount(150);
                chart1.getDescription().setText(timestamp);
                Paint p = chart1.getPaint(Chart.PAINT_DESCRIPTION);
                p.setColor(1);
                chart1.getDescription().setTextSize(16f);
//                chart1.getDescription().setPosition(0f,1f);
                chart1.setData(data);
                chart1.moveViewToX((set.getEntryCount()));
                chart1.setVisibleXRangeMaximum(10);
                YAxis right = chart1.getAxisRight();
                right.setAxisMinimum((Float.parseFloat(readings)) + 100);
                right.setAxisMinimum(0);
            }
                LineData data2 = chart2.getData();
                if (data2 != null) {
                    ILineDataSet set = data2.getDataSetByIndex(0);
                    if (set == null) {
                        set = createSet("");
                        data2.addDataSet(set);
                    }
                    Entry entry = new Entry(set.getEntryCount(), Float.parseFloat(vel));
                    data2.addEntry(entry, 0);
                    data2.notifyDataChanged();
                    chart2.notifyDataSetChanged();
                    chart2.setMaxVisibleValueCount(150);
                    chart2.getDescription().setText(timestamp);
                    chart2.getDescription().setTextSize(16f);
//                    chart2.getDescription().setPosition(0f,1f);
                    chart2.setData(data2);
                    chart2.moveViewToX((set.getEntryCount()));
                    chart2.setVisibleXRangeMaximum(10);
                    YAxis right = chart2.getAxisRight();
                    right.setAxisMinimum((Float.parseFloat(vel)) + 20);
                    right.setAxisMinimum(0);
                }
            }
            else{
                LineChart chart3 = vel_chart;
                LineData data = chart3.getData();
                if (data != null) {
                    ILineDataSet set = data.getDataSetByIndex(0);
                    if (set == null) {
                        set = createSet(sensorType);
                        data.addDataSet(set);
                    }
                    Entry entry = new Entry(set.getEntryCount(), Float.parseFloat(vel));
                    data.addEntry(entry, 0);
                    data.notifyDataChanged();
                    chart3.notifyDataSetChanged();
                    chart3.setMaxVisibleValueCount(150);
                    chart3.getDescription().setText(timestamp);
                    chart3.getDescription().setTextSize(16f);
//                    chart3.getDescription().setPosition(0f,1f);
                    chart3.setData(data);
                    chart3.moveViewToX((set.getEntryCount()));
                    chart3.setVisibleXRangeMaximum(10);
                    YAxis right = chart3.getAxisRight();
                    right.setAxisMinimum((Float.parseFloat(readings)) + 20);
                    right.setAxisMinimum(0);
                }
            }
        }

        private LineDataSet createSet(String sensorType) {
            LineDataSet set = null;
            if (sensorType.equals("SONAR")) {
                set = new LineDataSet(null, "Car Distance Data");
                set.setColor(Color.RED);
            } else if(sensorType.equals("HALL")) {
                set = new LineDataSet(null, "Bike Velocity Data");
                set.setColor(Color.GREEN);
            }
            else {
                set = new LineDataSet(null, "Car Velocity Data");
                set.setColor(Color.MAGENTA);
            }
            set.setAxisDependency(YAxis.AxisDependency.RIGHT);
            set.setLineWidth(3);
            set.setMode(LineDataSet.Mode.CUBIC_BEZIER);
            set.setCubicIntensity(0.2f);
            return set;
        }


        class ServerThread implements Runnable {
            String sensorType;
            ServerSocket serverSocket;
            private final ExecutorService pool;

            public ServerThread(int port, String sensorType, int poolSize) throws IOException {
                this.sensorType = sensorType;
                this.serverSocket = new ServerSocket(port);
                pool = Executors.newFixedThreadPool(poolSize);
            }

            public void run() {
                Socket socket = null;
                try {
                    for (; ; ) {
                        pool.execute(new Handler(serverSocket.accept(), sensorType));
                    }
                } catch (IOException e) {
                    pool.shutdown();
                    e.printStackTrace();
                }
            }

            class Handler implements Runnable {
                private final Socket socket;
                String sensorType;
                LineChart chart;

                Handler(Socket socket, String sensorType) {
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
                    double sensor_distance = 0;
                    double sensor_velocity = 0;
                    String timestamp = "";
                    try {
                        data = input.readLine();
                        JSONObject json_data = new JSONObject(data);
                        sensor_distance = (double) json_data.get("distance");
                        sensor_velocity = (double) json_data.get("velocity");
                        timestamp = (String) json_data.get("timestamp");
                    } catch (IOException | JSONException e) {
                        e.printStackTrace();
                    }
                    addEntry(String.valueOf(sensor_distance),String.valueOf(sensor_velocity), timestamp, this.sensorType);
                }
            }
        }
        class ResultsThread implements Runnable {
            ServerSocket serverSocket;
            private final ExecutorService pool;

            public ResultsThread(int port, int poolSize) throws IOException {
                this.serverSocket = new ServerSocket(port);
                pool = Executors.newFixedThreadPool(poolSize);
            }

            public void run() {
                Socket socket = null;
                try {
                    for (; ; ) {
                        pool.execute(new Handler(serverSocket.accept()));
                    }
                } catch (IOException e) {
                    pool.shutdown();
                    e.printStackTrace();
                }
            }

            class Handler implements Runnable {
                private final Socket socket;

                Handler(Socket socket) {
                    this.socket = socket;
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
                    double car_distance = 0;
                    double car_velocity = 0;
                    double bike_velocity = 0;
                    String car_plate = "";
                    try {
                        data = input.readLine();
                        JSONObject json_data = new JSONObject(data);
                        car_distance = (double) json_data.get("car_distance");
                        car_velocity = (double) json_data.get("car_velocity");
                        bike_velocity = (double) json_data.get("bike_velocity");
                        car_plate = (String) json_data.get("car_plate");
                    } catch (IOException | JSONException e) {
                        e.printStackTrace();
                    }
                    carPlate.setText("Car Plate: " + String.valueOf(car_plate));
                    carDistance.setText("Car Distance: " + String.valueOf((float) car_distance) + " cm");
                    carVelocity.setText("Car Velocity: " + String.valueOf((float)car_velocity) + " cm/s");
                    bikeVelocity.setText("Bike Velocity: " + String.valueOf((float)bike_velocity) + " cm/s");
                    SharedPreferences.Editor e = prefs.edit();
                    e.putString("car_plate",String.valueOf(car_plate));
                    e.putFloat("car_distance",(float)car_distance);
                    e.putFloat("car_velocity",(float)car_velocity);
                    e.putFloat("bike_velocity",(float)bike_velocity);
                    com.google.firebase.storage.StorageReference imagesRef = storageRef.child("images/img.jpeg");
                    // Create a storage reference from our app
                    com.google.firebase.storage.StorageReference storageRef = storage.getReference();

                    // Or Create a reference to a file from a Google Cloud Storage URI
                    com.google.firebase.storage.StorageReference gsReference = storage.getReferenceFromUrl("gs://cyclebuddy-17c84.appspot.com/images/img.jpeg");
                    // Reference to an image file in Cloud Storage
                // ImageView in your Activity
                // Download directly from StorageReference using Glide
                // (See MyAppGlideModule for Loader registration)
//                    Glide.with(getApplicationContext()).load(imagesRef).into(plateImg);
//                    writeNewCar(car_plate,car_distance,car_velocity,bike_velocity);

                    gsReference.getDownloadUrl().addOnSuccessListener(new OnSuccessListener<Uri>() {
                        @Override
                        public void onSuccess(Uri uri) {
                            // Got the download URL for 'users/me/profile.png'
//                            imgLink.setText(String.valueOf(uri));
//                            e.putString("img_link",String.valueOf(uri));
                            Glide.with(MainActivity.this).load(String.valueOf(uri)).placeholder(R.mipmap.ic_launcher).into(plateImg);
                            //Glide.with(getApplicationContext()).load(String.valueOf(uri)).into(plateImg);
                            //plateImg.setImageURI(uri);
                           //Toast.makeText(getApplicationContext(), String.valueOf(uri), Toast.LENGTH_LONG).show();
                        }
                    }).addOnFailureListener(new OnFailureListener() {
                        @Override
                        public void onFailure(@NonNull Exception exception) {
                            // Handle any errors
                        }
                    });
                }
            }
        }

        public static Drawable LoadImageFromWebOperations(String url) {
            try {
                InputStream is = (InputStream) new URL(url).getContent();
                Drawable d = Drawable.createFromStream(is, "src name");
                return d;
            } catch (Exception e) {
                return null;
            }
        }

//        public Bitmap StringToBitMap(String encodedString) {
//            try {
//                byte[] encodeByte = Base64.decode(encodedString, Base64.DEFAULT);
//                Bitmap bitmap = BitmapFactory.decodeByteArray(encodeByte, 0, encodeByte.length);
//                return bitmap;
//            } catch (Exception e) {
//                e.getMessage();
//                return null;
//            }
//        }
//
//        class ImageThread implements Runnable {
//            ServerSocket serverSocket;
//            private final ExecutorService pool;
//
//            public ImageThread(int port, int poolSize) throws IOException {
//                this.serverSocket = new ServerSocket(port);
//                pool = Executors.newFixedThreadPool(poolSize);
//            }
//
//            public void run() {
//                Socket socket = null;
//                try {
//                    for (; ; ) {
//                        pool.execute(new Handler(serverSocket.accept()));
//                    }
//                } catch (IOException e) {
//                    pool.shutdown();
//                    e.printStackTrace();
//                }
//            }
//
//            class Handler implements Runnable {
//                private final Socket socket;
//
//                Handler(Socket socket) {
//                    this.socket = socket;
//                }
//
//                @RequiresApi(api = Build.VERSION_CODES.KITKAT)
//                public void run() {
//                    // read and service request on socket
//                    BufferedReader input = null;
//                    try {
//                        input = new BufferedReader(new InputStreamReader(socket.getInputStream()));
//                    } catch (IOException e) {
//                        e.printStackTrace();
//                    }
//                    try {
//                        InputStream stream = socket.getInputStream();
//                        byte[] data = new byte[FILESIZE];
//                        int count = stream.read(data);
//                        //Bitmap bmp = BitmapFactory.decodeByteArray(data, 0, FILESIZE);
//                        //Drawable d = Drawable.createFromStream(new ByteArrayInputStream(data), null);
//                        //plateImg.setImageBitmap(bmp);
//                        try {
//                            runOnUiThread(new Runnable() {
//                                @Override
//                                public void run() {
//                                    Glide.with(MainActivity.this)
//                                            .load(data)
//                                            .into(plateImg);
//                                }
//                            });
//                        } catch (Exception e) {
//                            e.printStackTrace();
//                            Exception check = e;
//                        }
//                    } catch (IOException ioException) {
//                        ioException.printStackTrace();
//                    }
//                }
//            }
//        }
    }