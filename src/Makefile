CC              = gcc
CXX             = g++
CCFlags         = -g -O1 -w -Wall -pedantic -std=c++11 -fPIC 
CCFlagsRoot	= `root-config --cflags`
#DevFlags                    = -D__CBCDAQ_DEV__
DevFlags                   =

IncludeDirs     =   ../

IncludePaths            = $(IncludeDirs:%=-I%)

LibraryDirs = ../lib
LibraryPaths = $(LibraryDirs:%=-L%)

ExternalObjects= $(LibraryPaths) 
ExtObjectsRoot= `root-config --glibs` -lTools 


%.o: %.cc %.h
	$(CXX) -std=c++11 $(DevFlags) $(CCFlags) $(UserCCFlags) $(CCDefines) $(IncludePaths) -c -o $@ $<

# binariesRoot=analyzer fourier
binariesRoot=analyzer 

all: 	clean rootflags $(binariesRoot)

rootflags:
	$(eval CCFlags += $(CCFlagsRoot))
	$(eval ExternalObjects += $(ExtObjectsRoot))

analyzer: noise_01.cc
	$(CXX) -std=c++11 $(CCFlags) -o $@ $< $(IncludePaths) $(ExternalObjects)
	mv $@ ../bin
	
# fourier: Fourier_Test.cc
# 	$(CXX) -std=c++11 $(CCFlags) -o $@ $< $(IncludePaths) $(ExternalObjects)
# 	cp $@ ../bin

.PHONY: clean

clean:
	rm -f $(binaries) *.o