ifndef YMP_DigitViewer2
YMP_DigitViewer2 := YMP_DigitViewer2

CURRENT_DIR := DigitViewer2
CURRENT :=


CURRENT += PrintHelpers.cpp
CURRENT += DigitCount/DigitCount.cpp
CURRENT += DigitHash/DigitHash.cpp

CURRENT += RawToAscii/RawToAscii.cpp
CURRENT += RawToCompressed/RawToCompressed.cpp
CURRENT += WordToRaw/WordToRaw.cpp

CURRENT += DigitReaders/InconsistentMetadataException.cpp
CURRENT += DigitReaders/ParsingTools.cpp
CURRENT += DigitReaders/BasicTextReader.cpp
CURRENT += DigitReaders/BasicYcdFileReader.cpp
CURRENT += DigitReaders/BasicYcdSetReader.cpp

CURRENT += DigitWriters/BasicTextWriter.cpp
CURRENT += DigitWriters/BasicYcdFileWriter.cpp
CURRENT += DigitWriters/BasicYcdSetWriter.cpp

CURRENT += DigitViewer/DigitViewerTasks.cpp
CURRENT += DigitViewer/DigitViewerUI2.cpp


SOURCES := $(SOURCES) $(addprefix $(CURRENT_DIR)/, $(CURRENT))
endif
