#include "ui.h"
#include "inventory.h"
#include <Arduino.h>
#include <string.h> 
#include <stdlib.h>

// Deklarasi extern agar file ini bisa melihat variabel global dari main.ino
extern Barang* head_node;
extern Barang* tail_node; 

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
    // Mengambil perintah utama (contoh: "ADD", "FIND")
    char* token_perintah = strtok(input_teks, ",");

    if (token_perintah == NULL) return; 

    if (strcmp(token_perintah, "ADD") == 0) {
        // Lanjutkan memotong sisa parameter satu per satu menggunakan NULL
        char* token_id       = strtok(NULL, ",");
        char* token_nama     = strtok(NULL, ",");
        char* token_kategori = strtok(NULL, ",");
        char* token_stok     = strtok(NULL, ",");
        char* token_lokasi   = strtok(NULL, ",");
        char* token_status   = strtok(NULL, ",");
        char* token_pemilik  = strtok(NULL, ",");
        char* token_pic      = strtok(NULL, ",");

        // Pengecekan keamanan: Pastikan user tidak mengetik format yang kurang
        if (!token_id || !token_nama || !token_kategori || !token_stok || 
            !token_lokasi || !token_status || !token_pemilik || !token_pic) {
            Serial.println(F("-> ERROR: Format ADD tidak lengkap. Gunakan format: ADD,id,nama,kategori,stok,lokasi,status,pemilik,pic"));
            return;
        }

        // Konversi tipe data String ke Integer untuk ID dan Stok
        unsigned int id = (unsigned int)strtoul(token_id, NULL, 10);
        int stok = (int)strtol(token_stok, NULL, 10);
        int kode_pesan;

        // Eksekusi penambahan ke Linked List dengan data dinamis
        tambahBarang(&head_node, &tail_node, id, token_nama, token_kategori, stok, token_lokasi, token_status, token_pemilik, token_pic, &kode_pesan);
        
        cetakPesan(kode_pesan);
    } 
    

    else if (strcmp(token_perintah, "DEL") == 0) {
        Serial.println(F("-> BERHASIL: Perintah DEL diterima (Fungsi Hapus belum aktif)."));
    } 
    
    else if (strcmp(token_perintah, "FIND") == 0) {
        // Ambil ID yang mau dicari
        char* token_id = strtok(NULL, ",");
        
        if (token_id == NULL) {
            Serial.println(F("-> ERROR: Format FIND salah. Gunakan format: FIND,id"));
            return;
        }

        unsigned int id = (unsigned int)strtoul(token_id, NULL, 10);
        Barang* hasil = NULL;
        
        cariBarang(head_node, id, &hasil);
        
        if (hasil != NULL) {
            // Jika ketemu, cetak ID dan Nama barangnya agar lebih interaktif
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
            Serial.println(F("-> ERROR: Format perintah UPD salah. Gunakan format: UPD,id,stok_baru,status_baru"));
            return;
        }

        unsigned int id = (unsigned int)strtoul(token_id, NULL, 10);
        int stok_baru = (int)strtol(token_stok, NULL, 10);
        int kode_pesan;

        updateBarang(head_node, id, stok_baru, token_status, &kode_pesan);
        cetakPesan(kode_pesan);
    } 
    

    else if (strcmp(token_perintah, "PRINT") == 0) {
        Serial.println(F("-> Menjalankan perintah PRINT (Fungsi Tampil belum aktif)."));
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

    Serial.println(F("===================================================================================================="));
    Serial.println(F("ID\t| Nama\t\t| Kategori\t| Stok\t| Lokasi\t| Status\t| Pemilik\t| PIC"));
    Serial.println(F("===================================================================================================="));

    Barang* current = head_node;
    
    while (current != NULL){
        Serial.print(current->id);       Serial.print(F("\t| "));
        Serial.print(current->nama);     Serial.print(F("\t| "));
        Serial.print(current->kategori); Serial.print(F("\t| "));
        Serial.print(current->stok);     Serial.print(F("\t| "));
        Serial.print(current->lokasi);   Serial.print(F("\t| "));
        Serial.print(current->status);   Serial.print(F("\t| "));
        Serial.print(current->pemilik);  Serial.print(F("\t| "));
        Serial.println(current->pic);    
        current = current->next;         
    }
    
    Serial.println(F("===================================================================================================="));
}
