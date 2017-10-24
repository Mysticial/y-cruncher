ifndef YCR_DigitViewer
YCR_DigitViewer := YCR_DigitViewer


CURRENT_DIR := DigitViewer
CURRENT :=


CURRENT += DigitConverter/DigitConverter.cpp

CURRENT += DigitReaders/DigitReader.cpp
CURRENT += DigitReaders/TextReader.cpp
CURRENT += DigitReaders/YCDReader.cpp
CURRENT += DigitReaders/YCDFileReader.cpp

CURRENT += DigitWriters/DigitWriter.cpp
CURRENT += DigitWriters/TextWriter.cpp
CURRENT += DigitWriters/YCDWriter.cpp
CURRENT += DigitWriters/YCDFileWriter.cpp

CURRENT += PrintHelpers.cpp
CURRENT += EndPointTasks.cpp
CURRENT += Menu_DigitViewer.cpp


SOURCES := $(SOURCES) $(addprefix $(CURRENT_DIR)/, $(CURRENT))
endif
