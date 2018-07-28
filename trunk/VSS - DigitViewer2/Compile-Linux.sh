mkdir "Binaries" #
#
#
dir=$(pwd)/$(dirname $0) #
echo ${dir} #
#
#
gccflags="-std=c++14 -Wall -Wno-unused-function -O2 -lpthread" #
includepath="${dir}/../Source" #
#
SOURCES= #
SOURCES+=" ../Source/PublicLibs/SMC_PublicLibs.cpp" #
SOURCES+=" ../Source/DigitViewer/PrintHelpers.cpp" #
SOURCES+=" ../Source/DigitViewer/DigitReaders/InconsistentMetadataException.cpp" #
SOURCES+=" ../Source/DigitViewer/DigitReaders/ParsingTools.cpp" #
SOURCES+=" ../Source/DigitViewer2/SMC_DigitViewer2.cpp" #
SOURCES+=" ../Source/DigitViewer2/Main.cpp" #
#
echo "Compiling 05-A64 (x64 SSE3)..." #
g++ $SOURCES -I "${includepath}" ${gccflags} -D X64_04_SSE3    -msse3                -o "Binaries/Digit Viewer - 05-A64" #
#
echo "Compiling 07-PNR (x64 SSE4.1)..." #
g++ $SOURCES -I "${includepath}" ${gccflags} -D X64_07_Penryn  -march=core2 -msse4.1 -o "Binaries/Digit Viewer - 07-PNR" #
#
echo "Compiling 13-HSW (x64 AVX2)..." #
g++ $SOURCES -I "${includepath}" ${gccflags} -D X64_13_Haswell -march=haswell        -o "Binaries/Digit Viewer - 13-HSW" #
#
echo "Compiling 17-SKX (x64 AVX512BW)..." #
g++ $SOURCES -I "${includepath}" ${gccflags} -D X64_17_Skylake -march=skylake-avx512 -o "Binaries/Digit Viewer - 17-SKX" #
#
