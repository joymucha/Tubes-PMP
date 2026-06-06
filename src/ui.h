// --- File: ui.h ---

#ifndef UI_H
#define UI_H

// Menyertakan inventory.h agar UI mengenali tipe data 'Barang' jika perlu mencetak datanya
#include "inventory.h"

// Deklarasi Fungsi Void untuk Antarmuka dan Parsing

// Menyalakan komunikasi Serial Arduino (baud rate 9600)
void inisialisasiUI(void);

// Mendengarkan teks yang diketik oleh alat Scanner QR
void bacaInputSerial(void);

// Memecah teks dari scanner (strtok) dan memanggil fungsi di inventory.c
void parseDanEksekusi(char* input_teks);

// Menelusuri seluruh node dari head ke tail untuk dicetak ke Serial Monitor
void tampilkanSemuaBarang(void);

// Pusat penanganan peringatan dan konfirmasi (Misal: Memori penuh, ID ganda, Berhasil dihapus)
void cetakPesan(int kode_pesan);

#endif