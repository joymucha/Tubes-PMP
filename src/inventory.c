#include "inventory.h"
#include <stdlib.h> // Library standar C untuk mengenali NULL

// Fungsi bertipe void menggunakan double pointer (Pass-by-Reference)
void inisialisasiList(Barang** head_pointer) {
    // Memastikan pointer antrean menunjuk pada kosong (NULL) saat sistem baru menyala
    *head_pointer = NULL; 
}

// butuh pointer ke head dari linked list biar tau mulai dari mana
void cariBarang(Barang* head, unsigned int target_id, Barang** hasil_pencarian) {
    // jaga-jaga kalo ID ga ketemu
    *hasil_pencarian = NULL; 

    // pointer sementara buat menelusuri list
    Barang* current = head;

    // iterasi selama nodenya belum abis
    while (current != NULL) {
        // kalau ID nya ketemu
        if (current->id == target_id) {
            // kasih alamat memori data yang ketemu ke parameter double pointer
            *hasil_pencarian = current; 
            return; 
        }
        // lanjut ke node lain
        current = current->next;
    }
}

// ku tambahin pointer 'tail' biar pas nambah barang langsung cepet ke tail pake pointer, ga perlu nelusurin list satu-satu sampe akhir
void tambahBarang(Barang** head, Barang** tail, unsigned int id, const char* nama, 
                  const char* kategori, int stok, const char* lokasi, 
                  const char* status, const char* pemilik, const char* pic) {
    
    // alokasi memori dinamis
    Barang* nodeBaru = (Barang*)malloc(sizeof(Barang));
    
    // kalau NULL berarti SRAM arduinonya full
    if (nodeBaru == NULL) {
        return; 
    }

    // pengisian data
    nodeBaru->id = id;
    nodeBaru->stok = stok;
    
    // pakai strcpy buat mencegah kebocoran memori (buffer overflow)
    strncpy(nodeBaru->nama, nama, 15); nodeBaru->nama[15] = '\0';
    strncpy(nodeBaru->kategori, kategori, 11); nodeBaru->kategori[11] = '\0';
    strncpy(nodeBaru->lokasi, lokasi, 7); nodeBaru->lokasi[7] = '\0';
    strncpy(nodeBaru->status, status, 11); nodeBaru->status[11] = '\0';
    strncpy(nodeBaru->pemilik, pemilik, 11); nodeBaru->pemilik[11] = '\0';
    strncpy(nodeBaru->pic, pic, 11); nodeBaru->pic[11] = '\0';
    
    nodeBaru->next = NULL;

    // nyambungin node ke linked list
    if (*head == NULL) {
        // kasus A : kalau inventorynya masih kosong node baru langsung jadi head dan tail
        *head = nodeBaru;
        *tail = nodeBaru;
    } else {
        // kasus B : inventorynya udah ada isi, langsung tambah barang baru ke tail pake pointer
        (*tail)->next = nodeBaru; 
        *tail = nodeBaru;         
    }
}
