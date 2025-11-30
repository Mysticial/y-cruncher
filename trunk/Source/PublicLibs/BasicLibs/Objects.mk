ifndef YMP_PublicBasicLibs
YMP_PublicBasicLibs := YMP_PublicBasicLibs

CURRENT_DIR := PublicLibs/BasicLibs
CURRENT :=


CURRENT += StringTools/ToString.cpp
CURRENT += StringTools/Unicode.cpp
CURRENT += StringTools/ymb_STR.cpp

CURRENT += LargePrimitives/Int128.cpp
CURRENT += LargePrimitives/Int128_IO.cpp

CURRENT += ConfigTree/ConfigValue.cpp
CURRENT += ConfigTree/ConfigArray.cpp
CURRENT += ConfigTree/ConfigObject.cpp
CURRENT += ConfigTree/ConfigParser.cpp

CURRENT += Concurrency/ExceptionHolder.cpp
CURRENT += Concurrency/Parallelizer.cpp
CURRENT += Concurrency/NullParallelizer.cpp
CURRENT += Memory/AlignedMalloc.cpp
CURRENT += Serialization/Serialization.cpp


SOURCES := $(SOURCES) $(addprefix $(CURRENT_DIR)/, $(CURRENT))
endif
