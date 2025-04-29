package com.autovibe.ui.home;

import androidx.lifecycle.LiveData;
import androidx.lifecycle.MutableLiveData;
import androidx.lifecycle.ViewModel;

public class HomeViewModel extends ViewModel {

    private final MutableLiveData<Integer> currentRpm = new MutableLiveData<>();

    public HomeViewModel() {
        currentRpm.setValue(1000); // Initial RPM value
    }

    public LiveData<Integer> getCurrentRpm() {
        return currentRpm;
    }

    // Simulate RPM updates in the background
    public void simulateRpmUpdates() {
        new Thread(() -> {
            int rpm = 1000;
            while (true) {
                try {
                    Thread.sleep(2000); // Simulate RPM change every second
                    if (rpm > 8000) rpm = 1000;
                    final int finalRpm = rpm;
                    currentRpm.postValue(finalRpm); // Post updates on the main thread
                    rpm += 500;
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
        }).start();
    }
}
