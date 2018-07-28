ifndef YCR_DigitViewer
YCR_DigitViewer := YCR_DigitViewer


CURRENT_DIR := DigitViewer
CURRENT :=


CURRENT += DigitConverter/DigitConverter.cpp

CURRENT += DigitReaders/InconsistentMetadataException.cpp
CURRENT += DigitReaders/ParsingTools.cpp
CURRENT += DigitReaders/DigitReader.cpp
CURRENT += DigitReaders/TextDigitReader.cpp
CURRENT += DigitReaders/YCDDigitReader.cpp
CURRENT += DigitReaders/YCDFileReader.cpp

CURRENT += DigitWriters/DigitWriter.cpp
CURRENT += DigitWriters/TextDigitWriter.cpp
CURRENT += DigitWriters/YCDDigitWriter.cpp
CURRENT += DigitWriters/YCDFileWriter.cpp

CURRENT += PrintHelpers.cpp
CURRENT += EndPointTasks.cpp
CURRENT += Menu_DigitViewer.cpp


SOURCES := $(SOURCES) $(addprefix $(CURRENT_DIR)/, $(CURRENT))
endif
