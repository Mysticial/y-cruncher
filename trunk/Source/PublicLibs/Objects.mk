ifndef YMP_PublicLibs
YMP_PublicLibs := YMP_PublicLibs

CURRENT_DIR := PublicLibs
CURRENT :=


CURRENT += StringTools/ToString.cpp
CURRENT += StringTools/Unicode.cpp
CURRENT += StringTools/ymb_STR.cpp

CURRENT += ConsoleIO/BasicIO.cpp
CURRENT += ConsoleIO/Margin.cpp
CURRENT += ConsoleIO/Label.cpp
CURRENT += ConsoleIO/Array.cpp

CURRENT += ErrorHandling.cpp

CURRENT += Time/Time.cpp
CURRENT += Time/StopWatch.cpp

CURRENT += Memory/AlignedMalloc.cpp
CURRENT += ManagedMemory/CppAllocator.cpp

CURRENT += FileIO/FileException.cpp
CURRENT += FileIO/FileIO.cpp
CURRENT += FileIO/BasicFile.cpp
CURRENT += Serialization/Serialization.cpp
CURRENT += Exceptions/SMC_Exceptions.cpp

CURRENT += Environment/Environment.cpp
CURRENT += ProcessorCapability/cpu_x86.cpp
CURRENT += Concurrency/Parallelizers.cpp

CURRENT += CompileOptions_PublicLibs.cpp


SOURCES := $(SOURCES) $(addprefix $(CURRENT_DIR)/, $(CURRENT))
endif
