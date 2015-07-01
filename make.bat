call "C:\Program Files (x86)\Microsoft Visual Studio 11.0\VC\vcvarsall.bat"

cl simd.c
cl original.c
cl openMP.c

simd
original
openMP
pause