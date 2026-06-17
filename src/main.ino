#include <Arduino.h>
#include "inventory.h"
#include "ui.h"

// Deklarasi variabel global
Barang* head_node = NULL; 
Barang* tail_node = NULL; 

int main(void) {
    // Menyiapkan perangkat keras Arduino
    init();

    // Inisialisasi awal
    inisialisasiList(&head_node); 
    inisialisasiUI();

    // Looping tak terbatas
    while (1) {
        // BARIS INI SANGAT KRUSIAL: Memanggil fungsi pendengar
        bacaInputSerial(); 
        
        // Memastikan Arduino bisa menangkap paket data di background
        if (serialEventRun) {
            serialEventRun(); 
        }
    }

    return 0;
}