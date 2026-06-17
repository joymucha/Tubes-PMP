#ifndef UI_H
#define UI_H

#include "inventory.h"


void inisialisasiUI(void);

void bacaInputSerial(void);

void parseDanEksekusi(char* input_teks);

void tampilkanSemuaBarang(void);

void cetakPesan(int kode_pesan);

#endif