package com.example.kellosofta;

import static android.app.PendingIntent.getActivity;

import android.app.Activity;
import android.app.job.JobInfo;
import android.app.job.JobScheduler;
import android.content.ComponentName;
import android.content.Intent;
import android.os.Build;
import android.os.Bundle;
import android.util.Log;
import android.view.View;

import android.widget.Button;
import android.widget.ImageButton;


import androidx.annotation.RequiresApi;

import com.example.kellosofta.databinding.ActivityMainBinding;

public class MainActivity extends Activity {
    private static final String TAG = "MainActivity";
    // ladataanko muuttuja tehty nappien toimintaa varten, olisi tarkoitus että se mitä käyttäjä painaa otetaan talteen ja tallentaan tietokantaan.
    protected boolean ladataanko;
    // näytön napit ovat ns kuvanappeja.
    ImageButton yButton;
    ImageButton nButton;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
            // Nappien alustukset.
            yButton = (ImageButton) findViewById(R.id.Yes_Button);
            nButton = (ImageButton) findViewById(R.id.No_Button) ;
        Button dButton = (Button) findViewById(R.id.dataButton);
        Button sButton = (Button) findViewById(R.id.showData) ;

        // Kuuntelijat halutaanko ladata napeille, tarkoitus olisi että tämä tieto tallennetaan tietokantaan. Mutta sitä ei saatu toteutettua. Kummallekkin vastaukselle tehty oma activity.
            yButton.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View v) {
                    ladataanko = true;
                    Intent yes = new Intent(MainActivity.this,YesActivity.class);
                    startActivity(yes);
                }
            });

             nButton.setOnClickListener(new View.OnClickListener() {
                 @Override
                 public void onClick(View v) {
                     ladataanko = false;
                    Intent no = new Intent(MainActivity.this, NoActivity.class);
                     startActivity(no);


                 }
            });
            // data napi
             dButton.setOnClickListener(new View.OnClickListener() {
                 @RequiresApi(api = Build.VERSION_CODES.P)
                 @Override
                 // kun nappia on painettu Jobbuilderille annettaan seuraavat arvot.
                 public void onClick(View view) {
                     ComponentName componentName = new ComponentName(MainActivity.this,DataService.class);
                     // luodaan info johon asetetaan seuraavat ehdot tausta-ajon suorittamista varten.
                     JobInfo info = new JobInfo.Builder(111,componentName)
                                // tarvitseeko laitteen olla latauksessa (ei)
                             .setRequiresCharging(false)
                             // minkälainen internetyhteys vaaditaan (ei väliä)
                             .setRequiredNetworkType(JobInfo.NETWORK_TYPE_ANY)
                             // säilyykö suoritus rebootin yli (kyllä)
                             .setPersisted(true)
                             // asetaan suoritusaika väli (15 minuuttia)
                             .setPeriodic(15*60*1000)
                             // kasataan tiedot valmiiksi.
                             .build();
                     // alla tarkistetaan onnistuiko suorituksen ajastus.
                     JobScheduler scheduler = (JobScheduler) getSystemService(JOB_SCHEDULER_SERVICE);
                     int resultCode = scheduler.schedule(info);

                     if(resultCode == JobScheduler.RESULT_SUCCESS){
                         Log.d(TAG," Job scheduled");
                     }
                     else{
                         Log.d(TAG,"Job scheduling failed.");
                     }

                 }
                 // Katso lisätietoja  https://developer.android.com/reference/android/app/job/JobInfo.Builder
             });

             // tämä ei vielä toimi. Lisä tietoa DisplayActivityssä.
            sButton.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View view) {
                    Intent show = new Intent(MainActivity.this,DisplayActivity.class);
                    startActivity(show);
                }
            });


    }











}
