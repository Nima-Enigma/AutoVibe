package com.autovibe;

import android.content.Intent;
import android.media.MediaPlayer;
import android.os.Bundle;
import android.os.Handler;
import android.view.View;
import android.view.animation.AlphaAnimation;
import android.widget.ImageView;
import android.widget.TextView;

import androidx.appcompat.app.AppCompatActivity;

import com.bumptech.glide.Glide;
import com.bumptech.glide.request.RequestOptions;

public class SplashActivity extends AppCompatActivity {
    private MediaPlayer mediaPlayer;  // Declare MediaPlayer at class level so it's accessible in both onCreate and onPause

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_splash);

        // Initialize views
        TextView appName = findViewById(R.id.app_name);
        ImageView logoImage = findViewById(R.id.logo_image);
        ImageView carGif = findViewById(R.id.car_gif);

        // Initialize and start MediaPlayer
        mediaPlayer = MediaPlayer.create(this, R.raw.splash_sound); // Your sound file
        int startPosition = (int) (1000);  // 50% of the duration
        mediaPlayer.seekTo(startPosition);
        mediaPlayer.start();

        // Preload the GIF to ensure it's cached
        Glide.with(this)
                .asGif()
                .load(R.drawable.splash_gif)
                .preload(); // Preload GIF immediately

        // Load the GIF into ImageView and start the animation
        Glide.with(this)
                .asGif()
                .load(R.drawable.splash_gif)
                .apply(new RequestOptions().placeholder(R.drawable.first_frame)) // Static first frame
                .into(carGif);

        // Apply fade-in animation only to logo and app name (not the car GIF)
        fadeIn(appName);
        fadeIn(logoImage);

        // Set a delay for fade-out and transition
        new Handler().postDelayed(() -> {
            fadeOut(appName);
            fadeOut(logoImage);
            fadeOut(carGif);
            // Transition to MainActivity after fade-out
            new Handler().postDelayed(() -> {
                stopSound();
                startActivity(new Intent(SplashActivity.this, MQTTConnectActivity.class));
                finish();
            }, 500); // Delay to ensure fade-out completes
        }, 4000); // Splash screen duration
    }

    private void fadeIn(View view) {
        AlphaAnimation fadeIn = new AlphaAnimation(0.0f, 1.0f);
        fadeIn.setDuration(2000);
        view.startAnimation(fadeIn);
        view.setVisibility(View.VISIBLE);
    }

    private void fadeOut(View view) {
        AlphaAnimation fadeOut = new AlphaAnimation(1.0f, 0.0f);
        fadeOut.setDuration(500);
        view.startAnimation(fadeOut);
        view.setVisibility(View.INVISIBLE);
    }

    private void stopSound() {
        if (mediaPlayer != null) {
            if (mediaPlayer.isPlaying()) {
                mediaPlayer.stop();
            }
            mediaPlayer.release();
            mediaPlayer = null; // Prevent reuse
        }
    }

    @Override
    protected void onPause() {
        super.onPause();
        if (mediaPlayer != null) {
            if (mediaPlayer.isPlaying()) {
                mediaPlayer.stop();
            }
            mediaPlayer.release();
            mediaPlayer = null; // Prevent reuse
        }
    }

}
