#include <DHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Function declarations
float suhuDingin(float suhu);
float suhuSedang(float suhu);
float suhuPanas(float suhu);

float kelembabanRendah(float kelembaban);
float kelembabanSedang(float kelembaban);
float kelembabanTinggi(float kelembaban);

float kipasLambat(float value);
float kipasSedang(float value);
float kipasCepat(float value);

float inferensi(float suhu, float kelembaban, float (*fuzzySuhu)(float), float (*fuzzyKelembaban)(float), float (*fuzzyOutput)(float));

#define DHTPIN 2        // Pin tempat Anda menghubungkan sensor DHT22
#define DHTTYPE DHT22   // Jenis sensor DHT yang digunakan

DHT dht(DHTPIN, DHTTYPE);

// Pin untuk LED
int led1Pin = 3;
int led2Pin = 4;
int led3Pin = 5;

// Pin untuk LCD I2C
LiquidCrystal_I2C lcd(0x27, 16, 2); // Alamat I2C LCD: 0x27, Kolom: 16, Baris: 2

void setup() {
  // Set pin LED sebagai OUTPUT
  pinMode(led1Pin, OUTPUT);
  pinMode(led2Pin, OUTPUT);
  pinMode(led3Pin, OUTPUT);

  Serial.begin(9600);
  lcd.init();
  lcd.backlight();

  // Menampilkan pesan awal
  lcd.print("Fuzzy Logic DHT22");
  delay(2000);
  lcd.clear();


  // Inisialisasi LCD
  lcd.begin(16, 2);
  lcd.clear();
  // Inisialisasi Serial Monitor
  Serial.begin(9600);

  // Inisialisasi sensor DHT
  dht.begin();
}

void loop() {
  // Baca suhu dan kelembaban dari sensor DHT
  float suhu = dht.readTemperature();
  float kelembaban = dht.readHumidity();

  // Tampilkan suhu dan kelembaban di Serial Monitor
  Serial.print("Suhu: ");
  Serial.print(suhu);
  Serial.print(" °C, Kelembaban: ");
  Serial.print(kelembaban);
  Serial.println("%");

  // Fuzzy Logic - Implementasi aturan fuzzy dan defuzzyfikasi di sini
  float inferensiLambat = inferensi(suhu, kelembaban, suhuDingin, kelembabanRendah, kipasLambat);
  float inferensiSedang = inferensi(suhu, kelembaban, suhuSedang, kelembabanSedang, kipasSedang);
  float inferensiCepat = inferensi(suhu, kelembaban, suhuPanas, kelembabanTinggi, kipasCepat);

  // Defuzzyfikasi (Centroid Method)
  float centroid = (inferensiLambat * 1 + inferensiSedang * 2 + inferensiCepat * 3) / (inferensiLambat + inferensiSedang + inferensiCepat);

  // Tampilkan hasil fuzzy dan kecepatan kipas di LCD
  lcd.setCursor(0, 0);
  lcd.print("Fuzzy: ");
  lcd.print(centroid, 2);

  // Tampilkan hasil fuzzy dan kecepatan kipas di Serial Monitor
  Serial.print("Hasil defuzzyfikasi (kecepatan kipas): ");
  Serial.print(centroid, 2);

  // Kontrol LED berdasarkan kecepatan kipas
  digitalWrite(led1Pin, LOW);
  digitalWrite(led2Pin, LOW);
  digitalWrite(led3Pin, LOW);
  lcd.setCursor(0, 1);
  if (centroid <= 1.4) {
    digitalWrite(led1Pin, HIGH);
    Serial.println("Kipas: Lambat");
    lcd.print("Kipas: Lambat");
  } else if (centroid <= 2.3) {
    digitalWrite(led2Pin, HIGH);
    Serial.println("Kipas: Sedang");
    lcd.print("Kipas: Sedang");
  } else {
    digitalWrite(led3Pin, HIGH);
    Serial.println("Kipas: Cepat");
    lcd.print("Kipas: Cepat");
  }

  delay(500); // Delay untuk membaca dan menampilkan data dengan interval tertentu
}

// Fungsi Keanggotaan untuk Suhu
float suhuDingin(float suhu) {
    if (suhu >= 0 && suhu <= 20)
        return 1;
    else if (suhu > 20 && suhu <= 26)
        return (26 - suhu) / 6;
    else
        return 0;
}

float suhuSedang(float suhu) {
    if (suhu >= 20 && suhu <= 26)
        return 1;
    else if (suhu > 0 && suhu < 20)
        return (suhu - 0) / 20;
    else if (suhu > 26 && suhu <= 40)
        return (40 - suhu) / 14;
    else
        return 0;
}

float suhuPanas(float suhu) {
    if (suhu >= 27 && suhu <= 40)
        return 1;
    else if (suhu > 20 && suhu < 27)
        return (suhu - 20) / 7;
    else
        return 0;
}

// Fungsi Keanggotaan untuk Kelembaban
float kelembabanRendah(float kelembaban) {
    if (kelembaban >= 0 && kelembaban <= 40)
        return 1;
    else if (kelembaban > 40 && kelembaban <= 60)
        return (60 - kelembaban) / 20;
    else
        return 0;
}

float kelembabanSedang(float kelembaban) {
    if (kelembaban >= 41 && kelembaban <= 60)
        return 1;
    else if (kelembaban > 0 && kelembaban < 41)
        return (kelembaban - 0) / 40;
    else if (kelembaban > 60 && kelembaban <= 100)
        return (100 - kelembaban) / 40;
    else
        return 0;
}

float kelembabanTinggi(float kelembaban) {
    if (kelembaban >= 61 && kelembaban <= 100)
        return 1;
    else if (kelembaban > 40 && kelembaban < 61)
        return (kelembaban - 40) / 20;
    else
        return 0;
}

// Fungsi Keanggotaan untuk Kecepatan Kipas
float kipasLambat(float value) {
    if (value >= 0 && value <= 1)
        return 1;
    else if (value > 1 && value <= 2)
        return (2 - value) / 1;
    else
        return 0;
}

float kipasSedang(float value) {
    if (value >= 1 && value <= 2)
        return 1;
    else if (value > 0 && value < 1)
        return value;
    else if (value > 2 && value <= 3)
        return (3 - value) / 1;
    else if (value > 3 && value <= 4)
        return (4 - value) / 1;
    else
        return 0;
}

float kipasCepat(float value) {
    if (value >= 2 && value <= 3)
        return 1;
    else if (value > 1 && value < 2)
        return (value - 1) / 1;
    else
        return 0;
}

// Fungsi Inferensi (Mamdani)
float inferensi(float suhu, float kelembaban, float (*fuzzySuhu)(float), float (*fuzzyKelembaban)(float), float (*fuzzyOutput)(float)) {
    float hasilInferensi[5];
    hasilInferensi[0] = fmin(fuzzySuhu(suhu), fuzzyKelembaban(kelembaban));
    hasilInferensi[1] = fmin(fuzzySuhu(suhu), fuzzyKelembaban(kelembaban));
    hasilInferensi[2] = fmin(fuzzySuhu(suhu), fuzzyKelembaban(kelembaban));

    // Sesuaikan aturan fuzzy untuk kecepatan kipas cepat pada kondisi suhu tinggi dan kelembaban rendah
    hasilInferensi[3] = fmin(hasilInferensi[2], fuzzyOutput(kelembaban));

    float maxInferensi = hasilInferensi[0];
    for (int i = 1; i < 4; ++i) {
        if (hasilInferensi[i] > maxInferensi)
            maxInferensi = hasilInferensi[i];
    }

    return maxInferensi;
}
