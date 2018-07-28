SOURCE_ROOT := $(shell dirname $(realpath $(lastword $(MAKEFILE_LIST))))
OUTPUT_ROOT := $(SOURCE_ROOT)/../Linux-y-cruncher

CC=g++

CFLAGS := -D YMP_BUILD_DEVELOPER
CFLAGS += -I $(SOURCE_ROOT) -std=c++14 -fno-rtti -Wall -O2 -g -ggdb
CFLAGS += -D YMP_STANDALONE -D YMP_ENABLE_CILK -D YMP_ENABLE_LIBNUMA
CFLAGS += -lpthread -fcilkplus -lnuma
CFLAGS += -Wl,-rpath=.,-rpath=Binaries



#BINARY := 05-A64
#CFLAGS += -D X64_04_SSE3           -msse3 -mtune=barcelona

#BINARY := 07-PNR
#CFLAGS += -D X64_07_Penryn         -march=core2 -msse4.1 -mtune=core2

#BINARY := 08-NHM
#CFLAGS += -D X64_08_Nehalem        -march=core2 -msse4.1 -mtune=nehalem

#BINARY := 11-SNB
#CFLAGS += -D X64_11_SandyBridge    -march=sandybridge -mtune=sandybridge

#BINARY := 11-BD1
#CFLAGS += -D X64_11_Bulldozer      -march=bdver1  -mtune=bdver1

BINARY := 13-HSW
CFLAGS += -D X64_13_Haswell        -march=haswell -mtune=haswell

#BINARY := 14-BDW
#CFLAGS += -D X64_14_Broadwell      -march=broadwell -mtune=broadwell

#BINARY := 16-KNL
#CFLAGS += -D X64_16_KnightsLanding -march=knl -mtune=knl

#BINARY := 17-ZD1
#CFLAGS += -D X64_17_Zen            -march=znver1 -mtune=znver1

#BINARY := 17-SKX
#CFLAGS += -D X64_17_Skylake        -march=skylake-avx512 -mtune=skylake-avx512

#BINARY := 17-CNL
#CFLAGS += -D X64_18_CannonLake     -march=skylake-avx512 -mavx512ifma -mavx512vbmi -mtune=skylake-avx512



OUTPUT := $(OUTPUT_ROOT)/$(BINARY)


.DEFAULT_GOAL := all

-include $(addprefix $(OUTPUT)/, $(SOURCES:.cpp=.d))

$(OUTPUT)/%.o: $(SOURCE_ROOT)/%.cpp
	mkdir -p $(shell dirname $@)
	$(CC) $< $(CFLAGS) -c  -o $@
	$(CC) $< $(CFLAGS) -MM -MT $@ > $(@:.o=.d)
	@cp -f $(@:.o=.d) $(@:.o=.d.tmp)
	@sed -e 's/.*://' -e 's/\\$$//' < $(@:.o=.d.tmp) | fmt -1 | \
	 sed -e 's/^ *//' -e 's/$$/:/' >> $(@:.o=.d)
	@rm -f $(@:.o=.d.tmp)

dump:
	echo $(SOURCES)

clean:
	rm -r $(OUTPUT_ROOT)

all: $(addprefix $(OUTPUT)/, $(SOURCES:.cpp=.o))




