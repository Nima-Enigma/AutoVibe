package com.autovibe.ui.contactUs;

import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;
import androidx.annotation.NonNull;
import androidx.fragment.app.Fragment;
import androidx.lifecycle.ViewModelProvider;

import com.autovibe.databinding.FragmentContactUsBinding;

public class ContactUsFragment extends Fragment {

private FragmentContactUsBinding binding;

    public View onCreateView(@NonNull LayoutInflater inflater,
            ViewGroup container, Bundle savedInstanceState) {
        ContactUsViewModel contactUsViewModel =
                new ViewModelProvider(this).get(ContactUsViewModel.class);

    binding = FragmentContactUsBinding.inflate(inflater, container, false);
    View root = binding.getRoot();
    
        return root;
    }

@Override
    public void onDestroyView() {
        super.onDestroyView();
        binding = null;
    }
}