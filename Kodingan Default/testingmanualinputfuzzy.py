# Fungsi Keanggotaan untuk Suhu
def suhu_dingin(suhu):
    if 0 <= suhu <= 20:
        return 1
    elif 20 < suhu <= 26:
        return (26 - suhu) / 6
    else:
        return 0

def suhu_sedang(suhu):
    if 20 <= suhu <= 26:
        return 1
    elif 0 < suhu < 20:
        return (suhu - 0) / 20
    elif 26 < suhu <= 40:
        return (40 - suhu) / 14
    else:
        return 0

def suhu_panas(suhu):
    if 27 <= suhu <= 40:
        return 1
    elif 20 < suhu < 27:
        return (suhu - 20) / 7
    else:
        return 0

# Fungsi Keanggotaan untuk Kelembaban
def kelembaban_rendah(kelembaban):
    if 0 <= kelembaban <= 40:
        return 1
    elif 40 < kelembaban <= 60:
        return (60 - kelembaban) / 20
    else:
        return 0

def kelembaban_sedang(kelembaban):
    if 41 <= kelembaban <= 60:
        return 1
    elif 0 < kelembaban < 41:
        return (kelembaban - 0) / 40
    elif 60 < kelembaban <= 100:
        return (100 - kelembaban) / 40
    else:
        return 0

def kelembaban_tinggi(kelembaban):
    if 61 <= kelembaban <= 100:
        return 1
    elif 40 < kelembaban < 61:
        return (kelembaban - 40) / 20
    else:
        return 0

# Fungsi Keanggotaan untuk Kecepatan Kipas
def kipas_lambat(value):
    if 0 <= value <= 1:
        return 1
    elif 1 < value <= 2:
        return (2 - value) / 1
    else:
        return 0

def kipas_sedang(value):
    if 1 <= value <= 2:
        return 1
    elif 0 < value < 1:
        return value
    elif 2 < value <= 3:
        return (3 - value) / 1
    elif 3 < value <= 4:
        return (4 - value) / 1
    else:
        return 0

def kipas_cepat(value):
    if 2 <= value <= 3:
        return 1
    elif 1 < value < 2:
        return (value - 1) / 1
    else:
        return 0

# Fungsi Inferensi (Mamdani)
def inferensi(suhu, kelembaban, fuzzy_suhu, fuzzy_kelembaban, fuzzy_output):
    hasil_inferensi = [
        min(fuzzy_suhu(suhu), fuzzy_kelembaban(kelembaban)),
        min(fuzzy_suhu(suhu), fuzzy_kelembaban(kelembaban)),
        min(fuzzy_suhu(suhu), fuzzy_kelembaban(kelembaban)),
    ]

    # Sesuaikan aturan fuzzy untuk kecepatan kipas cepat pada kondisi suhu tinggi dan kelembaban rendah
    hasil_inferensi.append(min(hasil_inferensi[2], fuzzy_output(kelembaban)))

    max_inferensi = hasil_inferensi[0]
    for i in range(1, 4):
        if hasil_inferensi[i] > max_inferensi:
            max_inferensi = hasil_inferensi[i]

    return max_inferensi

# Input manual suhu dan kelembaban
suhu = float(input("Masukkan suhu (0-40): "))
kelembaban = float(input("Masukkan kelembaban (0-100): "))

# Inferensi dan Output
inferensi_lambat = inferensi(suhu, kelembaban, suhu_dingin, kelembaban_rendah, kipas_lambat)
inferensi_sedang = inferensi(suhu, kelembaban, suhu_sedang, kelembaban_sedang, kipas_sedang)
inferensi_cepat = inferensi(suhu, kelembaban, suhu_panas, kelembaban_tinggi, kipas_cepat)

# Defuzzyfikasi (Centroid Method)
centroid = (inferensi_lambat * 1 + inferensi_sedang * 2 + inferensi_cepat * 3) / (
        inferensi_lambat + inferensi_sedang + inferensi_cepat
)

print(f"Hasil defuzzyfikasi (kecepatan kipas): {centroid:.2f}")

if centroid <= 1.4:
    print("Kecepatan Kipas: Lambat")
elif centroid < 2:
    print("Kecepatan Kipas: Sedang")
else:
    print("Kecepatan Kipas: Cepat")
