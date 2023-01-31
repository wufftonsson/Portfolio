package com.example.kellosofta;


import static android.hardware.Sensor.TYPE_HEART_RATE;
import static android.os.Build.ID;


import android.app.job.JobParameters;
import android.app.job.JobService;

import android.content.ContentValues;
import android.content.Context;
import android.database.Cursor;
import android.database.sqlite.SQLiteDatabase;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.hardware.lights.Light;
import android.nfc.Tag;
import android.util.Log;

import java.time.LocalDateTime;
import java.util.List;

// Tässä servicessa haetaan data antureilta ja se tallennetaan tietokantaan doDatacollection funktiossa.

public class DataService extends JobService implements SensorEventListener{
    private static final String TAG = "DataService";
    private boolean jobCancelled = false;

// Muuttujat sensori dataa varten, mSensorManageria käytetään sensorien hakemiseen järjestelmästäs, mSensorHeart on sydämmen sykesensoria varten ja currentValue sensorista saatavaa arvoa varten.
    private SensorManager mSensorManager;
    private Sensor mSensorHeart;
    float currentValue;



// Kun tausta-ajo luodaan
    @Override
    public boolean onStartJob(JobParameters params) {
        // Haetaan anturilista järjestelmästä
        mSensorManager = (SensorManager) getSystemService((Context.SENSOR_SERVICE));
        // Haetaan sydämensyken anturi mSensorManagerista.
        mSensorHeart = mSensorManager.getDefaultSensor(TYPE_HEART_RATE);

        String sensor_error = getResources().getString(R.string.Sens_error);

        // Tarkistetaan löytyikkö tarvittava anturi
        if (mSensorHeart == null) {
            Log.d(TAG, sensor_error);
        }
        // Mikäli löytyi alustetaan anturi kuuntelija.
        if (mSensorHeart != null) {
            mSensorManager.registerListener((SensorEventListener) this, mSensorHeart, SensorManager.SENSOR_DELAY_FASTEST);
        }
        SQLiteDatabase sensorsDB = null;
        // Avataan tai luodaan sensors niminen tietokanta mikäli sellaista ei vielä ole.
        sensorsDB = this.openOrCreateDatabase("Sensors", MODE_PRIVATE, null);
        // Siirrettään tiedot doDataColletion funktiolle.
        doDataCollection(params, sensorsDB);

        return true;
    }

    private void doDataCollection(final JobParameters params, SQLiteDatabase sensorsDB){
        new Thread(new Runnable(){
            // Luodaan säie jossa luodaan tietokanta "pöytä" johon talletaan ID, sydämensyke. Lopuksi sensorsDB.closella varmistetaan että tietokanta suljetaan joka käytön jälkeen. Muuten saattaa aiheutua muistivuoto.
            @Override
            public void run(){
                int id = 1;
                    Log.d(TAG,"doDataCollection");
                    try {
                        sensorsDB.execSQL("CREATE TABLE IF NOT EXISTS sensors (id INT(2),heart_rate INT(4))");
                        ContentValues contentValues = new ContentValues();
                        contentValues.put("id", id);
                        contentValues.put("heart_rate", currentValue);
                         sensorsDB.insert("sensors", null, contentValues);
                        sensorsDB.close();
                    }
                    catch (Exception e){
                        e.printStackTrace();
                    }

                    if(jobCancelled){
                        return;
                    }
                Log.d(TAG,"Job finished");
                    // Taustatyö valmis, ajastetaan uudelleen suoritus.
                jobFinished(params,true);
            }
        }). start();
    }
    @Override
    public boolean onStopJob(JobParameters params){
        Log.d(TAG,"Job cancelled before completion");
                // Tällä varmistetaan että tausta-ajo sammuu jos jostain syystä sen suoritus loppuu ennen aikoja.
                jobCancelled = true;
                return true;
    }


    // Ensin tallennetaan sensorin tyyppi sensortype muuttujaan, Listeneri tallentaa sensori dataa sensorEvent.values taulukkoon, sieltä indexissä nolla on sydämen syke arvo.
    @Override
    public void onSensorChanged(SensorEvent sensorEvent) {

        /*Log.d(TAG,"Olentäällä");
        int sensorType = sensorEvent.sensor.getType();
        currentValue = sensorEvent.values[0];
        switch (sensorType){
            case TYPE_HEART_RATE:
                Log.d(TAG, String.valueOf(currentValue));
                mSensorManager.unregisterListener(this);
                break;

            default:

        }*/
        // Erilainen tapa toteuttaa sensorien luku, mutta jostain syystä ei toimi kun lisätään unregister listener. tarvitsee lisää työtä.
        if(sensorEvent.sensor.getType()== TYPE_HEART_RATE){
            do {
                currentValue = sensorEvent.values[0];
                Log.d(TAG, String.valueOf(currentValue));
            }while(currentValue == 0);
        }
        else{
            Log.d(TAG,"Pieleen män");
        }
        Log.d(TAG,"Kuuntelia pois");
        mSensorManager.unregisterListener(this);

    }

        




    @Override
    public void onAccuracyChanged(Sensor sensor, int i) {

    }
}


