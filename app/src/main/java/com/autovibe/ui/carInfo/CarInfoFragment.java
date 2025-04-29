package com.autovibe.ui.carInfo;

import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import androidx.annotation.NonNull;
import androidx.fragment.app.Fragment;
import androidx.lifecycle.ViewModelProvider;
import com.autovibe.databinding.FragmentCarInfoBinding;

public class CarInfoFragment extends Fragment {

    private FragmentCarInfoBinding binding;

    public View onCreateView(@NonNull LayoutInflater inflater,
                             ViewGroup container, Bundle savedInstanceState) {
        CarInfoViewModel carInfoViewModel =
                new ViewModelProvider(this).get(CarInfoViewModel.class);

        // Use View Binding
        binding = FragmentCarInfoBinding.inflate(inflater, container, false);
        View root = binding.getRoot();

        // Set example Average RPM value using View Binding
        binding.carAvgRpm.setText("3500 RPM");

        return root;
    }

    @Override
    public void onDestroyView() {
        super.onDestroyView();
        binding = null;
    }
}
