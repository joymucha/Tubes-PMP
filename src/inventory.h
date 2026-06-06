// --- File: inventory.h ---

#ifndef INVENTORY_H
#define INVENTORY_H

typedef struct Barang {
    unsigned int id;       // 2 byte (Kapasitas ID: 0 hingga 65,535)
    char nama[16];         // 16 byte (Maksimal 15 huruf + 1 null terminator)
    char kategori[12];     // 12 byte (Misal: "Sensor", "Mikro", "Kabel")
    int stok;              // 2 byte
    char lokasi[8];        // 8 byte  (Misal: "Rak A-1")
    char status[12];       // 12 byte (Misal: "Tersedia", "Dipinjam")
    char pemilik[12];      // 12 byte (Misal: "Lab Sisben")
    char pic[12];          // 12 byte (Nama Penanggung Jawab)
    struct Barang* next;   // 2 byte  (Pointer memori di arsitektur 16-bit Arduino)
} Barang;


// Total alokasi per node: ~78 Byte.
// Dengan ukuran ini, SRAM 2KB Arduino aman menampung hingga 20-22 barang
// tanpa menyentuh batas kritis memori sistem operasi.


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