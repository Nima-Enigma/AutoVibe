package com.autovibe.screens.home;

import androidx.lifecycle.LiveData;
import androidx.lifecycle.MutableLiveData;
import androidx.lifecycle.ViewModel;

import com.autovibe.screens.carInfo.CarDetails;

import java.util.HashMap;
import java.util.Map;

public class HomeViewModel extends ViewModel {
    private final MutableLiveData<String> carSelection = new MutableLiveData<>();
    private final Map<String, CarDetails> carDetailsMap = new HashMap<>();

    public HomeViewModel() {
        carDetailsMap.put("911 GT3 RS", new CarDetails("Porsche", "Flat-6", 9000, 2023));
        carDetailsMap.put("Nissan GTR", new CarDetails("Nissan", "V6 Twin Turbo", 7000, 2021));
        carDetailsMap.put("Ford Mustang 69'", new CarDetails("Ford", "V8", 6000, 1969));
    }

    public CarDetails getSelectedCarDetails() {
        String selected = carSelection.getValue();
        return selected != null ? carDetailsMap.get(selected) : null;
    }

    private final MutableLiveData<Integer> rpm = new MutableLiveData<>(0);
    private final MutableLiveData<Integer> volumeLevel = new MutableLiveData<>(50);

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
