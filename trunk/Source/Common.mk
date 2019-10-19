SOURCE_ROOT := $(shell dirname $(realpath $(lastword $(MAKEFILE_LIST))))
OUTPUT_ROOT := $(SOURCE_ROOT)/../Linux-y-cruncher

CC=g++

CFLAGS := -D YMP_BUILD_DEVELOPER
CFLAGS += -I $(SOURCE_ROOT) -std=c++17 -fno-rtti -Wall -O2 -g -ggdb
CFLAGS += -D YMP_STANDALONE -D YMP_ENABLE_LIBNUMA -D YMP_ENABLE_CILK -D YMP_ENABLE_TBB
CFLAGS += -lpthread -lnuma -fcilkplus -ltbb
CFLAGS += -Wl,-rpath=.,-rpath=Binaries


dump:
	echo $(SOURCES)

clean:
	rm -r $(OUTPUT_ROOT)

all: 05-A64 07-PNR 08-NHM 11-BD1 11-SNB 13-HSW 14-BDW 16-KNL 17-ZN1 17-SKX 18-CNL 19-ZN2

.DEFAULT_GOAL := all


CFLAGS_05A64 := $(CFLAGS) -D X64_04_SSE3           -msse3 -mtune=barcelona
OUTPUT_05A64 := $(OUTPUT_ROOT)/05-A64
-include $(addprefix $(OUTPUT_05A64)/, $(SOURCES:.cpp=.d))
$(OUTPUT_05A64)/%.o: $(SOURCE_ROOT)/%.cpp
	mkdir -p $(shell dirname $@)
	$(CC) $< $(CFLAGS_05A64) -c  -o $@
	$(CC) $< $(CFLAGS_05A64) -MM -MT $@ > $(@:.o=.d)
	@cp -f $(@:.o=.d) $(@:.o=.d.tmp)
	@sed -e 's/.*://' -e 's/\\$$//' < $(@:.o=.d.tmp) | fmt -1 | \
	 sed -e 's/^ *//' -e 's/$$/:/' >> $(@:.o=.d)
	@rm -f $(@:.o=.d.tmp)
05-A64: $(addprefix $(OUTPUT_05A64)/, $(SOURCES:.cpp=.o))


CFLAGS_07PNR := $(CFLAGS) -D X64_07_Penryn         -march=core2 -msse4.1 -mtune=core2
OUTPUT_07PNR := $(OUTPUT_ROOT)/07-PNR
-include $(addprefix $(OUTPUT_07PNR)/, $(SOURCES:.cpp=.d))
$(OUTPUT_07PNR)/%.o: $(SOURCE_ROOT)/%.cpp
	mkdir -p $(shell dirname $@)
	$(CC) $< $(CFLAGS_07PNR) -c  -o $@
	$(CC) $< $(CFLAGS_07PNR) -MM -MT $@ > $(@:.o=.d)
	@cp -f $(@:.o=.d) $(@:.o=.d.tmp)
	@sed -e 's/.*://' -e 's/\\$$//' < $(@:.o=.d.tmp) | fmt -1 | \
	 sed -e 's/^ *//' -e 's/$$/:/' >> $(@:.o=.d)
	@rm -f $(@:.o=.d.tmp)
07-PNR: $(addprefix $(OUTPUT_07PNR)/, $(SOURCES:.cpp=.o))


CFLAGS_08NHM := $(CFLAGS) -D X64_08_Nehalem        -march=core2 -msse4.1 -mtune=nehalem
OUTPUT_08NHM := $(OUTPUT_ROOT)/08-NHM
-include $(addprefix $(OUTPUT_08NHM)/, $(SOURCES:.cpp=.d))
$(OUTPUT_08NHM)/%.o: $(SOURCE_ROOT)/%.cpp
	mkdir -p $(shell dirname $@)
	$(CC) $< $(CFLAGS_08NHM) -c  -o $@
	$(CC) $< $(CFLAGS_08NHM) -MM -MT $@ > $(@:.o=.d)
	@cp -f $(@:.o=.d) $(@:.o=.d.tmp)
	@sed -e 's/.*://' -e 's/\\$$//' < $(@:.o=.d.tmp) | fmt -1 | \
	 sed -e 's/^ *//' -e 's/$$/:/' >> $(@:.o=.d)
	@rm -f $(@:.o=.d.tmp)
08-NHM: $(addprefix $(OUTPUT_08NHM)/, $(SOURCES:.cpp=.o))


CFLAGS_11BD1 := $(CFLAGS) -D X64_11_Bulldozer      -march=sandybridge -mfma4 -mabm -mtune=bdver1
OUTPUT_11BD1 := $(OUTPUT_ROOT)/11-BD1
-include $(addprefix $(OUTPUT_11BD1)/, $(SOURCES:.cpp=.d))
$(OUTPUT_11BD1)/%.o: $(SOURCE_ROOT)/%.cpp
	mkdir -p $(shell dirname $@)
	$(CC) $< $(CFLAGS_11BD1) -c  -o $@
	$(CC) $< $(CFLAGS_11BD1) -MM -MT $@ > $(@:.o=.d)
	@cp -f $(@:.o=.d) $(@:.o=.d.tmp)
	@sed -e 's/.*://' -e 's/\\$$//' < $(@:.o=.d.tmp) | fmt -1 | \
	 sed -e 's/^ *//' -e 's/$$/:/' >> $(@:.o=.d)
	@rm -f $(@:.o=.d.tmp)
11-BD1: $(addprefix $(OUTPUT_11BD1)/, $(SOURCES:.cpp=.o))


CFLAGS_11SNB := $(CFLAGS) -D X64_11_SandyBridge    -march=sandybridge -mtune=sandybridge
OUTPUT_11SNB := $(OUTPUT_ROOT)/11-SNB
-include $(addprefix $(OUTPUT_11SNB)/, $(SOURCES:.cpp=.d))
$(OUTPUT_11SNB)/%.o: $(SOURCE_ROOT)/%.cpp
	mkdir -p $(shell dirname $@)
	$(CC) $< $(CFLAGS_11SNB) -c  -o $@
	$(CC) $< $(CFLAGS_11SNB) -MM -MT $@ > $(@:.o=.d)
	@cp -f $(@:.o=.d) $(@:.o=.d.tmp)
	@sed -e 's/.*://' -e 's/\\$$//' < $(@:.o=.d.tmp) | fmt -1 | \
	 sed -e 's/^ *//' -e 's/$$/:/' >> $(@:.o=.d)
	@rm -f $(@:.o=.d.tmp)
11-SNB: $(addprefix $(OUTPUT_11SNB)/, $(SOURCES:.cpp=.o))


CFLAGS_13HSW := $(CFLAGS) -D X64_13_Haswell        -march=haswell -mtune=haswell
OUTPUT_13HSW := $(OUTPUT_ROOT)/13-HSW
-include $(addprefix $(OUTPUT_13HSW)/, $(SOURCES:.cpp=.d))
$(OUTPUT_13HSW)/%.o: $(SOURCE_ROOT)/%.cpp
	mkdir -p $(shell dirname $@)
	$(CC) $< $(CFLAGS_13HSW) -c  -o $@
	$(CC) $< $(CFLAGS_13HSW) -MM -MT $@ > $(@:.o=.d)
	@cp -f $(@:.o=.d) $(@:.o=.d.tmp)
	@sed -e 's/.*://' -e 's/\\$$//' < $(@:.o=.d.tmp) | fmt -1 | \
	 sed -e 's/^ *//' -e 's/$$/:/' >> $(@:.o=.d)
	@rm -f $(@:.o=.d.tmp)
13-HSW: $(addprefix $(OUTPUT_13HSW)/, $(SOURCES:.cpp=.o))


CFLAGS_14BDW := $(CFLAGS) -D X64_14_Broadwell      -march=broadwell -mtune=skylake
OUTPUT_14BDW := $(OUTPUT_ROOT)/14-BDW
-include $(addprefix $(OUTPUT_14BDW)/, $(SOURCES:.cpp=.d))
$(OUTPUT_14BDW)/%.o: $(SOURCE_ROOT)/%.cpp
	mkdir -p $(shell dirname $@)
	$(CC) $< $(CFLAGS_14BDW) -c  -o $@
	$(CC) $< $(CFLAGS_14BDW) -MM -MT $@ > $(@:.o=.d)
	@cp -f $(@:.o=.d) $(@:.o=.d.tmp)
	@sed -e 's/.*://' -e 's/\\$$//' < $(@:.o=.d.tmp) | fmt -1 | \
	 sed -e 's/^ *//' -e 's/$$/:/' >> $(@:.o=.d)
	@rm -f $(@:.o=.d.tmp)
14-BDW: $(addprefix $(OUTPUT_14BDW)/, $(SOURCES:.cpp=.o))


CFLAGS_16KNL := $(CFLAGS) -D X64_16_KnightsLanding -march=knl -mtune=knl
OUTPUT_16KNL := $(OUTPUT_ROOT)/16-KNL
-include $(addprefix $(OUTPUT_16KNL)/, $(SOURCES:.cpp=.d))
$(OUTPUT_16KNL)/%.o: $(SOURCE_ROOT)/%.cpp
	mkdir -p $(shell dirname $@)
	$(CC) $< $(CFLAGS_16KNL) -c  -o $@
	$(CC) $< $(CFLAGS_16KNL) -MM -MT $@ > $(@:.o=.d)
	@cp -f $(@:.o=.d) $(@:.o=.d.tmp)
	@sed -e 's/.*://' -e 's/\\$$//' < $(@:.o=.d.tmp) | fmt -1 | \
	 sed -e 's/^ *//' -e 's/$$/:/' >> $(@:.o=.d)
	@rm -f $(@:.o=.d.tmp)
16-KNL: $(addprefix $(OUTPUT_16KNL)/, $(SOURCES:.cpp=.o))


CFLAGS_17ZN1 := $(CFLAGS) -D X64_17_Zen1           -march=znver1 -mtune=znver1
OUTPUT_17ZN1 := $(OUTPUT_ROOT)/17-ZN1
-include $(addprefix $(OUTPUT_17ZN1)/, $(SOURCES:.cpp=.d))
$(OUTPUT_17ZN1)/%.o: $(SOURCE_ROOT)/%.cpp
	mkdir -p $(shell dirname $@)
	$(CC) $< $(CFLAGS_17ZN1) -c  -o $@
	$(CC) $< $(CFLAGS_17ZN1) -MM -MT $@ > $(@:.o=.d)
	@cp -f $(@:.o=.d) $(@:.o=.d.tmp)
	@sed -e 's/.*://' -e 's/\\$$//' < $(@:.o=.d.tmp) | fmt -1 | \
	 sed -e 's/^ *//' -e 's/$$/:/' >> $(@:.o=.d)
	@rm -f $(@:.o=.d.tmp)
17-ZN1: $(addprefix $(OUTPUT_17ZN1)/, $(SOURCES:.cpp=.o))


CFLAGS_17SKX := $(CFLAGS) -D X64_17_Skylake        -march=skylake-avx512 -mtune=skylake-avx512
OUTPUT_17SKX := $(OUTPUT_ROOT)/17-SKX
-include $(addprefix $(OUTPUT_17SKX)/, $(SOURCES:.cpp=.d))
$(OUTPUT_17SKX)/%.o: $(SOURCE_ROOT)/%.cpp
	mkdir -p $(shell dirname $@)
	$(CC) $< $(CFLAGS_17SKX) -c  -o $@
	$(CC) $< $(CFLAGS_17SKX) -MM -MT $@ > $(@:.o=.d)
	@cp -f $(@:.o=.d) $(@:.o=.d.tmp)
	@sed -e 's/.*://' -e 's/\\$$//' < $(@:.o=.d.tmp) | fmt -1 | \
	 sed -e 's/^ *//' -e 's/$$/:/' >> $(@:.o=.d)
	@rm -f $(@:.o=.d.tmp)
17-SKX: $(addprefix $(OUTPUT_17SKX)/, $(SOURCES:.cpp=.o))


CFLAGS_18CNL := $(CFLAGS) -D X64_18_CannonLake     -march=skylake-avx512 -mavx512ifma -mavx512vbmi -mtune=skylake-avx512
OUTPUT_18CNL := $(OUTPUT_ROOT)/18-CNL
-include $(addprefix $(OUTPUT_18CNL)/, $(SOURCES:.cpp=.d))
$(OUTPUT_18CNL)/%.o: $(SOURCE_ROOT)/%.cpp
	mkdir -p $(shell dirname $@)
	$(CC) $< $(CFLAGS_18CNL) -c  -o $@
	$(CC) $< $(CFLAGS_18CNL) -MM -MT $@ > $(@:.o=.d)
	@cp -f $(@:.o=.d) $(@:.o=.d.tmp)
	@sed -e 's/.*://' -e 's/\\$$//' < $(@:.o=.d.tmp) | fmt -1 | \
	 sed -e 's/^ *//' -e 's/$$/:/' >> $(@:.o=.d)
	@rm -f $(@:.o=.d.tmp)
18-CNL: $(addprefix $(OUTPUT_18CNL)/, $(SOURCES:.cpp=.o))


CFLAGS_19ZN2 := $(CFLAGS) -D X64_19_Zen2           -march=znver1 -mtune=znver1
OUTPUT_19ZN2 := $(OUTPUT_ROOT)/19-ZN2
-include $(addprefix $(OUTPUT_19ZN2)/, $(SOURCES:.cpp=.d))
$(OUTPUT_19ZN2)/%.o: $(SOURCE_ROOT)/%.cpp
	mkdir -p $(shell dirname $@)
	$(CC) $< $(CFLAGS_19ZN2) -c  -o $@
	$(CC) $< $(CFLAGS_19ZN2) -MM -MT $@ > $(@:.o=.d)
	@cp -f $(@:.o=.d) $(@:.o=.d.tmp)
	@sed -e 's/.*://' -e 's/\\$$//' < $(@:.o=.d.tmp) | fmt -1 | \
	 sed -e 's/^ *//' -e 's/$$/:/' >> $(@:.o=.d)
	@rm -f $(@:.o=.d.tmp)
19-ZN2: $(addprefix $(OUTPUT_19ZN2)/, $(SOURCES:.cpp=.o))







