package com.example.cyclebuddy;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

import com.google.android.gms.tasks.OnCompleteListener;
import com.google.android.gms.tasks.Task;
import com.google.firebase.auth.AuthResult;
import com.google.firebase.auth.FirebaseAuth;
import com.google.firebase.auth.FirebaseUser;
import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;

import java.util.HashMap;
import java.util.Map;

public class UserActivity extends AppCompatActivity {

    public class User {

        public String email;
        public String passwd;

        public User() {
            // Default constructor required for calls to DataSnapshot.getValue(User.class)
        }

        public User(String email, String passwd) {
            this.email = email;
            this.passwd = passwd;
        }

    }


    Button logIn;
    Button signUp;
    EditText email;
    EditText passwd;
    private FirebaseAuth mAuth;
    public static DatabaseReference mDatabase;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_user);
        logIn = (Button) findViewById(R.id.logIn);
        signUp = (Button) findViewById(R.id.signUp);
        email = (EditText) findViewById(R.id.email);
        passwd = (EditText) findViewById(R.id.psswd);
        mAuth = FirebaseAuth.getInstance();
        mDatabase = FirebaseDatabase.getInstance().getReference();
        logIn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                User user = new User(String.valueOf(email.getText()), String.valueOf(passwd.getText()));
                mAuth.signInWithEmailAndPassword(user.email, user.passwd)
                        .addOnCompleteListener(UserActivity.this, new OnCompleteListener<AuthResult>() {
                            @Override
                            public void onComplete(@NonNull Task<AuthResult> task) {
                                if (task.isSuccessful()) {
                                    // Sign in success, update UI with the signed-in user's information
                                    Log.d("0", "signInWithEmail:success");
                                    Toast.makeText(UserActivity.this, "Success!",
                                            Toast.LENGTH_SHORT).show();
                                    FirebaseUser user = mAuth.getCurrentUser();
                                    Intent myIntent = new Intent(UserActivity.this, MainActivity.class);
//                                    myIntent.putExtra("user", user); //Optional parameters
                                    UserActivity.this.startActivity(myIntent);
                                } else {
                                    // If sign in fails, display a message to the user.
                                    Log.w("1", "signInWithEmail:failure", task.getException());
                                    Toast.makeText(UserActivity.this, "Authentication failed.",
                                            Toast.LENGTH_SHORT).show();
                                }
                            }
                        });

            }
        });
        signUp.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                User user = new User(String.valueOf(email.getText()), String.valueOf(passwd.getText()));
                if(user.email.isEmpty() || user.passwd.isEmpty()){
                    Toast.makeText(UserActivity.this, "Please enter information",Toast.LENGTH_SHORT).show();
                }
                else if(isEmailValid(user.email) && isPasswdValid(user.passwd)) {

                    mAuth.createUserWithEmailAndPassword(user.email, user.passwd).addOnCompleteListener(UserActivity.this, new OnCompleteListener<AuthResult>() {
                        @Override
                        public void onComplete(@NonNull Task<AuthResult> task) {
                            if (task.isSuccessful()) {
                                // Sign in success, update UI with the signed-in user's information
                                Log.d("0", "createUserWithEmail:success");
                                FirebaseUser user = mAuth.getCurrentUser();
                                Toast.makeText(UserActivity.this, "Success!",
                                        Toast.LENGTH_SHORT).show();
                                Intent myIntent = new Intent(UserActivity.this, MainActivity.class);
//                                myIntent.putExtra("user", user); //Optional parameters
                                UserActivity.this.startActivity(myIntent);
                            } else {
                                // If sign in fails, display a message to the user.
                                Log.w("1", "createUserWithEmail:failure", task.getException());
                                Toast.makeText(UserActivity.this, "Authentication failed.",
                                        Toast.LENGTH_SHORT).show();
                            }
                        }
                    });
                }
                else{
                    Toast.makeText(UserActivity.this, "Please enter a valid email/password",Toast.LENGTH_SHORT).show();
                }
//                Map<String, String> map = new HashMap<>();
//                if(isEmailValid(user.email)) {
//                    map.put("Email", user.email);
//                    map.put("Password", user.passwd);
//                    mDatabase.child("Users").push().setValue(map);
//                }
//                else{
//                    Toast.makeText(getApplicationContext(),"Email is not valid",Toast.LENGTH_SHORT).show();
//                }
            }
        });
    }
    boolean isEmailValid(CharSequence email) {
        return android.util.Patterns.EMAIL_ADDRESS.matcher(email).matches();
    }
    boolean isPasswdValid(String passwd){
        return passwd.length() > 8;
    }
}