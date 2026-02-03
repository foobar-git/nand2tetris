#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * ⚡ SUPATINENDO: UNIVERSAL NITRO EDITION ⚡
 * Cross-platform Hack CPU Emulator (Win32 GDI / Linux SDL2)
 */

// --- Hardware Memory Map ---
#define RAM_SIZE 32768      // 32K words of 16-bit RAM
#define ROM_SIZE 32768      // 32K words of 16-bit ROM
#define SCREEN_START 16384  // Memory-mapped screen start (ends at 24575)
#define KBD_ADDR 24576      // Memory-mapped keyboard address
#define SCREEN_WIDTH 512    // Standard Hack Resolution
#define SCREEN_HEIGHT 256

// --- Global Hardware State ---
uint16_t RAM[RAM_SIZE] = {0};
uint16_t ROM[ROM_SIZE] = {0};
uint16_t A = 0, D = 0, PC = 0; // The Holy Trinity of CPU Registers
int mem_dump_enabled = 0;      // Set via command line '-md'

// 32-bit pixel buffer for the "Nitro" blit (Windows/Linux both use 0xRRGGBB)
uint32_t PixelBuffer[SCREEN_WIDTH * SCREEN_HEIGHT];

// --- Platform Detection & Headers ---
#ifdef _WIN32
    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>
    const char* PLATFORM_NAME = "Windows (GDI Native)";
#else
    #include <SDL2/SDL.h>
    const char* PLATFORM_NAME = "Linux (SDL2 Accelerated)";
#endif

// ==========================================================
// CORE NITRO ENGINE (CPU & ALU)
// ==========================================================

/**
 * ALU: Arithmetic Logic Unit
 * Pure math logic. No platform-specific code here.
 */
uint16_t compute(uint16_t c, uint16_t a) {
    uint16_t x = D;
    uint16_t y = a ? RAM[A] : A; // The 'a' bit chooses between A and M
    switch (c) {
        case 0x2A: return 0;   case 0x3F: return 1;   case 0x3A: return -1;
        case 0x0C: return x;   case 0x30: return y;   case 0x0D: return !x;
        case 0x31: return !y;  case 0x0F: return -x;  case 0x33: return -y;
        case 0x1F: return x+1; case 0x37: return y+1; case 0x0E: return x-1;
        case 0x32: return y-1; case 0x02: return x+y; case 0x13: return x-y;
        case 0x07: return y-x; case 0x00: return x&y; case 0x15: return x|y;
        default: return 0;
    }
}

/**
 * CPU Fetch-Decode-Execute Cycle
 */
void execute_cycle() {
    uint16_t inst = ROM[PC];
    if (!(inst & 0x8000)) { // A-Instruction: @value
        A = inst; 
        PC++; 
    }
    else { // C-Instruction: dest = comp ; jump
        uint16_t out = compute((inst >> 6) & 0x3F, (inst >> 12) & 1);
        uint16_t dest = (inst >> 3) & 7;
        uint16_t jmp = inst & 7;

        if (dest & 1) RAM[A] = out; // Store to M
        if (dest & 2) D = out;      // Store to D
        if (dest & 4) A = out;      // Store to A

        // Jump calculation using signed comparison
        int jump = 0; int16_t s = (int16_t)out;
        if ((jmp==1 && s>0) || (jmp==2 && s==0) || (jmp==3 && s>=0) || 
            (jmp==4 && s<0) || (jmp==5 && s!=0) || (jmp==6 && s<=0) || (jmp==7)) jump = 1;
        
        PC = jump ? A : PC + 1;
    }
}

/**
 * Screen Translation
 * Converts the 16-bit packed RAM bits into a 32-bit linear PixelBuffer.
 */
void update_pixels() {
    for (int y = 0; y < 256; y++) {
        for (int x_word = 0; x_word < 32; x_word++) {
            uint16_t word = RAM[SCREEN_START + (y * 32) + x_word];
            for (int b = 0; b < 16; b++) {
                // 1 = Black, 0 = White
                PixelBuffer[y * 512 + (x_word * 16 + b)] = (word & (1 << b)) ? 0x00000000 : 0x00FFFFFF;
            }
        }
    }
}

/**
 * Memory Dump
 * Saves final state to disk for debugging
 */
void dump_memory() {
    printf("\nSaving state to ST_memory_dump.txt...\n");
    FILE *f = fopen("ST_memory_dump.txt", "w");
    if (!f) return;
    fprintf(f, "CPU STATE: PC=%d, A=%d, D=%d\n\n", PC, A, D);
    for (int i = 0; i < RAM_SIZE; i++) {
        fprintf(f, "RAM[%5d]: %d (0x%04X)\n", i, (int16_t)RAM[i], RAM[i]);
    }
    fclose(f);
}

// ==========================================================
// WINDOWS PLATFORM (WIN32 GDI)
// ==========================================================
#ifdef _WIN32
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_KEYDOWN: // Map Windows VK codes to Hack Keyboard Map
            if (wParam == VK_LEFT) RAM[KBD_ADDR] = 130;
            else if (wParam == VK_UP) RAM[KBD_ADDR] = 131;
            else if (wParam == VK_RIGHT) RAM[KBD_ADDR] = 132;
            else if (wParam == VK_DOWN) RAM[KBD_ADDR] = 133;
            else if (wParam == VK_SPACE) RAM[KBD_ADDR] = 32;
            else if (wParam < 128) RAM[KBD_ADDR] = (uint16_t)wParam;
            return 0;
        case WM_KEYUP: RAM[KBD_ADDR] = 0; return 0;
        case WM_PAINT: {
            PAINTSTRUCT ps; HDC hdc = BeginPaint(hwnd, &ps);
            update_pixels();
            BITMAPINFO bmi = {0};
            bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
            bmi.bmiHeader.biWidth = 512; bmi.bmiHeader.biHeight = -256; // Top-down
            bmi.bmiHeader.biPlanes = 1; bmi.bmiHeader.biBitCount = 32;
            // The Nitro Fart Blit: slamming pixels to screen
            SetDIBitsToDevice(hdc, 0, 0, 512, 256, 0, 0, 0, 256, PixelBuffer, &bmi, DIB_RGB_COLORS);
            EndPaint(hwnd, &ps); return 0;
        }
        case WM_DESTROY: PostQuitMessage(0); return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
#endif

// ==========================================================
// MAIN ENTRY POINT (Logic shared by both platforms)
// ==========================================================
int main(int argc, char *argv[]) {
    if (argc < 2) { printf("Usage: %s <file.hack> [-md]\n", argv[0]); return 1; }
    for (int i = 1; i < argc; i++) if (strcmp(argv[i], "-md") == 0) mem_dump_enabled = 1;

    // Load ROM from disk
    FILE *f = fopen(argv[1], "r");
    if (!f) { printf("Error: Could not find ROM file.\n"); return 1; }
    char line[32]; int r_idx = 0;
    while (fgets(line, sizeof(line), f) && r_idx < ROM_SIZE) {
        if (strlen(line) >= 16) ROM[r_idx++] = (uint16_t)strtol(line, NULL, 2);
    }
    fclose(f);

    printf("⚡ SupaTinendo Booting on %s...\n", PLATFORM_NAME);

#ifdef _WIN32
    // --- Windows Loop ---
    HINSTANCE hi = GetModuleHandle(NULL);
    WNDCLASS wc = {0}; wc.lpfnWndProc = WindowProc; wc.hInstance = hi;
    wc.lpszClassName = "SupaTinendo"; wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    RegisterClass(&wc);
    HWND hwnd = CreateWindowEx(0, "SupaTinendo", "SupaTinendo Emulator", WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                               CW_USEDEFAULT, CW_USEDEFAULT, 528, 295, NULL, NULL, hi, NULL);
    MSG msg = {0}; uint32_t cyc = 0;
    while (msg.message != WM_QUIT) {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) { TranslateMessage(&msg); DispatchMessage(&msg); }
        execute_cycle();
        if (++cyc % 10000 == 0) InvalidateRect(hwnd, NULL, FALSE);
        if (ROM[PC] == (0x8000 | (0x2A << 6) | 7) && A == PC) break; // Infinite Loop Detection
    }
    if (mem_dump_enabled) dump_memory();
#else
    // --- Linux SDL2 Loop ---
    if (SDL_Init(SDL_INIT_VIDEO) < 0) return 1;
    SDL_Window* win = SDL_CreateWindow("SupaTinendo Nitro", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 512, 256, 0);
    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
    SDL_Texture* tex = SDL_CreateTexture(ren, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, 512, 256);
    int run = 1; uint32_t cyc = 0; SDL_Event e;
    while (run) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) run = 0;
            if (e.type == SDL_KEYDOWN || e.type == SDL_KEYUP) {
                uint16_t v = (e.type == SDL_KEYDOWN) ? 1 : 0;
                switch (e.key.keysym.sym) {
                    case SDLK_LEFT: RAM[KBD_ADDR] = v ? 130 : 0; break;
                    case SDLK_UP:   RAM[KBD_ADDR] = v ? 131 : 0; break;
                    case SDLK_RIGHT:RAM[KBD_ADDR] = v ? 132 : 0; break;
                    case SDLK_DOWN: RAM[KBD_ADDR] = v ? 133 : 0; break;
                    case SDLK_SPACE:RAM[KBD_ADDR] = v ? 32 : 0;  break;
                    default: if (e.key.keysym.sym < 128) RAM[KBD_ADDR] = v ? e.key.keysym.sym : 0;
                }
            }
        }
        execute_cycle();
        if (++cyc % 10000 == 0) {
            update_pixels();
            SDL_UpdateTexture(tex, NULL, PixelBuffer, 512 * 4);
            SDL_RenderCopy(ren, tex, NULL, NULL); SDL_RenderPresent(ren);
        }
        if (ROM[PC] == (0x8000 | (0x2A << 6) | 7) && A == PC) run = 0;
    }
    if (mem_dump_enabled) dump_memory();
    SDL_DestroyTexture(tex); SDL_DestroyRenderer(ren); SDL_DestroyWindow(win); SDL_Quit();
#endif
    return 0;
}