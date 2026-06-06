#include <Arduino.h>
#include "inventory.h"
#include "ui.h"

// Deklarasi variabel global untuk head dari Linked List
Barang* head_node; 

int main(void) {
    // 1. init() adalah fungsi internal Arduino. 
    // Wajib dipanggil jika kita menggunakan int main() alih-alih setup/loop
    init();

    // 2. Memanggil fungsi inisialisasi sesuai aturan (main hanya untuk memanggil)
    inisialisasiList(&head_node); // Mengirim alamat memori dari head_node menggunakan '&'
    inisialisasiUI();

    // 3. Looping tak terbatas (Menggantikan fungsi loop() bawaan Arduino)
    while (1) {
        // Fungsi ini nanti Anda buat di ui.c untuk terus-menerus mendengarkan Scanner QR
        // bacaInputSerial(); 
        
        // serialEventRun() diperlukan agar Arduino bisa menangkap input Serial di background
        if (serialEventRun) serialEventRun(); 
    }

    return 0;
}