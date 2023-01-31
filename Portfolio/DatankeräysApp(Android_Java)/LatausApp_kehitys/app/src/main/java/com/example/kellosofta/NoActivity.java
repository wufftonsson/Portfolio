package com.example.kellosofta;

import android.app.Activity;
import android.os.Bundle;
// No Actityssä tulostetaan näytölle UI elementit, jotka on määritetty nolayout.xml tiedostossa.
public class NoActivity extends Activity {
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.nolayout);
}}
