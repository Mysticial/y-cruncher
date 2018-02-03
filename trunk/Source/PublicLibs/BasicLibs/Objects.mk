ifndef YMP_PublicBasicLibs
YMP_PublicBasicLibs := YMP_PublicBasicLibs

CURRENT_DIR := PublicLibs/BasicLibs
CURRENT :=


CURRENT += StringTools/ToString.cpp
CURRENT += StringTools/Unicode.cpp
CURRENT += StringTools/ymb_STR.cpp

CURRENT += Memory/AlignedMalloc.cpp
CURRENT += Serialization/Serialization.cpp


SOURCES := $(SOURCES) $(addprefix $(CURRENT_DIR)/, $(CURRENT))
endif
