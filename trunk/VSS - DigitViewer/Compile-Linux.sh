mkdir "Binaries" #
#
#
dir=$(pwd)/$(dirname $0) #
echo ${dir} #
#
#
gccflags="-std=c++0x -static -Wall -Wno-unused-function -O2 -D YMP_DIGITVIEWER_STANDALONE" #
includepath="${dir}/../Source" #
source0="../Source/DigitViewer/Main.cpp" #
source1="../Source/DigitViewer/SMC_DigitViewer.cpp" #
#
echo "Compiling x64 SSE3..." #
g++ "${source0}" "${source1}" "${source2}" -I "${includepath}" ${gccflags} -D X64_04_SSE3    -msse3                -o "Binaries/Digit Viewer - x64 SSE3" #
#
echo "Compiling x64 SSE4.1..." #
g++ "${source0}" "${source1}" "${source2}" -I "${includepath}" ${gccflags} -D X64_07_Penryn  -march=core2 -msse4.1 -o "Binaries/Digit Viewer - x64 SSE4.1" #
#
echo "Compiling x64 AVX2..." #
g++ "${source0}" "${source1}" "${source2}" -I "${includepath}" ${gccflags} -D X64_13_Haswell -march=haswell        -o "Binaries/Digit Viewer - x64 AVX2" #
#
