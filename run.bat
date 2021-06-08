@echo off
setlocal enabledelayedexpansion

set /p var1=input file:
set /p var2=output file:
set /p var3=words split result ouput file:

if "%var1%"=="" set var1=input06
if "%var2%"=="" set var2=%var1%.out
if "%var3%"=="" set var3=%var1%.words

cd build
cmake ..
make -j

echo [target built]
echo input file                    : %var1%
echo output file                   : %var2%
echo words split result ouput file : %var3%

complier.exe ../%var1% ../%var2% ../%var3%

echo [run complier compeleted]