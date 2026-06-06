#include "ui.h"
#include "inventory.h"
#include <Arduino.h>
#include <string.h> 

extern Barang* head_node;

void inisialisasiUI() {
    // Membuka jalur komunikasi dengan baud rate 9600 (standar Arduino)
    Serial.begin(9600);
    
    // Memberikan jeda kecil agar Serial siap
    delay(100); 
    
    // Mencetak pesan awal ke layar
    Serial.println("=========================================");
    Serial.println(" SISTEM INVENTARISASI LAB EMBEDDED SYSTEM");
    Serial.println("=========================================");
    Serial.println("Status: Memori Kosong (0 Barang)");
    Serial.println("Menunggu input dari Scanner QR...\n");
}

void parseDanEksekusi(char* input_teks) {
    // Simulasi memecah perintah pertama (contoh: "ADD", "DEL", "PRINT")
    // Di program aslinya, Anda menggunakan strtok() di sini.
    char perintah[10]; 
    
    // --- PEMANGGILAN SISA FUNGSI BERADA DI SINI ---

    if (strcmp(perintah, "ADD") == 0) {
        // Memanggil fungsi dari inventory.h
        tambahBarang(101, "Sensor Suhu", "Modul", 5, "RakA", "Ada", "Lab", "Budi");
        // Memanggil fungsi dari ui.h
        cetakPesan(1); // 1 = Pesan sukses tambah
    } 
    else if (strcmp(perintah, "DEL") == 0) {
        // Memanggil fungsi dari inventory.h
        hapusBarang(101);
        cetakPesan(2); // 2 = Pesan sukses hapus
    } 
    else if (strcmp(perintah, "FIND") == 0) {
        Barang* hasil = NULL;
        // Memanggil fungsi dari inventory.h (menggunakan referensi pointer)
        cariBarang(101, &hasil);
        
        if (hasil != NULL) {
            Serial.println("Barang Ditemukan!");
        } else {
            cetakPesan(404); // 404 = Pesan Error Tidak Ditemukan
        }
    } 
    else if (strcmp(perintah, "UPD") == 0) {
        // Memanggil fungsi dari inventory.h
        updateBarang(101, 10, "Tersedia");
        cetakPesan(3); 
    } 
    else if (strcmp(perintah, "PRINT") == 0) {
        // Memanggil fungsi dari ui.h
        tampilkanSemuaBarang();
    } 
    else {
        // Memanggil fungsi dari ui.h untuk error
        cetakPesan(99); // 99 = Pesan Perintah Tidak Dikenal
    }
}