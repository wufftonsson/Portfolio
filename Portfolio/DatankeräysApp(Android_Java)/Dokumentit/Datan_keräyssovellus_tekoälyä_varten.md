Datan keräyssovellus tekoälyä varten
Wear OS App
===
##  Polar projekti syksy 2022
#### Projektiopinnot 4
Christian Antonsson & Jere Kemppainen TTV19SA 

## Huomioita

Koodia on testattu sekä emulaattorilla, sekä fyysisellä Polar M600 -kellolla.

Kelloissa on eroja. Ensimmäinen kello antoi suoritusajan vartin kuluttua koodin ajamisesta. Loppupuolella kokeilimme toista samanlaista kelloa, joka alkaa heti suorittamaan ajettua koodia. Jälkimmäinen kello helpotti huomattavasti koodin testaamista. Harmittavasti tämä huomattiin vasta projektin lopussa.

Kellon johto on todella herkkä irtoamaan, joten ohjelmaa testatessa on syytä käsitellä kelloa varoen.

Ajoittain on suotavaa käynnistää Android Studio uudestaan testauksen yhteydessä. 

Kellon sykeanturi toimii välillä mielenkiintoisesti. Antaa esimerkiksi sykelukuja ilman, että sormi on sensorilla. Tähän mahdollinen ratkaisu voi olla kellon palauttaminen tehdas asetuksiin, mutta ongelma voi myös liittyä koodiin. Kyseinen ongelma rakentui ajankuluessa.




## Kellon ottaminen käyttöön Android Studiossa:


Kytke kello tietokoneeseen USB-portilla. Kellon asetuksista kohdasta About -> Build Number -> paina 7 kertaa. Näin saat Developer Moden päälle. Ohjeet myös videomuodossa: https://www.youtube.com/watch?v=xoGbnbEM1z0

Sen jälkeen Asetusvalikosta löytyy About kohdan alta Developer Settings. Anna sieltä lupa *ADB Debugging* kohtaan. Lisäksi on järkevää laittaa päälle asetus, jossa näyttö ei sammu muuten kuin latauksessa.

Android Studion pitäisi tässä kohtaa jo havaita laite (esim. Polar M600) ja näyttää se Device Managerissa. 

Kun koodi on ajettu kelloon, valitse kellon Settings -> Apps -> LatausApp -> Permission -> täppää Sensors kohtaan lupa. Näin ajettava sovellus pääsee lukemaan ja saa käyttöönsä kellon sensoridatan. __*Huom!*__ Tämä toimenpide on suoritettava joka kerta, kun App asennetaan uudestaan. Eli kun koodia testa

## Ohjelmiston toiminta

Sovellusta ajettaessa kelloon käynnistyvä aloitusikkuna sisältää kysymyksen "Haluatko ladata kelloa?" Valitsemalla kyllä tai ei, sovellus etenee uuteen ikkunaan vahvistaen käyttäjän valinnan. Tähän lopetusikkunaan ei ole tehty automaattista sulkemista. Käyttäjän valintaa ei myöskään tallenneta mihinkään. Vasemmalla alhaalla oleva painike "Kerää Dataa" on nimensä mukaisesti datan keräystä varten. Tämän hetkisessä koodissa sykesensori on ainut sensori. Ajatuksena oli lisätä gyroscope, mutta emme saaneet koodia toimimaan. Mahdollisesti Accelerometer voisi olla vaihtoehto, mutta emme ehtineet sitä testaamaan.

Ohjelmisto on luotu toimimaan tausta-ajossa. Kun ohjelma käynnistetään ja anturin datahaku käynnistetään, anturidataa haetaan myös ohjelmiston sulkemisen jälkeen.


Koodin ajaminen tapahtuu Android Studion kautta Run painikkeella. Kun koodi on ajettu, kello avaa sovelluksen. Painamalla ""Kerää Dataa" painiketta datan keräys alkaa. Tämän voi havaita Android Studion alapalkissa olevasta **Logcat** kohdasta, johon pitäisi tulostua teksti *Job Scheduled*. Jos siihen tulostuu vain jotain muuta, kannattaa kokeilla uudestaan. Tämän jälkeen siihen pitäisi tulostua kellon lukemaan sykedataa epätasaisen tasaisin väliajoin. Asettamalla sormen kellon sykeanturille pitäisi tulostua muutakin kuin 0.0.


Tietokanta luodaan ohjelman alussa mikäli sitä ei ole luotu edellisellä ajokerralla. Sykeanturin luvut on ohjelmoitu myös tallentumaan kellon MySQL-tietokantaan. Tietokantaan tallentuu myös kellon ID. Testatessamme gyroscopea saimme tietokannassa luotua ja näkymään sensorin otsikot, mutta lukemia emme saaneet ja ajon aikana tapahtui tietokannan ylivuoto.



## Tietokannan lukeminen

Miten se tietokanta sitten löytyy Android Studiosta?
- Device Manager 
- Physical
- Actions kohdan alapuolella oleva kansion kuvake
- data
- data
- kellosofta 
- database 
- Sensors
- paina Mouse2 ja Save as..  
- Tallenna esimerkiksi työpöydälle
- Avaa koneelle ladattu DB Browser for SQLite (https://sqlitebrowser.org/)
- Raahaa tallennettu tietokanta DB Browseriin



## Mahdollisia jatkotoimenpiteitä

__Koodi on siis muutettu takaisin pisteeseen, missä sen pitäisi toimia ongelmitta.__
Toistaiseksi ohjelmistoa ei ole testattu käytännössä eli pitämällä kelloa kädessä. Ohjelmistoon täytyy lisätä lisää antureita. Accelerometer voisi olla hyödyllinen. 

Myös aikaleiman lisääminen oli ajatuksena sarakkeeksi tietokantaan ja sitä kokeilimmekin. Se toimi, mutta samalla ohjelman kokonaisuus lakkasi toimimasta. Tässä kohtaa projekti eli viimeisiä hetkiä, joten selvittämättä jäi vian syy. Johtuiko se aikaleimasta vai siitä, ettei tietokantaa suljettu tallennuksen jälkeen.
 

Kellon tietokanta on tarkoitus lähettää laajempaan, koulun palvelimelle luotavaan tietokantaan. Käypiä vaihtoehtoja koulun palvelimeksi voisi olla ainakin MariaDB, PostgreSQL tai InfluxDB. Näistä ei ole vielä tarkemmin tutkittu, mikä olisi paras tai sopivin.  

Alustava ajatus kuljettaa kellon tietokanta koulun tietokantaan on tehdä se langattomasti. Niin pitkälle emme kuitenkaan selvinneet, jotta asia olisi tarkemmin otettu työn alle.
