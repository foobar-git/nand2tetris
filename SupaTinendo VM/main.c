#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int label_count = 0;
char clean_vm_name[64]; 

// --- FUNCTION PROTOTYPES ---
void clean_line(char *line);
void write_arithmetic(FILE *out, char *command);
void write_push_pop(FILE *out, char *command, char *segment, int index);

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: ./VMTranslator <path/to/file.vm>\n");
        return 1;
    }

    FILE *in = fopen(argv[1], "r");
    if (!in) { printf("Error: Could not open %s\n", argv[1]); return 1; }

    // Path stripping: "projects/07/StackTest.vm" -> "StackTest"
    char *base = strrchr(argv[1], '/');
    if (!base) base = strrchr(argv[1], '\\');
    if (!base) base = argv[1];
    else base++;

    strcpy(clean_vm_name, base);
    char *dot = strchr(clean_vm_name, '.');
    if (dot) *dot = '\0';

    char out_name[128];
    sprintf(out_name, "%s.asm", clean_vm_name);
    FILE *out = fopen(out_name, "w");

    // Bootstrap
    fprintf(out, "// --- BOOTSTRAP ---\n@256\nD=A\n@SP\nM=D\n\n");

    char line[256];
    while (fgets(line, sizeof(line), in)) {
        char original[256];
        strcpy(original, line);
        clean_line(line);
        if (strlen(line) == 0) continue;

        char cmd[64], arg1[64];
        int arg2;
        int num_parts = sscanf(line, "%s %s %d", cmd, arg1, &arg2);

        fprintf(out, "// VM: %s", original);
        if (num_parts == 1) write_arithmetic(out, cmd);
        else if (num_parts == 3) write_push_pop(out, cmd, arg1, arg2);
        fprintf(out, "\n");
    }

    fprintf(out, "// --- END INFINITE LOOP ---\n(END)\n@END\n0;JMP\n");
    fclose(in); fclose(out);
    printf("Successfully translated %s to %s\n", argv[1], out_name);
    return 0;
}

void clean_line(char *line) {
    char *comment = strstr(line, "//");
    if (comment) *comment = '\0';
    int len = strlen(line);
    while (len > 0 && (line[len-1] == ' ' || line[len-1] == '\n' || line[len-1] == '\r' || line[len-1] == '\t')) {
        line[--len] = '\0';
    }
}

void write_arithmetic(FILE *out, char *command) {
    if (strcmp(command, "add") == 0) fprintf(out, "@SP\nAM=M-1\nD=M\nA=A-1\nM=D+M\n");  // 0: The strings are an exact match
    else if (strcmp(command, "sub") == 0) fprintf(out, "@SP\nAM=M-1\nD=M\nA=A-1\nM=M-D\n");
    else if (strcmp(command, "neg") == 0) fprintf(out, "@SP\nA=M-1\nM=-M\n");
    else if (strcmp(command, "and") == 0) fprintf(out, "@SP\nAM=M-1\nD=M\nA=A-1\nM=D&M\n");
    else if (strcmp(command, "or") == 0)  fprintf(out, "@SP\nAM=M-1\nD=M\nA=A-1\nM=D|M\n");
    else if (strcmp(command, "not") == 0) fprintf(out, "@SP\nA=M-1\nM=!M\n");
    else if (strcmp(command, "eq") == 0 || strcmp(command, "gt") == 0 || strcmp(command, "lt") == 0) {
        char *jmp = "JEQ";
        if (strcmp(command, "gt") == 0) jmp = "JGT";
        if (strcmp(command, "lt") == 0) jmp = "JLT";
        fprintf(out, "@SP\nAM=M-1\nD=M\nA=A-1\nD=M-D\n@TRUE%d\nD;%s\n@SP\nA=M-1\nM=0\n@CONT%d\n0;JMP\n(TRUE%d)\n@SP\nA=M-1\nM=-1\n(CONT%d)\n", label_count, jmp, label_count, label_count, label_count);
        label_count++;
    }
}

void write_push_pop(FILE *out, char *command, char *segment, int index) {
    char *ptr = NULL;
    if (strcmp(segment, "local") == 0) ptr = "LCL";
    else if (strcmp(segment, "argument") == 0) ptr = "ARG";
    else if (strcmp(segment, "this") == 0) ptr = "THIS";
    else if (strcmp(segment, "that") == 0) ptr = "THAT";

    if (strcmp(command, "push") == 0) {
        if (strcmp(segment, "constant") == 0) fprintf(out, "@%d\nD=A\n@SP\nA=M\nM=D\n@SP\nM=M+1\n", index);
        else if (ptr) fprintf(out, "@%s\nD=M\n@%d\nA=D+A\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n", ptr, index);
        else if (strcmp(segment, "temp") == 0) fprintf(out, "@%d\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n", 5 + index);
        else if (strcmp(segment, "pointer") == 0) fprintf(out, "@%d\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n", 3 + index);
        else if (strcmp(segment, "static") == 0) fprintf(out, "@%s.%d\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n", clean_vm_name, index);
    } else if (strcmp(command, "pop") == 0) {
        if (ptr) fprintf(out, "@%s\nD=M\n@%d\nD=D+A\n@R13\nM=D\n@SP\nAM=M-1\nD=M\n@R13\nA=M\nM=D\n", ptr, index);
        else if (strcmp(segment, "temp") == 0) fprintf(out, "@SP\nAM=M-1\nD=M\n@%d\nM=D\n", 5 + index);
        else if (strcmp(segment, "pointer") == 0) fprintf(out, "@SP\nAM=M-1\nD=M\n@%d\nM=D\n", 3 + index);
        else if (strcmp(segment, "static") == 0) fprintf(out, "@SP\nAM=M-1\nD=M\n@%s.%d\nM=D\n", clean_vm_name, index);
    }
}