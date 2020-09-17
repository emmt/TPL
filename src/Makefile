# Where are the sources?
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
CFLAGS = -Wall  $(VECTORIZE)
CPPFLAGS = -I$(srcdir) $(VCL_DEFS) $(PVC_DEFS)

CXX = gcc -std=c++17
CXXFLAGS = $(CFLAGS)

LIBS = -L. -ltpl -lm

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
all: libtpl.a interp-tests

clean:
	rm -f *.o *~

libtpl.a: $(OBJS)
	$(AR) $(ARFLAGS) $@ $(OBJS)

%.e: $(srcdir)/%.cpp
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -E "$<" -o $@

%.e: $(srcdir)/%.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -E "$<" -o $@

%.S: $(srcdir)/%.cpp
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -S "$<" -o $@

%.S: $(srcdir)/%.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -S "$<" -o $@

%.o: $(srcdir)/%.cpp
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c "$<" -o $@

%.o: $(srcdir)/%.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c "$<" -o $@

interp.e: $(srcdir)/tpl-base.h $(srcdir)/tpl-interp.h
interp.S: $(srcdir)/tpl-base.h $(srcdir)/tpl-interp.h
interp.o: $(srcdir)/tpl-base.h $(srcdir)/tpl-interp.h

filter.e: $(srcdir)/tpl-base.h $(srcdir)/tpl-filter.h
filter.S: $(srcdir)/tpl-base.h $(srcdir)/tpl-filter.h
filter.o: $(srcdir)/tpl-base.h $(srcdir)/tpl-filter.h

filter-vect.e: $(srcdir)/tpl-base.h $(srcdir)/tpl-filter.h
filter-vect.S: $(srcdir)/tpl-base.h $(srcdir)/tpl-filter.h
filter-vect.o: $(srcdir)/tpl-base.h $(srcdir)/tpl-filter.h

filter-2d.e: $(srcdir)/tpl-base.h $(srcdir)/tpl-image.h $(srcdir)/tpl-inline.h
filter-2d.S: $(srcdir)/tpl-base.h $(srcdir)/tpl-image.h $(srcdir)/tpl-inline.h
filter-2d.o: $(srcdir)/tpl-base.h $(srcdir)/tpl-image.h $(srcdir)/tpl-inline.h

interp-tests: $(srcdir)/interp-tests.c $(srcdir)/tpl-base.h $(srcdir)/tpl-interp.h
%: $(srcdir)/%.c
	$(CC) $(CFLAGS) $(CPPFLAGS) $^ -o $@ $(LIBS)
