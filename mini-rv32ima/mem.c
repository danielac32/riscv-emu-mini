
#include <mem.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
 

#if 1
// Función para obtener el tamaño del archivo
uint32_t get_file_size(FILE *file) {
    fseek(file, 0, SEEK_END); // Moverse al final del archivo
    uint32_t size = ftell(file); // Obtener la posición actual
    rewind(file); // Volver al inicio del archivo
    return size; // Retornar el tamaño
}


#if 0
// Función para crear la memoria a partir de un archivo binario
Memory create_memory(const char *filename) {
    Memory mem;
    mem.file = fopen(filename, "rb"); // Abrir el archivo en modo lectura
    if (mem.file == NULL) {
        fprintf(stderr, "Error: No se pudo abrir el archivo.\n");
        exit(EXIT_FAILURE);
    }

    // Obtener el tamaño del archivo original
    mem.size = get_file_size(mem.file);

    // Crear o sobrescribir el archivo "dram"
    const char *dram_filename = "dram";
    remove(dram_filename); // Eliminar "dram" si existe
    FILE *dram_file = fopen(dram_filename, "wb");
    if (dram_file == NULL) {
        fprintf(stderr, "Error: No se pudo crear el archivo 'dram'.\n");
        fclose(mem.file);
        exit(EXIT_FAILURE);
    }

    // Copiar el contenido del archivo original a "dram"
    //uint8_t *buffer = (uint8_t *)malloc(mem.size);
    //fread(buffer, sizeof(uint8_t), mem.size, mem.file);
    //fwrite(buffer, sizeof(uint8_t), mem.size, dram_file);
    //free(buffer);
    
    uint8_t buffer[BLOCK_SIZE]; // Buffer para copiar datos
    size_t bytes_read;
    while ((bytes_read = fread(buffer, sizeof(uint8_t), BLOCK_SIZE, mem.file)) > 0) {
        fwrite(buffer, sizeof(uint8_t), bytes_read, dram_file);
    }

    // Asegurarse de que "dram" tenga un tamaño fijo de 125000 bytes
    /*const uint32_t target_size = DRAM_SIZE;
    if (mem.size < target_size) {
        uint8_t *zero_data = (uint8_t *)calloc(target_size - mem.size, sizeof(uint8_t));
        fwrite(zero_data, sizeof(uint8_t), target_size - mem.size, dram_file);
        free(zero_data);
    }*/

    fclose(dram_file); // Cerrar el archivo "dram"
    rewind(mem.file);  // Volver al inicio del archivo original
    return mem;
}

#else
Memory create_memory(const char *filename) {
    Memory mem;

   

    mem.file = fopen(filename, "rb"); // Abrir el archivo en modo lectura
    if (mem.file == NULL) {
        printf("Error: No se pudo abrir el archivo.\n");
        while(1);
    }

    // Obtener el tamaño del archivo original
    mem.size = get_file_size(mem.file);
    mem.p = malloc(90000);
    
    uint8_t buffer[BLOCK_SIZE]; // Buffer para copiar datos
    size_t bytes_read;
    fread(mem.p, 1, mem.size,mem.file);
 
    fclose(mem.file);
    //hexDump2(0,mem.p,mem.size);
    printf("END\n");
    return mem;
}

#endif


// Función para liberar la memoria
void free_memory(Memory *mem) {
    fclose(mem->file);
    mem->file = NULL;
    mem->size = 0;
}

// Funciones de escritura en "dram"
void write_memory_1(const char *dram_filename, uint32_t ofs, uint8_t val) {
    FILE *dram_file = fopen(dram_filename, "r+b"); // Abrir en modo lectura y escritura
    if (dram_file == NULL) {
        fprintf(stderr, "Error: No se pudo abrir el archivo 'dram'.\n");
        return;
    }
    fseek(dram_file, ofs, SEEK_SET);
    fwrite(&val, sizeof(uint8_t), 1, dram_file);
    fclose(dram_file);
}

void write_memory_2(const char *dram_filename, uint32_t ofs, uint16_t val) {
    FILE *dram_file = fopen(dram_filename, "r+b"); // Abrir en modo lectura y escritura
    if (dram_file == NULL) {
        fprintf(stderr, "Error: No se pudo abrir el archivo 'dram'.\n");
        return;
    }
    fseek(dram_file, ofs, SEEK_SET);
    fwrite(&val, sizeof(uint16_t), 1, dram_file);
    fclose(dram_file);
}

void write_memory_4(const char *dram_filename, uint32_t ofs, uint32_t val) {
    FILE *dram_file = fopen(dram_filename, "r+b"); // Abrir en modo lectura y escritura
    if (dram_file == NULL) {
        fprintf(stderr, "Error: No se pudo abrir el archivo 'dram'.\n");
        return;
    }
    fseek(dram_file, ofs, SEEK_SET);
    fwrite(&val, sizeof(uint32_t), 1, dram_file);
    fclose(dram_file);
}

// Funciones de lectura de "dram"
uint8_t read_memory_1(const char *dram_filename, uint32_t ofs) {
    uint8_t val = 0;
    FILE *dram_file = fopen(dram_filename, "rb"); // Abrir en modo lectura
    if (dram_file == NULL) {
        fprintf(stderr, "Error: No se pudo abrir el archivo 'dram'.\n");
        return 0;
    }
    fseek(dram_file, ofs, SEEK_SET);
    fread(&val, sizeof(uint8_t), 1, dram_file);
    fclose(dram_file);
    return val;
}

uint16_t read_memory_2(const char *dram_filename, uint32_t ofs) {
    uint16_t val = 0;
    FILE *dram_file = fopen(dram_filename, "rb"); // Abrir en modo lectura
    if (dram_file == NULL) {
        fprintf(stderr, "Error: No se pudo abrir el archivo 'dram'.\n");
        return 0;
    }
    fseek(dram_file, ofs, SEEK_SET);
    fread(&val, sizeof(uint16_t), 1, dram_file);
    fclose(dram_file);
    return val;
}

uint32_t read_memory_4(const char *dram_filename, uint32_t ofs) {
    uint32_t val = 0;
    FILE *dram_file = fopen(dram_filename, "rb"); // Abrir en modo lectura
    if (dram_file == NULL) {
        fprintf(stderr, "Error: No se pudo abrir el archivo 'dram'.\n");
        return 0;
    }
    fseek(dram_file, ofs, SEEK_SET);
    fread(&val, sizeof(uint32_t), 1, dram_file);
    fclose(dram_file);
    return val;
}
 #endif


int psram_init(void){
    return 0;
}
int psram_read(uint32_t addr, void *buf, int len){
    FILE *dram_file = fopen("dram", "rb");
    fseek(dram_file, addr, SEEK_SET);
    fread(buf, 1, len, dram_file);
    fclose(dram_file);
    return 0;
}
int psram_write(uint32_t addr, void *buf, int len){
    FILE *dram_file = fopen("dram", "r+b"); // Abrir en modo lectura y escritura
    if (dram_file == NULL) {
        fprintf(stderr, "Error: No se pudo abrir el archivo 'dram'.\n");
        return -1;
    }
    fseek(dram_file, addr, SEEK_SET); // Mover el puntero al offset deseado
    fwrite(buf, 1, len, dram_file);   
    fclose(dram_file);
    return 0;
}
 
#define CACHESIZE   4096
struct cacheline {
    uint8_t data[64];
};

static uint64_t accessed, hit;
static uint32_t tags[CACHESIZE/64/2][2];
static struct cacheline cachelines[CACHESIZE/64/2][2];

/*
 * bit[0]: valid
 * bit[1]: dirty
 * bit[2]: for LRU
 * bit[3:10]: reserved
 * bit[11:31]: tag
 */
#define VALID       (1 << 0)
#define DIRTY       (1 << 1)
#define LRU     (1 << 2)
#define LRU_SFT     2
#define TAG_MSK     0xfffff800

/*
 * bit[0: 5]: offset
 * bit[6: 10]: index
 * bit[11: 31]: tag
 */
static inline int get_index(uint32_t addr)
{
    return (addr >> 6) & 0x1f;
}

void cache_write(uint32_t ofs, void *buf, uint32_t size)
{
    if (((ofs | (64 - 1)) != ((ofs + size - 1) | (64 - 1))))
        printf("write cross boundary, ofs:%x size:%x\n", ofs, size);

    int ti, i, index = get_index(ofs);
    uint32_t *tp;
    uint8_t *p;

    ++accessed;

    for (i = 0; i < 2; i++) {
        tp = &tags[index][i];
        p = cachelines[index][i].data;
        if (*tp & VALID) {
            if ((*tp & TAG_MSK) == (ofs & TAG_MSK)) {
                ++hit;
                ti = i;
                break;
            } else {
                if (i != 1)
                    continue;

                ti = 1 - ((*tp & LRU) >> LRU_SFT);
                tp = &tags[index][ti];
                p = cachelines[index][ti].data;

                if (*tp & DIRTY) {
                    psram_write(*tp & ~0x3f, p, 64);
                }
                psram_read(ofs & ~0x3f, p, 64);
                *tp = ofs & ~0x3f;
                *tp |= VALID;
            }
        } else {
            if (i != 1)
                continue;

            ti = i;
            psram_read(ofs & ~0x3f, p, 64);
            *tp = ofs & ~0x3f;
            *tp |= VALID;
        }
    }

    tags[index][1] &= ~(LRU);
    tags[index][1] |= (ti << LRU_SFT);
    memcpy(p + (ofs & 0x3f), buf, size);
    *tp |= DIRTY;
}

void cache_read(uint32_t ofs, void *buf, uint32_t size)
{
    if (((ofs | (64 - 1)) != ((ofs + size - 1) | (64 - 1))))
        printf("read cross boundary, ofs:%x size:%x\n", ofs, size);

    int ti, i, index = get_index(ofs);
    uint32_t *tp;
    uint8_t *p;

    ++accessed;

    for (i = 0; i < 2; i++) {
        tp = &tags[index][i];
        p = cachelines[index][i].data;
        if (*tp & VALID) {
            if ((*tp & TAG_MSK) == (ofs & TAG_MSK)) {
                ++hit;
                ti = i;
                break;
            } else {
                if (i != 1)
                    continue;

                ti = 1 - ((*tp & LRU) >> LRU_SFT);
                tp = &tags[index][ti];
                p = cachelines[index][ti].data;

                if (*tp & DIRTY) {
                    psram_write(*tp & ~0x3f, p, 64);
                }
                psram_read(ofs & ~0x3f, p, 64);
                *tp = ofs & ~0x3f;
                *tp |= VALID;
            }
        } else {
            if (i != 1)
                continue;

            ti = i;
            psram_read(ofs & ~0x3f, p, 64);
            *tp = ofs & ~0x3f;
            *tp |= VALID;
        }
    }

    tags[index][1] &= ~(LRU);
    tags[index][1] |= (ti << LRU_SFT);
    memcpy(buf, p + (ofs & 0x3f), size);
}

void cache_get_stat(uint64_t *phit, uint64_t *paccessed)
{
    *phit = hit;
    *paccessed = accessed;
}