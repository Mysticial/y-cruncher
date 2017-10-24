include SourceRoot.mk
include Objects.mk
include Common.mk

all: $(addprefix $(OUTPUT)/, $(SOURCES:.cpp=.o))
	mkdir -p $(OUTPUT_ROOT)/Binaries/
	$(CC) ScratchLinux.cpp $^ $(CFLAGS) -o $(OUTPUT_ROOT)/Binaries/$(BINARY)
