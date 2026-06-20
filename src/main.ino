#include <Arduino.h>
#include "inventory.h"
#include "ui.h"

Barang* head_node = NULL; 
Barang* tail_node = NULL; 

bool status_sistem = true; 

int main(void) {
    init();

    // Inisialisasi awal
    inisialisasiList(&head_node); 
    inisialisasiUI();

    while (status_sistem) {
        bacaInputSerial(); 
        
        if (serialEventRun) {
            serialEventRun(); 
        }
    }
    return 0;
}