#include "ui.h"
#include "inventory.h"
#include <Arduino.h>
#include <string.h> 
#include <stdlib.h>

// Deklarasi extern agar file ini bisa melihat variabel global dari main.ino
extern Barang* head_node;
extern Barang* tail_node; 

void inisialisasiUI() {
    Serial.begin(9600);
    delay(100); 
    Serial.println(F("========================================="));
    Serial.println(F(" SISTEM INVENTARISASI LAB EMBEDDED SYSTEM"));
    Serial.println(F("========================================="));
    Serial.println(F("Status: Memori Kosong (0 Barang)"));
    Serial.println(F("Menunggu input dari Scanner QR...\n"));
}

// FUNGSI BARU: Wajib ada untuk menangkap ketikan Anda dari Serial Monitor
void bacaInputSerial() {
    // Mengecek apakah ada data/teks yang masuk
    if (Serial.available() > 0) {
        // Membaca semua teks sampai Anda menekan tombol Enter ('\n')
        String input = Serial.readStringUntil('\n');
        input.trim(); // Membuang spasi kosong di awal/akhir teks

        if (input.length() > 0) {
            // Mengubah tipe String bawaan C++ menjadi array char C murni
            char buffer[150];
            input.toCharArray(buffer, sizeof(buffer));

            // Melempar teks yang sudah ditangkap ke fungsi pemotong kata
            parseDanEksekusi(buffer);
        }
    }
}

void parseDanEksekusi(char* input_teks) {
    // Memotong KATA PERTAMA dari teks sebelum tanda koma (,)
    // Contoh: Jika input "ADD,101,Multimeter", maka token_perintah akan berisi "ADD"
    char* token_perintah = strtok(input_teks, ",");

    if (token_perintah == NULL) return; // Jika kosong, batalkan eksekusi

    // Pengecekan perintah
    if (strcmp(token_perintah, "ADD") == 0) {
        int kode_pesan;
        // (Sisa parameter hardcode untuk menguji coba logika Linked List)
        tambahBarang(&head_node, &tail_node, 101, "Sensor Suhu", "Modul", 5, "RakA", "Tersedia", "Lab", "Budi", &kode_pesan);
        cetakPesan(kode_pesan);
    } 
    else if (strcmp(token_perintah, "DEL") == 0) {
        Serial.println(F("-> BERHASIL: Perintah DEL diterima (Fungsi Hapus belum aktif)."));
    } 
    else if (strcmp(token_perintah, "FIND") == 0) {
        Barang* hasil = NULL;
        cariBarang(head_node, 101, &hasil);
        
        if (hasil != NULL) {
            Serial.println(F("-> BERHASIL: Barang [ID: 101] Ditemukan di dalam memori!"));
        } else {
            cetakPesan(PESAN_ID_TIDAK_DITEMUKAN);
        }
    } 
    else if (strcmp(token_perintah, "UPD") == 0) {
        char* token_id = strtok(NULL, ",");
        char* token_stok = strtok(NULL, ",");
        char* token_status = strtok(NULL, ",");
    
        if (token_id == NULL || token_stok == NULL || token_status == NULL) {
            Serial.println(F("-> ERROR: Format perintah UPD salah"));
            return;
        }

        //menguba string ke integer 
        unsigned int id = (unsigned int)strtoul(token_id, NULL, 10);
        int stok_baru = (int)strtol(token_stok, NULL, 10);
        int kode_pesan;

        updateBarang(head_node, id, stok_baru, token_status, &kode_pesan);

        //cetak pesan hasil update
        cetakPesan(kode_pesan);
    } 
    else if (strcmp(token_perintah, "PRINT") == 0) {
        Serial.println(F("-> Menjalankan perintah PRINT (Fungsi Tampil belum aktif)."));
    } 
    else {
        Serial.println(F("-> ERROR: Perintah tidak dikenali oleh sistem."));
    }
}

void cetakPesan(int kode_pesan){
    switch (kode_pesan) {
        case PESAN_UPDATE_BERHASIL:
            Serial.println(F("-> BERHASIL: Stok dan status barang berhasil diperbarui!"));
            break;
        case PESAN_DATA_KOSONG:
            Serial.println(F("-> GAGAL: Inventaris kosong."));
            break;
        case PESAN_ID_TIDAK_DITEMUKAN:
            Serial.println(F("-> GAGAL: ID barang tidak ditemukan."));
            break;
        case PESAN_STOK_TIDAK_VALID:
            Serial.println(F("-> GAGAL: Stok baru tidak valid."));
            break;
        case PESAN_STATUS_TIDAK_VALID:
            Serial.println(F("-> GAGAL: Status baru tidak valid."));
            break;
        case PESAN_ID_DUPLIKAT:
            Serial.println(F("-> GAGAL: ID barang sudah digunakan."));
            break;
        case PESAN_ALOKASI_GAGAL:
            Serial.println(F("-> GAGAL: Alokasi memori untuk barang baru gagal."));
            break;
        case PESAN_TAMBAH_BERHASIL:
            Serial.println(F("-> BERHASIL: Barang berhasil ditambahkan ke inventaris."));
            break;
        default:
            Serial.println(F("-> ERROR: Kode pesan tidak dikenali."));
            break;
    }
}