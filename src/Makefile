srcdir = .

# The PVC Library.
#
# The PVC Library is needed to compile TPL, see https://github.com/emmt/PVC
# to get it.
#
# Macro `PVC_DEFS` specifies the preprocessor flags needed to find the
# headers of the PVC Library.
#
PVC_DIR = $(srcdir)/../../pvc
PVC_DEFS = -I$(PVC_DIR)/src

# The Vector Class Library.
#
# Just define `VCL_DEFS = -DUSE_VCL=0` to not use this library for SIMD
# vectorization.
#
VCL_DEFS = -I$(VCL_SRC) -DUSE_VCL=1
VCL_SRC = $(srcdir)/../../vectorclasslibrary/version2

# Compiler flags for vectorization.
VECTORIZE = -O3 -march=native -ffast-math -funroll-loops

CC = gcc
CFLAGS = -Wall -I$(srcdir) $(VCL_DEFS) $(PVC_DEFS) $(VECTORIZE)

CXX = gcc -std=c++17
CXXFLAGS = $(CFLAGS)

OBJS = filter.o filter-vect.o

default: all
all: $(OBJS)

clean:
	rm -f *.o *~

%.e: $(srcdir)/%.cpp
	$(CXX) $(CXXFLAGS) -E "$<" -o "$@"
%.e: $(srcdir)/%.c
	$(CC) $(CFLAGS) -E "$<" -o "$@"

%.S: $(srcdir)/%.cpp
	$(CXX) $(CXXFLAGS) -S "$<" -o "$@"
%.S: $(srcdir)/%.c
	$(CC) $(CFLAGS) -S "$<" -o "$@"

%.o: $(srcdir)/%.cpp
	$(CXX) $(CXXFLAGS) -c "$<" -o "$@"
%.o: $(srcdir)/%.c
	$(CC) $(CFLAGS) -c "$<" -o "$@"

filter.e: tpl-filter.h
filter.S: tpl-filter.h
filter.o: tpl-filter.h

filter-vect.e: tpl-filter.h
filter-vect.S: tpl-filter.h
filter-vect.o: tpl-filter.h