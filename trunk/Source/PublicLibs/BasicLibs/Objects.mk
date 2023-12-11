ifndef YMP_PublicBasicLibs
YMP_PublicBasicLibs := YMP_PublicBasicLibs

CURRENT_DIR := PublicLibs/BasicLibs
CURRENT :=


CURRENT += StringTools/ToString.cpp
CURRENT += StringTools/Unicode.cpp
CURRENT += StringTools/ymb_STR.cpp

CURRENT += ConfigTree/ConfigValue.cpp
CURRENT += ConfigTree/ConfigArray.cpp
CURRENT += ConfigTree/ConfigObject.cpp
CURRENT += ConfigTree/ConfigParser.cpp

CURRENT += Concurrency/ExceptionHolder.cpp
CURRENT += Concurrency/BasicParallelizer.cpp
CURRENT += Memory/AlignedMalloc.cpp
CURRENT += Serialization/Serialization.cpp


SOURCES := $(SOURCES) $(addprefix $(CURRENT_DIR)/, $(CURRENT))
endif
