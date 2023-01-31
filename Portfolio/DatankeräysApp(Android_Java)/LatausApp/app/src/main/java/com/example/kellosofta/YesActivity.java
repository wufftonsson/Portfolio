package com.example.kellosofta;

import android.app.Activity;
import android.os.Bundle;

// // Yes Actityssä tulostetaan näytölle UI elementit, jotka on määritetty yes_layout.xml tiedostossa.
public class YesActivity extends Activity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.yes_layout);
    }
}
