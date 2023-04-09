# standard compile options for the c++ executable
FLAGS = -fPIC

# the python interface through swig
PYTHONI = -I/home/lachlan/.pyenv/versions/3.10.6/include/python3.10
PYTHONL = -Xlinker -export-dynamic

SOURCEDIR = src/
BUILDDIR = build/
PYDIR = ./

# default super-target
all: 
	g++ -fPIC -c $(SOURCEDIR)/kitty.cc -o $(BUILDDIR)/kitty.o
	swig -c++ -python -o $(BUILDDIR)kitty_wrap.cxx $(SOURCEDIR)kitty.i 
	g++ $(FLAGS) $(PYTHONI) -c $(BUILDDIR)kitty_wrap.cxx -o $(BUILDDIR)kitty_wrap.o
	g++ $(PYTHONL) $(LIBFLAGS) -shared $(BUILDDIR)kitty.o $(BUILDDIR)kitty_wrap.o -o $(PYDIR)_kitty.so