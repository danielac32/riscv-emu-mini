
#include <stdint.h>
#include <stdio.h>
#define DRAM_SIZE 125000 // Tamaño de la memoria RAM simulada
#define BLOCK_SIZE 32
#if 1
typedef struct {
    FILE *file;
    uint32_t size;
} Memory;

extern Memory mem;

// Función para obtener el tamaño del archivo
uint32_t get_file_size(FILE *file);
// Función para crear la memoria a partir de un archivo binario
Memory create_memory(const char *filename);
// Función para liberar la memoria
void free_memory(Memory *mem); 
// Funciones de escritura en "dram"
void write_memory_1(const char *dram_filename, uint32_t ofs, uint8_t val); 
void write_memory_2(const char *dram_filename, uint32_t ofs, uint16_t val);
void write_memory_4(const char *dram_filename, uint32_t ofs, uint32_t val);
// Funciones de lectura de "dram"
uint8_t read_memory_1(const char *dram_filename, uint32_t ofs);
uint16_t read_memory_2(const char *dram_filename, uint32_t ofs); 
uint32_t read_memory_4(const char *dram_filename, uint32_t ofs);
#endif


int psram_init(void);
int psram_read(uint32_t addr, void *buf, int len);
int psram_write(uint32_t addr, void *buf, int len);



 
void cache_write(uint32_t ofs, void *buf, uint32_t size);
void cache_read(uint32_t ofs, void *buf, uint32_t size);
void cache_get_stat(uint64_t *phit, uint64_t *paccessed);

 