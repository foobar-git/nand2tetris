#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>

/* --- CROSS-PLATFORM HEADERS --- */
#ifdef _WIN32
    #include <direct.h>
    #define GetCurrentDir _getcwd
    #define PATH_SEP '\\'
#else
    #include <unistd.h>
    #define GetCurrentDir getcwd
    #define PATH_SEP '/'
#endif

// Global counter for unique jump labels and return addresses
int label_count = 0;
// Buffer for the current VM filename (used for static variables and branching)
char clean_vm_name[64]; 

// --- FUNCTION PROTOTYPES ---
void clean_line(char *line);
void write_arithmetic(FILE *out, char *command);
void write_push_pop(FILE *out, char *command, char *segment, int index);
void write_program_flow(FILE *out, char *command, char *label);
void write_function(FILE *out, char *funcName, int nVars);
void write_call(FILE *out, char *funcName, int nArgs);
void write_return(FILE *out);
int is_vm_file(const char *name);
void process_file(const char *filename, FILE *out);

// --- MAIN ---
int main(int argc, char *argv[]) {
    // Modified to accept an optional 2nd argument for the output name
    if (argc < 2) {
        printf("Usage: ./VMTranslator <path/to/file_or_folder> [optional_output_name]\n");
        return 1;
    }

    // 1. PATH
    char path[256];
    strncpy(path, argv[1], 255);

    // Strip trailing slashes
    int path_len = strlen(path);
    while (path_len > 0 && (path[path_len - 1] == '/' || path[path_len - 1] == '\\')) {
        path[--path_len] = '\0';
    }

    // 2. DETERMINE OUTPUT FILENAME
    char clean_base[128] = "";
    struct stat path_stat;
    
    if (stat(path, &path_stat) != 0) {
        printf("Error: Path %s does not exist\n", path);
        return 1;
    }

    // PRIORITY 1: Use the name provided by the batch script (argv[2])
    if (argc >= 3) {
        strncpy(clean_base, argv[2], 127);
    } 
    // PRIORITY 2: If no name provided and it's a directory/dot, use folder name
    else if ((path_stat.st_mode & S_IFMT) == S_IFDIR || strcmp(path, ".") == 0) {
        char cwd[1024];
        if (GetCurrentDir(cwd, sizeof(cwd)) != NULL) {
            char *last_slash = strrchr(cwd, PATH_SEP);
            if (last_slash) strcpy(clean_base, last_slash + 1);
            else strcpy(clean_base, "Output");
        }
    } 
    // PRIORITY 3: Use the filename itself
    else {
        const char *final_segment = path;
        char *last_slash = strpbrk(path, "/\\");
        if (last_slash) final_segment = last_slash + 1;
        strcpy(clean_base, final_segment);
        char *dot_ptr = strstr(clean_base, ".vm");
        if (dot_ptr) *dot_ptr = '\0';
    }

    char out_name[256];
    sprintf(out_name, "%s.asm", clean_base);

    // 3. OPEN OUTPUT FILE
    FILE *out = fopen(out_name, "w");
    if (!out) {
        printf("Error: Could not create output file %s\n", out_name);
        return 1;
    }

    // 4. BOOTSTRAP CODE
    fprintf(out, "// --- BOOTSTRAP ---\n@256\nD=A\n@SP\nM=D\n");
    write_call(out, "Sys.init", 0); 

    // 5. PROCESSING
    if ((path_stat.st_mode & S_IFMT) == S_IFDIR || strcmp(path, ".") == 0) {
        DIR *d = opendir(path);
        struct dirent *dir_entry;
        if (d) {
            while ((dir_entry = readdir(d)) != NULL) {
                if (is_vm_file(dir_entry->d_name)) {
                    char full_path[512];
                    sprintf(full_path, "%s/%s", path, dir_entry->d_name);
                    printf("Processing: %s\n", full_path);
                    process_file(full_path, out);
                }
            }
            closedir(d);
        }
    } else {
        process_file(path, out);
    }

    // 6. FINALIZE
    fprintf(out, "\n// --- END INFINITE LOOP ---\n(END)\n@END\n0;JMP\n");
    fclose(out);

    printf("Successfully generated: %s\n", out_name);
    return 0;
}

// --- LOGIC FOR TRANSLATING A SINGLE FILE ---
void process_file(const char *filename, FILE *out) {
    FILE *in = fopen(filename, "r");
    if (!in) {
        printf("Error: Could not open %s\n", filename);
        return;
    }

    const char *base = filename;
    for (int i = 0; filename[i] != '\0'; i++) {
        if (filename[i] == '/' || filename[i] == '\\') base = &filename[i+1];
    }
    strncpy(clean_vm_name, base, 63);
    char *dot = strchr(clean_vm_name, '.');
    if (dot) *dot = '\0';

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
        
        if (num_parts == 3) {
            if (strcmp(cmd, "push") == 0 || strcmp(cmd, "pop") == 0) write_push_pop(out, cmd, arg1, arg2);
            else if (strcmp(cmd, "function") == 0) write_function(out, arg1, arg2);
            else if (strcmp(cmd, "call") == 0) write_call(out, arg1, arg2);
        } else if (num_parts == 2) {
            write_program_flow(out, cmd, arg1);
        } else if (num_parts == 1) {
            if (strcmp(cmd, "return") == 0) write_return(out);
            else write_arithmetic(out, cmd);
        }
        fprintf(out, "\n");
    }
    fclose(in);
}

// --- HELPER FUNCTIONS ---
int is_vm_file(const char *name) {
    const char *dot = strrchr(name, '.');
    return (dot && strcmp(dot, ".vm") == 0);
}

void clean_line(char *line) {
    char *comment = strstr(line, "//");
    if (comment) *comment = '\0';
    int len = strlen(line);
    while (len > 0 && (line[len-1] == ' ' || line[len-1] == '\n' || line[len-1] == '\r' || line[len-1] == '\t')) line[--len] = '\0';
}

void write_function(FILE *out, char *funcName, int nVars) {
    fprintf(out, "(%s)\n", funcName);
    for (int i = 0; i < nVars; i++) {
        fprintf(out, "@0\nD=A\n@SP\nA=M\nM=D\n@SP\nM=M+1\n");
    }
}

void write_call(FILE *out, char *funcName, int nArgs) {
    char retLabel[128];
    sprintf(retLabel, "RET_ADDR_%d", label_count++);
    fprintf(out, "@%s\nD=A\n@SP\nA=M\nM=D\n@SP\nM=M+1\n", retLabel);
    char *segments[] = {"LCL", "ARG", "THIS", "THAT"};
    for (int i = 0; i < 4; i++) {
        fprintf(out, "@%s\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n", segments[i]);
    }
    fprintf(out, "@SP\nD=M\n@5\nD=D-A\n@%d\nD=D-A\n@ARG\nM=D\n", nArgs);
    fprintf(out, "@SP\nD=M\n@LCL\nM=D\n");
    fprintf(out, "@%s\n0;JMP\n", funcName);
    fprintf(out, "(%s)\n", retLabel);
}

void write_return(FILE *out) {
    fprintf(out, "@LCL\nD=M\n@R14\nM=D\n"); // FRAME = LCL
    fprintf(out, "@5\nA=D-A\nD=M\n@R15\nM=D\n"); // RET = *(FRAME-5)
    fprintf(out, "@SP\nAM=M-1\nD=M\n@ARG\nA=M\nM=D\n"); // *ARG = pop()
    fprintf(out, "@ARG\nD=M+1\n@SP\nM=D\n"); // SP = ARG + 1
    char *segments[] = {"THAT", "THIS", "ARG", "LCL"};
    for (int i = 0; i < 4; i++) {
        fprintf(out, "@R14\nAM=M-1\nD=M\n@%s\nM=D\n", segments[i]);
    }
    fprintf(out, "@R15\nA=M\n0;JMP\n");
}

void write_program_flow(FILE *out, char *command, char *label) {
    if (strcmp(command, "label") == 0) fprintf(out, "(%s$%s)\n", clean_vm_name, label);
    else if (strcmp(command, "goto") == 0) fprintf(out, "@%s$%s\n0;JMP\n", clean_vm_name, label);
    else if (strcmp(command, "if-goto") == 0) fprintf(out, "@SP\nAM=M-1\nD=M\n@%s$%s\nD;JNE\n", clean_vm_name, label);
}

void write_arithmetic(FILE *out, char *command) {
    if (strcmp(command, "add") == 0) fprintf(out, "@SP\nAM=M-1\nD=M\nA=A-1\nM=D+M\n");
    else if (strcmp(command, "sub") == 0) fprintf(out, "@SP\nAM=M-1\nD=M\nA=A-1\nM=M-D\n");
    else if (strcmp(command, "neg") == 0) fprintf(out, "@SP\nA=M-1\nM=-M\n");
    else if (strcmp(command, "and") == 0) fprintf(out, "@SP\nAM=M-1\nD=M\nA=A-1\nM=D&M\n");
    else if (strcmp(command, "or") == 0)  fprintf(out, "@SP\nAM=M-1\nD=M\nA=A-1\nM=D|M\n");
    else if (strcmp(command, "not") == 0) fprintf(out, "@SP\nA=M-1\nM=!M\n");
    else if (strcmp(command, "eq") == 0 || strcmp(command, "gt") == 0 || strcmp(command, "lt") == 0) {
        char *jmp = (strcmp(command, "eq") == 0) ? "JEQ" : (strcmp(command, "gt") == 0 ? "JGT" : "JLT");
        fprintf(out, "@SP\nAM=M-1\nD=M\nA=A-1\nD=M-D\n@TRUE%d\nD;%s\n@SP\nA=M-1\nM=0\n@CONT%d\n0;JMP\n(TRUE%d)\n@SP\nA=M-1\nM=-1\n(CONT%d)\n", 
                label_count, jmp, label_count, label_count, label_count);
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
    } else {
        if (ptr) fprintf(out, "@%s\nD=M\n@%d\nD=D+A\n@R13\nM=D\n@SP\nAM=M-1\nD=M\n@R13\nA=M\nM=D\n", ptr, index);
        else if (strcmp(segment, "temp") == 0) fprintf(out, "@SP\nAM=M-1\nD=M\n@%d\nM=D\n", 5 + index);
        else if (strcmp(segment, "pointer") == 0) fprintf(out, "@SP\nAM=M-1\nD=M\n@%d\nM=D\n", 3 + index);
        else if (strcmp(segment, "static") == 0) fprintf(out, "@SP\nAM=M-1\nD=M\n@%s.%d\nM=D\n", clean_vm_name, index);
    }
}