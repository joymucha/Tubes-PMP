#include "inventory.h"
#include <stdlib.h> // Library standar C untuk mengenali NULL

// Fungsi bertipe void menggunakan double pointer (Pass-by-Reference)
void inisialisasiList(Barang** head_pointer) {
    // Memastikan pointer antrean menunjuk pada kosong (NULL) saat sistem baru menyala
    *head_pointer = NULL; 
}