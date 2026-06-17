#include <Arduino.h>
#include "inventory.h"
#include "ui.h"

Barang* head_node = NULL; 
Barang* tail_node = NULL; 

int main(void) {
    init();

    // Inisialisasi awal
    inisialisasiList(&head_node); 
    inisialisasiUI();

    while (1) {
        bacaInputSerial(); 
        
        if (serialEventRun) {
            serialEventRun(); 
        }
    }

    return 0;
}