package com.autovibe.ui.home;

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
import androidx.lifecycle.Observer;
import androidx.lifecycle.ViewModelProvider;

import com.autovibe.R;
import com.autovibe.databinding.FragmentHomeBinding;

public class HomeFragment extends Fragment {

    private FragmentHomeBinding binding;
    private TextView currentRpmTextView, volumeTextView;
    private SeekBar volumeSeekBar;
    private Spinner carSelectionSpinner;

    private String[] carList = {"Tesla Model S", "BMW M3", "Nissan GTR", "Ford Mustang", "Lamborghini Huracan"};
    private HomeViewModel homeViewModel;

    public View onCreateView(@NonNull LayoutInflater inflater,
                             ViewGroup container, Bundle savedInstanceState) {
        homeViewModel = new ViewModelProvider(this).get(HomeViewModel.class);

        binding = FragmentHomeBinding.inflate(inflater, container, false);
        View root = binding.getRoot();

        // Initialize UI elements
        currentRpmTextView = binding.textCurrentRpm;
        volumeTextView = binding.textVolumeIntensity;
        volumeSeekBar = binding.seekbarVolumeIntensity;
        carSelectionSpinner = binding.spinnerCarSelection;

        // Set up Spinner (Car Selection)
        ArrayAdapter<String> adapter = new ArrayAdapter<>(requireContext(), R.layout.spinner_item, carList);
        adapter.setDropDownViewResource(R.layout.spinner_item); // Center align dropdown items and set surface background
        carSelectionSpinner.setAdapter(adapter);

        // Handle SeekBar (Volume Intensity)
        volumeSeekBar.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
                volumeTextView.setText(progress + "%");
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {}

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {}
        });

        // Observe the current RPM value from ViewModel
        homeViewModel.getCurrentRpm().observe(getViewLifecycleOwner(), new Observer<Integer>() {
            @Override
            public void onChanged(Integer rpm) {
                currentRpmTextView.setText(rpm + " RPM"); // Update the RPM value in the UI
            }
        });

        // Start simulating RPM updates
        homeViewModel.simulateRpmUpdates();

        return root;
    }

    @Override
    public void onDestroyView() {
        super.onDestroyView();
        binding = null;
    }
}
