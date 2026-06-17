#include "ui.h"
#include "inventory.h"
#include <Arduino.h>
#include <string.h> 

// Deklarasi extern agar file ini bisa melihat variabel global dari main.ino
extern Barang* head_node;
extern Barang* tail_node; 

void inisialisasiUI() {
    Serial.begin(9600);
    delay(100); 
    Serial.println("=========================================");
    Serial.println(" SISTEM INVENTARISASI LAB EMBEDDED SYSTEM");
    Serial.println("=========================================");
    Serial.println("Status: Memori Kosong (0 Barang)");
    Serial.println("Menunggu input dari Scanner QR...\n");
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
        // (Sisa parameter hardcode untuk menguji coba logika Linked List)
        tambahBarang(&head_node, &tail_node, 101, "Sensor Suhu", "Modul", 5, "RakA", "Ada", "Lab", "Budi");
        Serial.println("-> BERHASIL: Barang [ID: 101] ditambahkan ke antrean memori!");
    } 
    else if (strcmp(token_perintah, "DEL") == 0) {
        Serial.println("-> BERHASIL: Perintah DEL diterima (Fungsi Hapus belum aktif).");
    } 
    else if (strcmp(token_perintah, "FIND") == 0) {
        Barang* hasil = NULL;
        cariBarang(head_node, 101, &hasil);
        
        if (hasil != NULL) {
            Serial.println("-> BERHASIL: Barang [ID: 101] Ditemukan di dalam memori!");
        } else {
            Serial.println("-> GAGAL: Barang tidak ditemukan (Memori kosong / ID salah).");
        }
    } 
    else if (strcmp(token_perintah, "UPD") == 0) {
        Serial.println("-> BERHASIL: Perintah UPD diterima (Fungsi Update belum aktif).");
    } 
    else if (strcmp(token_perintah, "PRINT") == 0) {
        Serial.println("-> Menjalankan perintah PRINT (Fungsi Tampil belum aktif).");
    } 
    else {
        Serial.println("-> ERROR: Perintah tidak dikenali oleh sistem.");
    }
}