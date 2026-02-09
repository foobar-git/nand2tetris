#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

#define MAX_SYMBOLS 1024
#define MAX_LINE_LEN 256

// --- DATA STRUCTURES ---
typedef struct {
    char name[64];
    int address;
} Symbol;

Symbol symbol_table[MAX_SYMBOLS];
int symbol_count = 0;
int next_variable_address = 16; // The "Missing Link" - RAM starts at 16

// --- SYMBOL TABLE FUNCTIONS ---
void add_symbol(char* name, int addr) {
    strcpy(symbol_table[symbol_count].name, name);
    symbol_table[symbol_count].address = addr;
    symbol_count++;
}

int lookup_symbol(char* name) {
    for (int i = 0; i < symbol_count; i++) {
        if (strcmp(symbol_table[i].name, name) == 0) {
            return symbol_table[i].address;
        }
    }
    return -1;
}

// Initialize Hack assebly language predefined symbols (R0-R15, SCREEN, KBD, etc.)
void initialize_symbols() {
    // Add R0 through R15
    for (int i = 0; i < 16; i++) {
        char r_name[5];
        sprintf(r_name, "R%d", i);
        add_symbol(r_name, i);
    }
    add_symbol("SP", 0);
    add_symbol("LCL", 1);
    add_symbol("ARG", 2);
    add_symbol("THIS", 3);
    add_symbol("THAT", 4);
    add_symbol("SCREEN", 16384);
    add_symbol("KBD", 24576);
}

// --- STRING PROCESSING ---
void clean_line(char *raw, char *clean) {
    int j = 0;
    for (int i = 0; raw[i] != '\0'; i++) {
        if (raw[i] == '/' && raw[i+1] == '/') break;
        if (!isspace(raw[i])) {
            clean[j] = raw[i];
            j++;
        }
    }
    clean[j] = '\0';
}

// Helper FN to convert int to 16-bit binary string
void int_to_bin(uint16_t value, char *output) {
    for (int i = 15; i >= 0; i--) {
        output[i] = (value & 1) ? '1' : '0';
        value >>= 1;
    }
    output[16] = '\0';
}

uint16_t get_dest(char *s) {
    uint16_t d = 0;
    if (strchr(s, 'A')) d |= 0x4; // 100
    if (strchr(s, 'D')) d |= 0x2; // 010
    if (strchr(s, 'M')) d |= 0x1; // 001
    return d;
}

uint16_t get_jump(char *s) {
    if (strcmp(s, "JGT") == 0) return 1;
    if (strcmp(s, "JEQ") == 0) return 2;
    if (strcmp(s, "JGE") == 0) return 3;
    if (strcmp(s, "JLT") == 0) return 4;
    if (strcmp(s, "JNE") == 0) return 5;
    if (strcmp(s, "JLE") == 0) return 6;
    if (strcmp(s, "JMP") == 0) return 7;
    return 0;
}

uint16_t get_comp(char *s) {
    // If 'M' is in the string, the 'a' bit (bit 6 of the 7-bit comp) must be 1
    uint16_t a = (strchr(s, 'M') != NULL) ? 0x40 : 0x00;
    
    // Map the logic based on the Hack assembly mnemonic
    if (strcmp(s, "0") == 0)   return a | 0x2A; // 101010
    if (strcmp(s, "1") == 0)   return a | 0x3F; // 111111
    if (strcmp(s, "-1") == 0)  return a | 0x3A; // 111010
    if (strcmp(s, "D") == 0)   return a | 0x0C; // 001100
    if (strcmp(s, "A") == 0 || strcmp(s, "M") == 0)   return a | 0x30; // 110000
    if (strcmp(s, "!D") == 0)  return a | 0x0D; // 001101
    if (strcmp(s, "!A") == 0 || strcmp(s, "!M") == 0)  return a | 0x31; // 110001
    if (strcmp(s, "-D") == 0)  return a | 0x0F; // 001111
    if (strcmp(s, "-A") == 0 || strcmp(s, "-M") == 0)  return a | 0x33; // 110011
    if (strcmp(s, "D+1") == 0) return a | 0x1F; // 011111
    if (strcmp(s, "A+1") == 0 || strcmp(s, "M+1") == 0) return a | 0x37; // 110111
    if (strcmp(s, "D-1") == 0) return a | 0x0E; // 001110
    if (strcmp(s, "A-1") == 0 || strcmp(s, "M-1") == 0) return a | 0x32; // 110010
    if (strcmp(s, "D+A") == 0 || strcmp(s, "D+M") == 0) return a | 0x02; // 000010
    if (strcmp(s, "D-A") == 0 || strcmp(s, "D-M") == 0) return a | 0x13; // 010011
    if (strcmp(s, "A-D") == 0 || strcmp(s, "M-D") == 0) return a | 0x07; // 000111
    if (strcmp(s, "D&A") == 0 || strcmp(s, "D&M") == 0) return a | 0x00; // 000000
    if (strcmp(s, "D|A") == 0 || strcmp(s, "D|M") == 0) return a | 0x15; // 010101
    
    return 0;
}

// --- MAIN ---
int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <filename.asm>\n", argv[0]);
        return 1;
    }

    initialize_symbols();
    
    FILE *file = fopen(argv[1], "r");
    if (!file) {
        printf("Error: Could not open input file %s\n", argv[1]);
        return 1;
    }

    // Create the output filename (replace .asm with .hack)
    char out_name[256];
    strcpy(out_name, argv[1]);
    char *dot = strrchr(out_name, '.');
    if (dot) strcpy(dot, ".hack");
    else strcat(out_name, ".hack");

    FILE *out_file = fopen(out_name, "w");
    if (!out_file) {
        printf("Error: Could not create output file %s\n", out_name);
        return 1;
    }

    char line[MAX_LINE_LEN];
    char cleaned[MAX_LINE_LEN];
    int rom_address = 0;

    // --- PASS 1: SCAN FOR LABELS ---
    while (fgets(line, sizeof(line), file)) {
        clean_line(line, cleaned);
        if (strlen(cleaned) == 0) continue;

        if (cleaned[0] == '(') {
            char label[64];
            int len = strlen(cleaned);
            cleaned[len - 1] = '\0'; 
            strcpy(label, &cleaned[1]);
            if (lookup_symbol(label) == -1) {
                add_symbol(label, rom_address);
            }
        } else {
            rom_address++; 
        }
    }

    // --- PASS 2: TRANSLATION ---
    rewind(file);
    while (fgets(line, sizeof(line), file)) {
        clean_line(line, cleaned);
        if (strlen(cleaned) == 0 || cleaned[0] == '(') continue;

        char bin_out[17];

        if (cleaned[0] == '@') {
            char *value = &cleaned[1];
            int addr;
            if (isdigit(value[0])) {
                addr = atoi(value);
            } else {
                addr = lookup_symbol(value);
                if (addr == -1) {
                    add_symbol(value, next_variable_address);
                    addr = next_variable_address;
                    next_variable_address++;
                }
            }
            int_to_bin((uint16_t)addr, bin_out);
        } 
        else {
            // C-INSTRUCTION SLICING & ASSEMBLY
            char dest_part[10] = "", comp_part[10] = "", jump_part[10] = "";
            char *equal_sign = strchr(cleaned, '=');

            if (equal_sign) {
                strncpy(dest_part, cleaned, equal_sign - cleaned);
                dest_part[equal_sign - cleaned] = '\0';
                strcpy(comp_part, equal_sign + 1); 
            } else {
                strcpy(comp_part, cleaned);
            }

            char *jump_search = strchr(comp_part, ';'); 
            if (jump_search) {
                strcpy(jump_part, jump_search + 1);
                *jump_search = '\0';
            }

            uint16_t binary_instr = 0xE000; // 111 prefix
            binary_instr |= (get_comp(comp_part) << 6);
            binary_instr |= (get_dest(dest_part) << 3);
            binary_instr |= get_jump(jump_part);
            int_to_bin(binary_instr, bin_out);
        }

        fprintf(out_file, "%s\n", bin_out); // Save to the .hack file
    }

    printf("Success! Created %s\n", out_name);
    fclose(file);
    fclose(out_file);
    return 0;
}