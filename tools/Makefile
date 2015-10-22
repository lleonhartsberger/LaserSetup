# Objs            = root_plots_single_meas.o
Objs            = analyze_00.o fourier.o
CC              = gcc
CXX             = g++
CCFlags         = -g -O1 -w -Wall -pedantic -fPIC `root-config --cflags` -Wcpp
#DevFlags                   = -D__CBCDAQ_DEV__
DevFlags		=

IncludeDirs     =  ../

IncludePaths            = $(IncludeDirs:%=-I%)

%.o: %.cc %.h
	$(CXX) -std=c++11  $(DevFlags) $(CCFlags) $(UserCCFlags) $(CCDefines) $(IncludePaths) -c -o $@ $<

all: $(Objs) 
	$(CC) -std=c++11 -shared -o libTools.so $(Objs) 
	mv libTools.so ../lib

clean:
	rm -f *.o
