package com.autovibe.screens.carInfo;

public class CarDetails {
    private final String brand;
    private final String engineType;
    private final int maxRpm;
    private final int releaseYear;

    public CarDetails(String brand, String engineType, int maxRpm, int releaseYear) {
        this.brand = brand;
        this.engineType = engineType;
        this.maxRpm = maxRpm;
        this.releaseYear = releaseYear;
    }

    public String getBrand() {
        return brand;
    }

    public String getEngineType() {
        return engineType;
    }

    public int getMaxRpm() {
        return maxRpm;
    }

    public int getReleaseYear() {
        return releaseYear;
    }
}
