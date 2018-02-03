ifndef YMP_PublicLibs
YMP_PublicLibs := YMP_PublicLibs

include $(SOURCE_ROOT)/PublicLibs/BasicLibs/Objects.mk
include $(SOURCE_ROOT)/PublicLibs/SystemLibs/Objects.mk

CURRENT_DIR := PublicLibs
CURRENT :=


CURRENT += ConsoleIO/BasicIO.cpp
CURRENT += ConsoleIO/Margin.cpp
CURRENT += ConsoleIO/Label.cpp
CURRENT += ConsoleIO/Array.cpp

CURRENT += ErrorHandling.cpp
CURRENT += Exceptions/SMC_Exceptions.cpp

CURRENT += CompileOptions_PublicLibs.cpp


SOURCES := $(SOURCES) $(addprefix $(CURRENT_DIR)/, $(CURRENT))
endif
