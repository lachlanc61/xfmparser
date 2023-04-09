# standard compile options for the c++ executable
FLAGS = -fPIC

NAME=xfmcparser

# the python interface through swig
PYTHONI = -I/home/lachlan/.pyenv/versions/3.10.6/include/python3.10
PYTHONL = -Xlinker -export-dynamic

SOURCEDIR = ./

# default super-target
all: 
	g++ $(FLAGS) -c $(SOURCEDIR)/$(NAME).cpp -o $(SOURCEDIR)/$(NAME).o
	swig -c++ -python -o $(SOURCEDIR)$(NAME)_wrap.cxx $(SOURCEDIR)$(NAME).i 
	g++ $(FLAGS) $(PYTHONI) -c $(SOURCEDIR)$(NAME)_wrap.cxx -o $(SOURCEDIR)$(NAME)_wrap.o
	g++ $(PYTHONL) $(LIBFLAGS) -shared $(SOURCEDIR)$(NAME).o $(SOURCEDIR)$(NAME)_wrap.o -o $(SOURCEDIR)_$(NAME).so

clean:
	-rm *.o *_wrap.cxx *_wrap.o 

uninstall:
	-rm *.o *_wrap.cxx *_wrap.o 
	-rm *.so *.py
