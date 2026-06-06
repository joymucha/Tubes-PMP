// --- File: inventory.h ---

#ifndef INVENTORY_H
#define INVENTORY_H

// 1. Tempatkan Struktur Data di sini
typedef struct Barang {
    unsigned int id;       // 2 byte 
    char nama[16];         // 16 byte 
    char kategori[12];     // 12 byte 
    int stok;              // 2 byte 
    char lokasi[8];        // 8 byte  
    char status[12];       // 12 byte 
    char pemilik[12];      // 12 byte 
    char pic[12];          // 12 byte 
    struct Barang* next;   // 2 byte  
} Barang;

// 2. Deklarasi Fungsi Void untuk Operasi Linked List (CRUD)
// Perhatikan: Kita tidak menulis isi fungsinya di sini, hanya "judul" fungsinya saja.

void inisialisasiList(Barang** head_pointer);

// Fungsi Tambah Node di akhir (Tail)
void tambahBarang(unsigned int id, char* nama, char* kategori, int stok, char* lokasi, char* status, char* pemilik, char* pic);

// Fungsi Hapus Node (harus bisa handle head, tengah, dan tail)
void hapusBarang(unsigned int target_id);

// Fungsi Cari Node (Menggunakan double pointer untuk mengembalikan hasil pencarian)
void cariBarang(unsigned int target_id, Barang** hasil_pencarian);

// Fungsi Update Stok dan Status
void updateBarang(unsigned int target_id, int stok_baru, char* status_baru);

#endif