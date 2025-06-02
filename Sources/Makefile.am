SUBDIRS = . Tools

lib_LTLIBRARIES = libMODE7.la

libMODE7_la_SOURCES = Object.cc Singleton.cc \
	Carousel/Carousel.cc Carousel/Magazine.cc Carousel/Page.cc Carousel/Frame.cc \
	Carousel/Cursor.cc \
	\
	Generators/Generator.cc Generators/Clock.cc Generators/TeletextGenerator.cc \
	Generators/SerialTeletextGenerator.cc Generators/ParallelTeletextGenerator.cc \
	\
	Sources/Source.cc Sources/SubProcessSource.cc \
	\
	Processors/Processor.cc Processors/RawProcessor.cc Processors/TeletextProcessor.cc \
	\
	Destinations/Destination.cc Destinations/StreamDestination.cc Destinations/FileDestination.cc \
	Destinations/PipeDestination.cc \
	\
	Formats/Tables.c Formats/PacketBuffer.cc Formats/Packet.cc \
	Formats/DebugPackets.cc Formats/T42Packets.cc Formats/T45Packets.cc

libMODE7_la_LDFLAGS = -no-undefined -avoid-version
