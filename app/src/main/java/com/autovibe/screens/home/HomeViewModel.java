package com.autovibe.screens.home;

import androidx.lifecycle.LiveData;
import androidx.lifecycle.MutableLiveData;
import androidx.lifecycle.ViewModel;

public class HomeViewModel extends ViewModel {

    private final MutableLiveData<Integer> rpm = new MutableLiveData<>(0);
    private final MutableLiveData<Integer> volumeLevel = new MutableLiveData<>(50);
    private final MutableLiveData<String> carSelection = new MutableLiveData<>("Tesla Model S");

    public LiveData<Integer> getRpm() {
        return rpm;
    }

    public void setRpm(int value) {
        rpm.setValue(value);
    }

    public LiveData<Integer> getVolumeLevel() {
        return volumeLevel;
    }

    public void setVolumeLevel(int value) {
        volumeLevel.setValue(value);
    }

    public LiveData<String> getCarSelection() {
        return carSelection;
    }

    public void setCarSelection(String value) {
        carSelection.setValue(value);
    }
}
