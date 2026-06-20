#include "inventory.h"
#include <stdlib.h> 
#include <string.h>

void inisialisasiList(Barang** head_pointer) {
    *head_pointer = NULL; 
}

void cariBarang(Barang* head, unsigned int target_id, Barang** hasil_pencarian) {
    *hasil_pencarian = NULL; 

    Barang* current = head;

    while (current != NULL) {
        if (current->id == target_id) {
            *hasil_pencarian = current; 
            return; 
        }
        current = current->next;
    }
}

void tambahBarang(Barang** head, Barang** tail, unsigned int id, const char* nama, 
                  const char* kategori, int stok, const char* lokasi, 
                  const char* status, const char* pemilik, const char* pic, int *kode_pesan) {
    
    if (kode_pesan == NULL){
        return;
    }

    if(stok < 0){
        *kode_pesan = PESAN_STOK_TIDAK_VALID;
        return;
    }

    if(status == NULL || status[0] == '\0' || strlen(status) >= 12){
        *kode_pesan = PESAN_STATUS_TIDAK_VALID;
        return;
    }
    if(strcmp(status, "Tersedia") != 0 && strcmp(status, "Dipinjam") != 0 && strcmp(status, "Rusak") != 0 && strcmp(status, "Habis") != 0){
        *kode_pesan = PESAN_STATUS_TIDAK_VALID;
        return;
    }

    if(stok > 0 && strcmp(status, "Habis") == 0){
        *kode_pesan = PESAN_STATUS_TIDAK_VALID;
        return;
    }

    if(stok == 0 && strcmp(status, "Habis") != 0){
        *kode_pesan = PESAN_STATUS_TIDAK_VALID;
        return;
    }

    Barang *barang_duplikat = NULL;
    cariBarang(*head, id, &barang_duplikat);
    if (barang_duplikat != NULL) {
        *kode_pesan = PESAN_ID_DUPLIKAT;
        return;
    }

    Barang* nodeBaru = (Barang*)malloc(sizeof(Barang));
    
    if (nodeBaru == NULL) {
        *kode_pesan = PESAN_ALOKASI_GAGAL;
        return; 
    }

    nodeBaru->id = id;
    nodeBaru->stok = stok;
    
    strncpy(nodeBaru->nama, nama, 15); nodeBaru->nama[15] = '\0';
    strncpy(nodeBaru->kategori, kategori, 11); nodeBaru->kategori[11] = '\0';
    strncpy(nodeBaru->lokasi, lokasi, 7); nodeBaru->lokasi[7] = '\0';
    strncpy(nodeBaru->status, status, 11); nodeBaru->status[11] = '\0';
    strncpy(nodeBaru->pemilik, pemilik, 11); nodeBaru->pemilik[11] = '\0';
    strncpy(nodeBaru->pic, pic, 11); nodeBaru->pic[11] = '\0';
    
    nodeBaru->next = NULL;

    if (*head == NULL) {
        *head = nodeBaru;
        *tail = nodeBaru;
    } else {
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

    if(head == NULL){
        *kode_pesan = PESAN_DATA_KOSONG;
        return;
    }

    if(stok_baru < 0){
        *kode_pesan = PESAN_STOK_TIDAK_VALID;
        return;
    }

    if(status_baru == NULL || status_baru[0] == '\0' || strlen(status_baru) >= 12){
        *kode_pesan = PESAN_STATUS_TIDAK_VALID;
        return;
    }

    if(strcmp(status_baru, "Tersedia") != 0 && strcmp(status_baru, "Dipinjam") != 0 && strcmp(status_baru, "Rusak") != 0 && strcmp(status_baru, "Habis") != 0){
        *kode_pesan = PESAN_STATUS_TIDAK_VALID;
        return;
    }

    if(stok_baru == 0 && strcmp(status_baru, "Habis") != 0){
        *kode_pesan = PESAN_STATUS_TIDAK_VALID;
        return;
    }

    if(stok_baru > 0 && strcmp(status_baru, "Habis") == 0){
        *kode_pesan = PESAN_STATUS_TIDAK_VALID;
        return;
    }

    cariBarang(head, target_id, &target);

    if(target == NULL){
        *kode_pesan = PESAN_ID_TIDAK_DITEMUKAN;
        return;
    }

    target->stok = stok_baru;

    strncpy(target->status, status_baru, sizeof(target->status) - 1);
    target->status[sizeof(target->status) - 1] = '\0';

    *kode_pesan = PESAN_UPDATE_BERHASIL;
}


extern Barang* head_node;
extern Barang* tail_node;

void hapusBarang(unsigned int target_id) {
    if (head_node == NULL) {
        return; 
    }

    Barang* current = head_node;
    Barang* prev = NULL;

    if (current != NULL && current->id == target_id) {
        head_node = current->next; 
        
        if (head_node == NULL) {
            tail_node = NULL; 
        }
        
        free(current);
        return;
    }

    while (current != NULL && current->id != target_id) {
        prev = current;
        current = current->next;
    }

    if (current == NULL) {
        return; 
    }

    prev->next = current->next; 

    if (current->next == NULL) {
        tail_node = prev;
    }

    free(current); 
}

//cek memori 
extern char __heap_start;
extern char *__brkval;

void cekMemoriBebas(int *jumlah_byte){
    char posisis_stack;
    char *akhir_heap;

    if(jumlah_byte == NULL){
        return;
    }

    if(__brkval == NULL){
        akhir_heap = &__heap_start;
    } else {
        akhir_heap = (char *)__brkval;
    }

    *jumlah_byte = (int)(&posisis_stack - akhir_heap);
}