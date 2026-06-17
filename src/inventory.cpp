#include "inventory.h"
#include <stdlib.h> // Library standar C untuk mengenali NULL
#include <string.h>

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
                  const char* status, const char* pemilik, const char* pic, int *kode_pesan) {
    
    if (kode_pesan == NULL){
        return;
    }

    //cek stok valid atau tidak
    if(stok < 0){
        *kode_pesan = PESAN_STOK_TIDAK_VALID;
        return;
    }

    //cek status valid atau tidak
    if(status == NULL || status[0] == '\0' || strlen(status) >= 12){
        *kode_pesan = PESAN_STATUS_TIDAK_VALID;
        return;
    }
    if(strcmp(status, "Tersedia") != 0 && strcmp(status, "Dipinjam") != 0 && strcmp(status, "Rusak") != 0 && strcmp(status, "Habis") != 0){
        *kode_pesan = PESAN_STATUS_TIDAK_VALID;
        return;
    }

    //saat stok > 0, status tidak boleh "Habis"
    if(stok > 0 && strcmp(status, "Habis") == 0){
        *kode_pesan = PESAN_STATUS_TIDAK_VALID;
        return;
    }

    //cek kasus khusus saat stok baru 0, status harus "Habis"
    if(stok == 0 && strcmp(status, "Habis") != 0){
        *kode_pesan = PESAN_STATUS_TIDAK_VALID;
        return;
    }

    //menangani ID duplikat
    Barang *barang_duplikat = NULL;
    cariBarang(*head, id, &barang_duplikat);
    if (barang_duplikat != NULL) {
        *kode_pesan = PESAN_ID_DUPLIKAT;
        return;
    }

    // alokasi memori dinamis
    Barang* nodeBaru = (Barang*)malloc(sizeof(Barang));
    
    // kalau NULL berarti SRAM arduinonya full
    if (nodeBaru == NULL) {
        *kode_pesan = PESAN_ALOKASI_GAGAL;
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

    *kode_pesan = PESAN_TAMBAH_BERHASIL;
}


void updateBarang(Barang *head, unsigned int target_id, int stok_baru, char* status_baru, int *kode_pesan){
    
    Barang *target = NULL;

    if(kode_pesan == NULL){
        return;
    }

    //Inventaris belum ada barang sama sekali
    if(head == NULL){
        *kode_pesan = PESAN_DATA_KOSONG;
        return;
    }

    //Stok yang ingin diupdate tidak valid
    if(stok_baru < 0){
        *kode_pesan = PESAN_STOK_TIDAK_VALID;
        return;
    }

    //Status yang ingin diupdate tidak valid
    if(status_baru == NULL || status_baru[0] == '\0' || strlen(status_baru) >= 12){
        *kode_pesan = PESAN_STATUS_TIDAK_VALID;
        return;
    }

    //cek status sesuai dengan status yang valid
    if(strcmp(status_baru, "Tersedia") != 0 && strcmp(status_baru, "Dipinjam") != 0 && strcmp(status_baru, "Rusak") != 0 && strcmp(status_baru, "Habis") != 0){
        *kode_pesan = PESAN_STATUS_TIDAK_VALID;
        return;
    }

    //cek kasus khusus saat stok baru 0, status harus "Habis"
    if(stok_baru == 0 && strcmp(status_baru, "Habis") != 0){
        *kode_pesan = PESAN_STATUS_TIDAK_VALID;
        return;
    }

    //cek saat stok_baru > 0, status tidak boleh "Habis"
    if(stok_baru > 0 && strcmp(status_baru, "Habis") == 0){
        *kode_pesan = PESAN_STATUS_TIDAK_VALID;
        return;
    }

    //cari barang yang mau diupdate
    cariBarang(head, target_id, &target);

    if(target == NULL){
        *kode_pesan = PESAN_ID_TIDAK_DITEMUKAN;
        return;
    }

    //perbarui stok
    target->stok = stok_baru;

    //perbarui status
    strncpy(target->status, status_baru, sizeof(target->status) - 1);
    target->status[sizeof(target->status) - 1] = '\0';

    *kode_pesan = PESAN_UPDATE_BERHASIL;
}