#include <stdio.h>

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

int main() {
    // Input manual suhu dan kelembaban
    float suhu, kelembaban;
    printf("Masukkan suhu (0-40): ");
    scanf("%f", &suhu);
    printf("Masukkan kelembaban (0-100): ");
    scanf("%f", &kelembaban);

    // Inferensi dan Output
    float inferensiLambat = inferensi(suhu, kelembaban, suhuDingin, kelembabanRendah, kipasLambat);
    float inferensiSedang = inferensi(suhu, kelembaban, suhuSedang, kelembabanSedang, kipasSedang);
    float inferensiCepat = inferensi(suhu, kelembaban, suhuPanas, kelembabanTinggi, kipasCepat);

    // Defuzzyfikasi (Centroid Method)
    float centroid = (inferensiLambat * 1 + inferensiSedang * 2 + inferensiCepat * 3) / (inferensiLambat + inferensiSedang + inferensiCepat);

    printf("Hasil defuzzyfikasi (kecepatan kipas): %.2f\n", centroid);

    if (centroid <= 1.4) {
        printf("Kecepatan Kipas: Lambat\n");
    } else if (centroid <2) {
        printf("Kecepatan Kipas: Sedang\n");
    } else {
        printf("Kecepatan Kipas: Cepat\n");
    }

    return 0;
}
