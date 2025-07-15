package com.autovibe.screens.carInfo;

import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

import androidx.annotation.NonNull;
import androidx.fragment.app.Fragment;
import androidx.lifecycle.ViewModelProvider;

import com.autovibe.databinding.FragmentCarInfoBinding;
import com.autovibe.screens.home.HomeViewModel;

public class CarInfoFragment extends Fragment {

    private FragmentCarInfoBinding binding;
    private HomeViewModel homeViewModel;

    public View onCreateView(@NonNull LayoutInflater inflater,
                             ViewGroup container, Bundle savedInstanceState) {

        binding = FragmentCarInfoBinding.inflate(inflater, container, false);
        View root = binding.getRoot();

        homeViewModel = new ViewModelProvider(requireActivity()).get(HomeViewModel.class);

        homeViewModel.getCarSelection().observe(getViewLifecycleOwner(), selectedCar -> {
            CarDetails details = homeViewModel.getSelectedCarDetails();
            if (details != null) {
                binding.carMaxRpm.setText(details.getMaxRpm() + " RPM");
                binding.carBrand.setText(details.getBrand());
                binding.carEngineType.setText(details.getEngineType());
                binding.carReleaseYear.setText(String.valueOf(details.getReleaseYear()));
            }
        });

        return root;
    }

    @Override
    public void onDestroyView() {
        super.onDestroyView();
        binding = null;
    }
}

