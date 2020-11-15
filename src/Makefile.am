bin_PROGRAMS=ucac4_to_index
ucac4_to_index_SOURCES=ACatalog.cpp ACatUCAC4.cpp AMath.cpp ATimeSpace.cpp \
    Projection.cpp ucac4_to_index.cpp

if DEBUG
  AM_CFLAGS = -g3 -O0 -Wall -DNDEBUG
  AM_CXXFLAGS = -g3 -O0 -Wall -DNDEBUG
else
  AM_CFLAGS = -O3 -Wall
  AM_CXXFLAGS = -O3 -Wall
endif

ucac4_to_index_LDADD = -lm -lcfitsio 
