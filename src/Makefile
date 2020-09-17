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

SRCS = \
    filter-2d.c \
    filter-vect.cpp \
    filter.c \
    interp.c \
    tpl-base.h \
    tpl-filter.h \
    tpl-image.h \
    tpl-inline.h \
    tpl-interp.h

OBJS = \
    filter-2d.o \
    filter-vect.o \
    filter.o \
    interp.o

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

interp.e: tpl-base.h tpl-interp.h
interp.S: tpl-base.h tpl-interp.h
interp.o: tpl-base.h tpl-interp.h

filter.e: tpl-base.h tpl-filter.h
filter.S: tpl-base.h tpl-filter.h
filter.o: tpl-base.h tpl-filter.h

filter-vect.e: tpl-base.h tpl-filter.h
filter-vect.S: tpl-base.h tpl-filter.h
filter-vect.o: tpl-base.h tpl-filter.h

filter-2d.e: tpl-base.h tpl-image.h tpl-inline.h
filter-2d.S: tpl-base.h tpl-image.h tpl-inline.h
filter-2d.o: tpl-base.h tpl-image.h tpl-inline.h
