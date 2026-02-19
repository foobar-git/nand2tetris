# Hack Computer (nand2tetris) Emulator written in C
* The world's first sound-barrier-breaking Hack CPU emulator, powered by the Nitro Fart kinetic-pressure engine.
* Optimized for performance that is officially Over 9000.

## ⚡ Features <br>
* Dual-Platform Support: Native Win32 GDI for Windows and SDL2 for Linux.
* Nitro Blitting: High-speed memory-to-screen pixel transfer.
* Recursive Engineering: An emulator written in C, running code compiled from VM code, designed by a Super Saiyan.
* The Nitro Exhaust: Optional memory dumping (-md) for debugging.

## 🛠️ Compilation <br>
* Windows (MSVC or MinGW): You'll need the GDI32 library (included with Windows).
```
Bash
gcc SupaTinendo.c -o SupaTinendo.exe -lgdi32
```
* Linux: You'll need the SDL2 development headers.
```
Bash
sudo apt-get install libsdl2-dev
gcc SupaTinendo.c -o SupaTinendo -lSDL2
```
🚀 Usage <br>
* Load any .hack binary file directly into the ROM. Use the -md flag if you want a RAM dump upon exit/halt.
* Close Window: triggers memory dump if it was enabled.
```
Bash
./SupaTinendo game.hack -md
```

---
📜 This project is a custom implementation of the hardware platform described in **"The Elements of Computing Systems"** by **Noam Nisan** and **Shimon Schocken**.
* **Original architecture:** the Hack CPU and Hardware Platform were made by the [Nand2Tetris](https://www.nand2tetris.org/) team in Java and later in JavaScript.
* **This emulator logic:** written in C by [Master Foo](https://github.com/foobar-git/).
* Some inspiration kicked in late in the midnight hours, so I wrote this humorous readme. Cheers! [ZENMASTERS Games](https://games.zenmasters.software/)
