@REM Удаляем папку build если есть
rd /s /q "build"

@REM Генерим build через cmake
cmake -S . -B build -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Debug

@REM Компилим проект
C:/msys64/mingw64/bin/mingw32-make.exe -C build -j