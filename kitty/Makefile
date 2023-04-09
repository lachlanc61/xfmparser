# standard compile options for the c++ executable
FLAGS = -fPIC

# the python interface through swig
PYTHONI = -I/home/lachlan/.pyenv/versions/3.10.6/include/python3.10
PYTHONL = -Xlinker -export-dynamic

SOURCEDIR = ./

# default super-target
all: 
	g++ -fPIC -c $(SOURCEDIR)/kitty.cc -o $(SOURCEDIR)/kitty.o
	swig -c++ -python -o $(SOURCEDIR)kitty_wrap.cxx $(SOURCEDIR)kitty.i 
	g++ $(FLAGS) $(PYTHONI) -c $(SOURCEDIR)kitty_wrap.cxx -o $(SOURCEDIR)kitty_wrap.o
	g++ $(PYTHONL) $(LIBFLAGS) -shared $(SOURCEDIR)kitty.o $(SOURCEDIR)kitty_wrap.o -o $(SOURCEDIR)_kitty.so

clean:
	-rm *.o *_wrap.cxx *_wrap.o 

uninstall:
	-rm *.o *_wrap.cxx *_wrap.o 
	-rm *.so *_wrap.cxx *_wrap.o *.py
