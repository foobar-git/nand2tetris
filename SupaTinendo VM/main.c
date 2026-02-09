#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>

/* --- GLOBALS --- */
int label_count = 0;
char clean_vm_name[64];
char current_func_name[128] = "null";

/* --- ASM GENERATORS --- */

// Pushes D-register value to the stack. Optimized to 4 lines.
void push_D(FILE *out) {
    fprintf(out, "@SP\nAM=M+1\nA=A-1\nM=D\n");
}

// Handles the system startup and the shared return logic.
void write_init(FILE *out) {
    // 1. BOOTSTRAP: Points SP to 256 and starts the OS
    fprintf(out, "// --- BOOTSTRAP ---\n@256\nD=A\n@SP\nM=D\n");

    // Manual expansion of 'call Sys.init 0' to avoid label collisions during init
    fprintf(out, "@INIT_RET\nD=A\n@SP\nAM=M+1\nA=A-1\nM=D\n"); // push retAddr
    char *segs[] = {"LCL","ARG","THIS","THAT"};
    for(int i=0; i<4; i++) { fprintf(out, "@%s\nD=M\n@SP\nAM=M+1\nA=A-1\nM=D\n", segs[i]); }
    fprintf(out, "@SP\nD=M\n@5\nD=D-A\n@ARG\nM=D\n@SP\nD=M\n@LCL\nM=D\n@Sys.init\n0;JMP\n(INIT_RET)\n");

    // 2. UNIVERSAL RETURN: One shared block to save thousands of ROM lines
    fprintf(out, "// --- SHARED RETURN ---\n(GLOBAL_RET)\n");
    fprintf(out, "@LCL\nD=M\n@R14\nM=D\n@5\nA=D-A\nD=M\n@R15\nM=D\n");
    fprintf(out, "@SP\nAM=M-1\nD=M\n@ARG\nA=M\nM=D\nD=A+1\n@SP\nM=D\n");
    char *r_segs[] = {"THAT", "THIS", "ARG", "LCL"};
    for(int i=0; i<4; i++) fprintf(out, "@R14\nAM=M-1\nD=M\n@%s\nM=D\n", r_segs[i]);
    fprintf(out, "@R15\nA=M\n0;JMP\n");
}

void write_arithmetic(FILE *out, char *command) {
    if (strcmp(command, "add") == 0) fprintf(out, "@SP\nAM=M-1\nD=M\nA=A-1\nM=D+M\n");
    else if (strcmp(command, "sub") == 0) fprintf(out, "@SP\nAM=M-1\nD=M\nA=A-1\nM=M-D\n");
    else if (strcmp(command, "neg") == 0) fprintf(out, "@SP\nA=M-1\nM=-M\n");
    else if (strcmp(command, "and") == 0) fprintf(out, "@SP\nAM=M-1\nD=M\nA=A-1\nM=D&M\n");
    else if (strcmp(command, "or") == 0)  fprintf(out, "@SP\nAM=M-1\nD=M\nA=A-1\nM=D|M\n");
    else if (strcmp(command, "not") == 0) fprintf(out, "@SP\nA=M-1\nM=!M\n");
    else {
        char *jmp = (strcmp(command, "eq") == 0) ? "JEQ" : (strcmp(command, "gt") == 0 ? "JGT" : "JLT");
        fprintf(out, "@SP\nAM=M-1\nD=M\nA=A-1\nD=M-D\nM=-1\n@L%d\nD;%s\n@SP\nA=M-1\nM=0\n(L%d)\n", label_count, jmp, label_count);
        label_count++;
    }
}

void write_push_pop(FILE *out, char *command, char *segment, int index) {
    char *segPtr = NULL;
    if (strcmp(segment, "local") == 0) segPtr = "LCL";
    else if (strcmp(segment, "argument") == 0) segPtr = "ARG";
    else if (strcmp(segment, "this") == 0) segPtr = "THIS";
    else if (strcmp(segment, "that") == 0) segPtr = "THAT";

    if (strcmp(command, "push") == 0) {
        if (strcmp(segment, "constant") == 0) {
            if (index == 0) fprintf(out, "@SP\nAM=M+1\nA=A-1\nM=0\n");
            else if (index == 1) fprintf(out, "@SP\nAM=M+1\nA=A-1\nM=1\n");
            else { fprintf(out, "@%d\nD=A\n@SP\nAM=M+1\nA=A-1\nM=D\n", index); }
            return;
        }
        if (segPtr) fprintf(out, "@%s\nD=M\n@%d\nA=D+A\nD=M\n", segPtr, index);
        else if (strcmp(segment, "temp") == 0)    fprintf(out, "@%d\nD=M\n", 5 + index);
        else if (strcmp(segment, "pointer") == 0) fprintf(out, "@%d\nD=M\n", 3 + index);
        else if (strcmp(segment, "static") == 0)  fprintf(out, "@%s.%d\nD=M\n", clean_vm_name, index);
        push_D(out);
    } else { // POP
        if (segPtr) {
            if (index == 0) fprintf(out, "@SP\nAM=M-1\nD=M\n@%s\nA=M\nM=D\n", segPtr);
            else fprintf(out, "@%s\nD=M\n@%d\nD=D+A\n@R13\nM=D\n@SP\nAM=M-1\nD=M\n@R13\nA=M\nM=D\n", segPtr, index);
        } else {
            fprintf(out, "@SP\nAM=M-1\nD=M\n");
            if (strcmp(segment, "temp") == 0)         fprintf(out, "@%d\nM=D\n", 5 + index);
            else if (strcmp(segment, "pointer") == 0) fprintf(out, "@%d\nM=D\n", 3 + index);
            else if (strcmp(segment, "static") == 0)  fprintf(out, "@%s.%d\nM=D\n", clean_vm_name, index);
        }
    }
}

void write_call(FILE *out, char *funcName, int nArgs) {
    char ret[16]; sprintf(ret, "z%d", label_count++);
    fprintf(out, "@%s\nD=A\n", ret); push_D(out);
    char *s[] = {"LCL","ARG","THIS","THAT"};
    for(int i=0; i<4; i++) { fprintf(out, "@%s\nD=M\n", s[i]); push_D(out); }
    fprintf(out, "@SP\nD=M\n@%d\nD=D-A\n@ARG\nM=D\n@SP\nD=M\n@LCL\nM=D\n@%s\n0;JMP\n(%s)\n", 5+nArgs, funcName, ret);
}

void write_return(FILE *out) {
    fprintf(out, "@GLOBAL_RET\n0;JMP\n");
}

void write_function(FILE *out, char *funcName, int nVars) {
    strncpy(current_func_name, funcName, 127);
    fprintf(out, "(%s)\n", funcName);
    if (nVars > 2) {
        fprintf(out, "@%d\nD=A\n(%s.L)\n@SP\nAM=M+1\nA=A-1\nM=0\nD=D-1\n@%s.L\nD;JGT\n", nVars, funcName, funcName);
    } else {
        for(int i=0; i<nVars; i++) fprintf(out, "@SP\nAM=M+1\nA=A-1\nM=0\n");
    }
}

void write_program_flow(FILE *out, char *command, char *label) {
    if (strcmp(command, "label") == 0) fprintf(out, "(%s$%s)\n", current_func_name, label);
    else if (strcmp(command, "goto") == 0) fprintf(out, "@%s$%s\n0;JMP\n", current_func_name, label);
    else if (strcmp(command, "if-goto") == 0) fprintf(out, "@SP\nAM=M-1\nD=M\n@%s$%s\nD;JNE\n", current_func_name, label);
}

/* --- MAIN LOGIC --- */

void process_file(const char *filename, FILE *out) {
    FILE *in = fopen(filename, "r");
    if (!in) return;

    // Extract filename for static variables
    const char *base = strrchr(filename, '/');
    base = base ? base + 1 : filename;
    strncpy(clean_vm_name, base, 63);
    char *dot = strchr(clean_vm_name, '.');
    if (dot) *dot = '\0';

    char line[256];
    while (fgets(line, sizeof(line), in)) {
        char *comment = strstr(line, "//");
        if (comment) *comment = '\0';
        char cmd[64], arg1[64]; int arg2;
        int n = sscanf(line, "%s %s %d", cmd, arg1, &arg2);
        if (n <= 0) continue;

        if (n == 3) {
            if (strcmp(cmd, "push")==0 || strcmp(cmd, "pop")==0) write_push_pop(out, cmd, arg1, arg2);
            else if (strcmp(cmd, "function")==0) write_function(out, arg1, arg2);
            else if (strcmp(cmd, "call")==0) write_call(out, arg1, arg2);
        } else if (n == 2) {
            write_program_flow(out, cmd, arg1);
        } else {
            if (strcmp(cmd, "return")==0) write_return(out);
            else write_arithmetic(out, cmd);
        }
    }
    fclose(in);
}

int main(int argc, char *argv[]) {
    if (argc < 2) { printf("Usage: %s <path>\n", argv[0]); return 1; }

    struct stat st;
    stat(argv[1], &st);

    char out_path[256];
    sprintf(out_path, "output.asm"); // Default output
    FILE *out = fopen(out_path, "w");

    write_init(out); // The Nitro Ignition

    if (S_ISDIR(st.st_mode)) {
        DIR *d = opendir(argv[1]);
        struct dirent *dir;
        while ((dir = readdir(d)) != NULL) {
            if (strstr(dir->d_name, ".vm")) {
                char path[512];
                sprintf(path, "%s/%s", argv[1], dir->d_name);
                process_file(path, out);
            }
        }
        closedir(d);
    } else {
        process_file(argv[1], out);
    }

    fclose(out);
    printf("Created %s successfully.\n", out_path);
    return 0;
}
