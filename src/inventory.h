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

enum KodePesan {
    PESAN_UPDATE_BERHASIL = 1,
    PESAN_DATA_KOSONG,
    PESAN_ID_TIDAK_DITEMUKAN,
    PESAN_STOK_TIDAK_VALID,
    PESAN_STATUS_TIDAK_VALID,
    PESAN_FORMAT_SALAH,
    PESAN_ID_DUPLIKAT,
    PESAN_ALOKASI_GAGAL,
    PESAN_TAMBAH_BERHASIL
};

void inisialisasiList(Barang** head_pointer);

void tambahBarang(Barang** head, Barang** tail, unsigned int id, const char* nama, 
                  const char* kategori, int stok, const char* lokasi, 
                  const char* status, const char* pemilik, const char* pic, int *kode_pesan);
  
void hapusBarang(unsigned int target_id);

void cariBarang(Barang* head, unsigned int target_id, Barang** hasil_pencarian);

void updateBarang(Barang *head, unsigned int target_id, int stok_baru, char* status_baru, int *kode_pesan);

#endif