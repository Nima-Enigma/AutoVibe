package com.autovibe.screens.home;

import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.SeekBar;
import android.widget.Spinner;
import android.widget.TextView;

import androidx.annotation.NonNull;
import androidx.fragment.app.Fragment;
import androidx.lifecycle.ViewModelProvider;

import com.autovibe.ConnectionHolder;
import com.autovibe.MqttHandler;
import com.autovibe.R;
import com.autovibe.databinding.FragmentHomeBinding;

import org.json.JSONException;
import org.json.JSONObject;

public class HomeFragment extends Fragment {

    private FragmentHomeBinding binding;
    private TextView currentRpmTextView, volumeTextView;
    private SeekBar volumeSeekBar;
    private Spinner carSelectionSpinner;

    private final String[] carList = {"Tesla Model S", "BMW M3", "Nissan GTR", "Ford Mustang", "Lamborghini Huracan"};
    private HomeViewModel homeViewModel;
    private MqttHandler mqttHandler;

    @Override
    public View onCreateView(@NonNull LayoutInflater inflater,
                             ViewGroup container, Bundle savedInstanceState) {
        homeViewModel = new ViewModelProvider(requireActivity()).get(HomeViewModel.class);
        mqttHandler = ConnectionHolder.getHandler();

        binding = FragmentHomeBinding.inflate(inflater, container, false);
        View root = binding.getRoot();

        // Bind UI
        currentRpmTextView = binding.textCurrentRpm;
        volumeTextView = binding.textVolumeIntensity;
        volumeSeekBar = binding.seekbarVolumeIntensity;
        carSelectionSpinner = binding.spinnerCarSelection;

        // Setup Spinner
        ArrayAdapter<String> adapter = new ArrayAdapter<>(requireContext(), R.layout.spinner_item, carList);
        adapter.setDropDownViewResource(R.layout.spinner_item);
        carSelectionSpinner.setAdapter(adapter);

        // Observe LiveData
        homeViewModel.getRpm().observe(getViewLifecycleOwner(), rpm ->
                currentRpmTextView.setText(rpm + " RPM")
        );

        homeViewModel.getVolumeLevel().observe(getViewLifecycleOwner(), volume -> {
            volumeTextView.setText(volume + "%");
            publishCarStatus();
        });

        homeViewModel.getCarSelection().observe(getViewLifecycleOwner(), car -> {
            publishCarStatus();
        });

        // Volume SeekBar listener
        volumeSeekBar.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
                if (fromUser) homeViewModel.setVolumeLevel(progress);
            }
            @Override public void onStartTrackingTouch(SeekBar seekBar) {}
            @Override public void onStopTrackingTouch(SeekBar seekBar) {}
        });

        // Spinner selection listener
        carSelectionSpinner.setOnItemSelectedListener(new android.widget.AdapterView.OnItemSelectedListener() {
            @Override public void onItemSelected(android.widget.AdapterView<?> parent, View view, int position, long id) {
                homeViewModel.setCarSelection(carList[position]);
            }

            @Override public void onNothingSelected(android.widget.AdapterView<?> parent) {}
        });

        subscribeToRpm();

        return root;
    }

    private void subscribeToRpm() {
        if (mqttHandler != null) {
            mqttHandler.setMessageCallback((topic, message) -> {
                try {
                    String payload = new String(message.getPayload());
                    JSONObject json = new JSONObject(payload);
                    int rpm = json.getInt("rpm");
                    requireActivity().runOnUiThread(() -> homeViewModel.setRpm(rpm));
                } catch (JSONException e) {
                    e.printStackTrace();
                }
            });
            mqttHandler.subscribe("car/rpm");
        }
    }

    private void publishCarStatus() {
        if (mqttHandler == null) return;

        Integer volume = homeViewModel.getVolumeLevel().getValue();
        String car = homeViewModel.getCarSelection().getValue();

        if (volume == null || car == null) return;

        try {
            JSONObject json = new JSONObject();
            json.put("volume_Level", volume);
            json.put("chosen_car", car);

            mqttHandler.publish("car/control", json.toString());
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    @Override
    public void onDestroyView() {
        super.onDestroyView();
        binding = null;
    }
}
