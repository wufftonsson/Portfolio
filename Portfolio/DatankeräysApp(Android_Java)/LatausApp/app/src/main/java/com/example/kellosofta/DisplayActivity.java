package com.example.kellosofta;

import android.annotation.SuppressLint;
import android.app.Activity;
import android.database.Cursor;
import android.database.sqlite.SQLiteDatabase;
import android.os.Bundle;
import android.os.PersistableBundle;
import android.util.Log;
import android.widget.TextView;

import androidx.annotation.Nullable;




// HOX!!! TÄMÄ EI OLE VIELÄ KÄYTÖSSÄ, JOTEN SUORITUS EI VIELÄ TOIMI, EIKÄ TÄLLÄ OLE MERKITYSTÄ. IDEANA OLI ETTÄ TIETOKANNAN TIEDOT OLISI SAANU TULOSTETTUA KELLON NÄYTÖLLE. MUTTA TÄMÄ KUOPATTIIN AIKATAULUONGELMIEN TAKIA
// Tätä ei oikeastaan tarvita, mutta mikäli aikoo ottaa käyttöön tulisi todennäköisesti suunnitella uusiksi.
public class DisplayActivity extends Activity {

    @SuppressLint("ResourceType")
    @Override
    public void onCreate(@Nullable Bundle savedInstanceState, @Nullable PersistableBundle persistentState) {
        super.onCreate(savedInstanceState, persistentState);
        setContentView(R.id.display_layout);

        TextView textHR = findViewById(R.id.textHeart);
        try {
            SQLiteDatabase sensorsDB = this.openOrCreateDatabase("Sensors", MODE_PRIVATE, null);
            Cursor c = sensorsDB.rawQuery("SElECT * FROM sensors",null);

            int idIndex = c.getColumnIndex("id");
            int heartIndex = c.getColumnIndex("heart_rate");


            c.moveToFirst();
            while(c!= null){
                Log.d("Results - id",c.getString(idIndex));
                Log.d("Results - heart_rate",c.getString(heartIndex));
                textHR.setText(getString(heartIndex));


                c.moveToNext();
            }
        }
        catch (Exception e)
        {
            e.printStackTrace();
        }
    }
}
