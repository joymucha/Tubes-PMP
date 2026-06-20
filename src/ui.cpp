#include "ui.h"
#include "inventory.h"
#include <Arduino.h>
#include <string.h> 
#include <stdlib.h>
#include <stdio.h> 

extern Barang* head_node;
extern Barang* tail_node; 
extern bool status_sistem; 

void inisialisasiUI() {
    Serial.begin(9600);
    delay(100); 
    Serial.println(F("========================================="));
    Serial.println(F(" SISTEM INVENTARISASI LAB EMBEDDED SYSTEM"));
    Serial.println(F("========================================="));
    Serial.println(F("Status: Memori Kosong (0 Barang)"));
    Serial.println(F("Menunggu input dari Scanner QR...\n"));
}

void bacaInputSerial() {
    if (Serial.available() > 0) {
        String input = Serial.readStringUntil('\n');
        input.trim(); 

        if (input.length() > 0) {
            char buffer[150];
            input.toCharArray(buffer, sizeof(buffer));
            parseDanEksekusi(buffer);
        }
    }
}

void parseDanEksekusi(char* input_teks) {
    char* token_perintah = strtok(input_teks, ",");

    if (token_perintah == NULL) return; 

    if (strcmp(token_perintah, "ADD") == 0) {
        char* token_id       = strtok(NULL, ",");
        char* token_nama     = strtok(NULL, ",");
        char* token_kategori = strtok(NULL, ",");
        char* token_stok     = strtok(NULL, ",");
        char* token_lokasi   = strtok(NULL, ",");
        char* token_status   = strtok(NULL, ",");
        char* token_pemilik  = strtok(NULL, ",");
        char* token_pic      = strtok(NULL, ",");

        if (!token_id || !token_nama || !token_kategori || !token_stok || 
            !token_lokasi || !token_status || !token_pemilik || !token_pic) {
            Serial.println(F("-> ERROR: Format ADD tidak lengkap. Gunakan: ADD,id,nama,kategori,stok,lokasi,status,pemilik,pic"));
            return;
        }

        unsigned int id = (unsigned int)strtoul(token_id, NULL, 10);
        int stok = (int)strtol(token_stok, NULL, 10);
        int kode_pesan;

        tambahBarang(&head_node, &tail_node, id, token_nama, token_kategori, stok, token_lokasi, token_status, token_pemilik, token_pic, &kode_pesan);
        cetakPesan(kode_pesan);

        int memori_bebas = 0;
        cekMemoriBebas(&memori_bebas);
        Serial.print(F("-> SRAM Bebas: "));
        Serial.print(memori_bebas);
        Serial.println(F(" byte"));

        if(kode_pesan == PESAN_TAMBAH_BERHASIL && memori_bebas < CADANGAN_SRAM_AMAN){
            Serial.println(F("-> PERINGATAN: SRAM hampir habis."));
        }
    } 
    
    else if (strcmp(token_perintah, "DEL") == 0) {
            char* token_id = strtok(NULL, ",");
            
            if (token_id == NULL) {
                Serial.println(F("-> ERROR: Format DEL salah. Gunakan format: DEL,id"));
                return;
            }

            unsigned int id = (unsigned int)strtoul(token_id, NULL, 10);
            
            Barang* target = NULL;
            
            cariBarang(head_node, id, &target);
            
            if (target == NULL) {
                cetakPesan(PESAN_ID_TIDAK_DITEMUKAN);
            } else {
                hapusBarang(id);
                Serial.print(F("-> BERHASIL: Perintah hapus untuk ID ["));
                Serial.print(id);
                Serial.println(F("] telah dieksekusi oleh sistem."));
            }
    }

    else if (strcmp(token_perintah, "FIND") == 0) {
        char* token_id = strtok(NULL, ",");
        
        if (token_id == NULL) {
            Serial.println(F("-> ERROR: Format FIND salah. Gunakan format: FIND,id"));
            return;
        }

        unsigned int id = (unsigned int)strtoul(token_id, NULL, 10);
        Barang* hasil = NULL;
        
        cariBarang(head_node, id, &hasil);
        
        if (hasil != NULL) {
            Serial.print(F("-> BERHASIL: Barang Ditemukan! [ID: "));
            Serial.print(hasil->id);
            Serial.print(F("] Nama: "));
            Serial.println(hasil->nama);
        } else {
            cetakPesan(PESAN_ID_TIDAK_DITEMUKAN);
        }
    } 

    else if (strcmp(token_perintah, "UPD") == 0) {
        char* token_id = strtok(NULL, ",");
        char* token_stok = strtok(NULL, ",");
        char* token_status = strtok(NULL, ",");
    
        if (token_id == NULL || token_stok == NULL || token_status == NULL) {
            Serial.println(F("-> ERROR: Format perintah UPD salah. Gunakan: UPD,id,stok_baru,status_baru"));
            return;
        }

        unsigned int id = (unsigned int)strtoul(token_id, NULL, 10);
        int stok_baru = (int)strtol(token_stok, NULL, 10);
        int kode_pesan;

        updateBarang(head_node, id, stok_baru, token_status, &kode_pesan);
        cetakPesan(kode_pesan);
    } 
    
    else if (strcmp(token_perintah, "PRINT") == 0) {
        tampilkanSemuaBarang();
    } 

    else if (strcmp(token_perintah, "EXIT") == 0) {
        Serial.println(F("========================================="));
        Serial.println(F("-> SISTEM KELUAR."));
        Serial.println(F("-> Program selesai dan mengeksekusi return 0."));
        Serial.println(F("========================================="));
        
        Serial.flush();

        status_sistem = false; 
    }

    else {
        Serial.println(F("-> ERROR: Perintah tidak dikenali oleh sistem."));
    }
}

void cetakPesan(int kode_pesan){
    switch (kode_pesan) {
        case PESAN_UPDATE_BERHASIL:
            Serial.println(F("-> BERHASIL: Stok dan status barang berhasil diperbarui!"));
            break;
        case PESAN_DATA_KOSONG:
            Serial.println(F("-> GAGAL: Inventaris kosong."));
            break;
        case PESAN_ID_TIDAK_DITEMUKAN:
            Serial.println(F("-> GAGAL: ID barang tidak ditemukan."));
            break;
        case PESAN_STOK_TIDAK_VALID:
            Serial.println(F("-> GAGAL: Stok baru tidak valid."));
            break;
        case PESAN_STATUS_TIDAK_VALID:
            Serial.println(F("-> GAGAL: Status baru tidak valid."));
            break;
        case PESAN_ID_DUPLIKAT:
            Serial.println(F("-> GAGAL: ID barang sudah digunakan."));
            break;
        case PESAN_ALOKASI_GAGAL:
            Serial.println(F("-> GAGAL: Alokasi memori untuk barang baru gagal."));
            break;
        case PESAN_TAMBAH_BERHASIL:
            Serial.println(F("-> BERHASIL: Barang berhasil ditambahkan ke inventaris."));
            break;
        default:
            Serial.println(F("-> ERROR: Kode pesan tidak dikenali."));
            break;
    }
}


void tampilkanSemuaBarang(void){
    if (head_node == NULL){
        Serial.println(F("-> INFO: Inventaris saat ini kosong."));
        return;
    }

    Serial.println(F("======================================================================================================"));
    Serial.println(F("ID    | Nama            | Kategori    | Stok  | Lokasi | Status      | Pemilik     | PIC"));
    Serial.println(F("======================================================================================================"));

    Barang* current = head_node;
    char buffer[120]; 
    
    while (current != NULL){
        sprintf(buffer, "%-5u | %-15s | %-11s | %-5d | %-6s | %-11s | %-11s | %-11s", 
                current->id, current->nama, current->kategori, current->stok, 
                current->lokasi, current->status, current->pemilik, current->pic);
        
        Serial.println(buffer);
        current = current->next;         
    }
    
    Serial.println(F("======================================================================================================"));
}