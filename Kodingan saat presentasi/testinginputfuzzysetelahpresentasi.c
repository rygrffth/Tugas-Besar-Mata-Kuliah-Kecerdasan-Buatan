#include <stdio.h>

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

int main() {
    // Input suhu dan kelembaban secara manual
    float suhu, kelembaban;
    printf("Masukkan suhu (dalam derajat Celsius): ");
    scanf("%f", &suhu);

    printf("Masukkan kelembaban (dalam persen): ");
    scanf("%f", &kelembaban);

    // Fuzzy Logic - Implementasi aturan fuzzy dan defuzzyfikasi di sini
    float inferensiLambat = inferensi(suhu, kelembaban, suhuDingin, kelembabanRendah, kipasLambat);
    float inferensiSedang = inferensi(suhu, kelembaban, suhuSedang, kelembabanSedang, kipasSedang);
    float inferensiCepat = inferensi(suhu, kelembaban, suhuPanas, kelembabanTinggi, kipasCepat);

    // Defuzzyfikasi (Centroid Method)
    float centroid = (inferensiLambat * 1 + inferensiSedang * 2 + inferensiCepat * 3) / (inferensiLambat + inferensiSedang + inferensiCepat);

    // Tampilkan hasil fuzzy dan kecepatan kipas di Console
    printf("Hasil defuzzyfikasi (kecepatan kipas): %.2f\n", centroid);

    // Kontrol LED berdasarkan kecepatan kipas
    if (centroid <= 1.4) {
        printf("Kipas: Lambat\n");
    } else if (centroid <= 2.3) {
        printf("Kipas: Sedang\n");
    } else {
        printf("Kipas: Cepat\n");
    }

    return 0;
}

// Fungsi Keanggotaan untuk Suhu
float suhuDingin(float suhu) {
    if (suhu >= 20 && suhu < 29)
        return 1;
    else if (suhu > 29 && suhu <= 32)
        return (32 - suhu) / 3;
    else
        return 0;
}

float suhuSedang(float suhu) {
    if (suhu >= 29 && suhu <= 32)
        return 1;
    else if (suhu >= 20 && suhu < 29)
        return (suhu - 20) / 9;
    else if (suhu > 32 && suhu <= 40)
        return (40 - suhu) / 8;
    else
        return 0;
}

float suhuPanas(float suhu) {
    if (suhu >= 33 && suhu <= 40)
        return 1;
    else if (suhu > 29 && suhu < 33)
        return (suhu - 29) / 4;
    else
        return 0;
}

// Fungsi Keanggotaan untuk Kelembaban
float kelembabanRendah(float kelembaban) {
    if (kelembaban >= 0 && kelembaban <= 59)
        return 1;
    else if (kelembaban > 59 && kelembaban <= 66)
        return (66 - kelembaban) / 7;
    else
        return 0;
}

float kelembabanSedang(float kelembaban) {
    if (kelembaban >= 60 && kelembaban <= 66)
        return 1;
    else if (kelembaban > 0 && kelembaban < 60)
        return (kelembaban - 0) / 60;
    else if (kelembaban > 66 && kelembaban <= 100)
        return (100 - kelembaban) / 34;
    else
        return 0;
}

float kelembabanTinggi(float kelembaban) {
    if (kelembaban >= 67 && kelembaban <= 100)
        return 1;
    else if (kelembaban > 59 && kelembaban < 67)
        return (kelembaban - 59) / 8;
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
