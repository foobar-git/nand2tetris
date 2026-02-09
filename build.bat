@echo off
setlocal enabledelayedexpansion

:: Get the absolute path of the current folder
set "ROOT_DIR=%~dp0"

if not exist "%ROOT_DIR%input" mkdir "%ROOT_DIR%input"
if not exist "%ROOT_DIR%output" mkdir "%ROOT_DIR%output"

echo Starting SupaTinendo Multi-Project Build...
echo ------------------------------------------------------

for /d %%D in ("%ROOT_DIR%input\*") do (
    set "TARGET_NAME=%%~nD"
    
    echo.
    echo [ PROCESSING: !TARGET_NAME! ]
    
    :: --- STEP ONE: VM TRANSLATION ---
    del "%ROOT_DIR%SupaTinendo VM\*.vm" 2>nul
    del "%ROOT_DIR%SupaTinendo VM\*.asm" 2>nul

    copy "%%D\*.vm" "%ROOT_DIR%SupaTinendo VM\" >nul

    pushd "%ROOT_DIR%SupaTinendo VM"
    VMTranslator.exe . !TARGET_NAME!
    popd

    :: --- STEP TWO: ASSEMBLY ---
    if exist "%ROOT_DIR%SupaTinendo VM\!TARGET_NAME!.asm" (
        echo   - Moving !TARGET_NAME!.asm to Assembler engine...
        move "%ROOT_DIR%SupaTinendo VM\!TARGET_NAME!.asm" "%ROOT_DIR%SupaTinendo Assembler\" >nul

        pushd "%ROOT_DIR%SupaTinendo Assembler"
        HackAssembler.exe !TARGET_NAME!.asm
        popd

        :: --- FINAL DELIVERY ---
        if exist "%ROOT_DIR%SupaTinendo Assembler\!TARGET_NAME!.hack" (
            :: Move the binary file
            move "%ROOT_DIR%SupaTinendo Assembler\!TARGET_NAME!.hack" "%ROOT_DIR%output\" >nul
            :: MOVE the assembly file
            move "%ROOT_DIR%SupaTinendo Assembler\!TARGET_NAME!.asm" "%ROOT_DIR%output\" >nul
            
            echo   - SUCCESS: !TARGET_NAME! compiled files moved to .\output\
        ) else (
            echo   - [!] ERROR: HackAssembler failed to create !TARGET_NAME!.hack
        )
    ) else (
        echo   - [!] ERROR: VMTranslator failed to create !TARGET_NAME!.asm
    )

    :: CLEANUP
    del "%ROOT_DIR%SupaTinendo VM\*.vm" 2>nul
)

echo.
echo ------------------------------------------------------
echo Done! All files moved to output.
pause