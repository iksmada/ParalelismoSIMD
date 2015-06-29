call "C:\Program Files (x86)\Microsoft Visual Studio 11.0\VC\vcvarsall.bat"

cl simd.c
cl openMP.c
cl original.c

simd
openMP
original
pause