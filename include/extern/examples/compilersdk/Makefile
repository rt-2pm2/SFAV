#
# file : Makefile (UNIX)
#
#  Example of a stand-alone makefile for compiler-generated M files
#
#  Copyright 1997-2005 The MathWorks, Inc.
#

#
# You can invoke this Makefile using 
#  gmake -f Makefile MATLABROOT=[directory where MATLAB is installed]
#
# If you do not want to specify the MATLABROOT at the gmake command line 
# every single time, you can define it by uncommenting the line below
# and assigning the correct root of MATLAB (with no trailing '/') on
# the right hand side.
#
# MATLABROOT	:=
#

#
# Defaults
#

MCC=$(MATLABROOT)/bin/mcc
MBUILD=$(MATLABROOT)/bin/mbuild

C_TARGETS=matrixdriver triangle

CPP_TARGETS=matrixdriverp trianglep

STAND_ALONE_TARGETS=$(C_TARGETS) $(CPP_TARGETS)


# Use the following target to build and test all the executables
bnt_exes: build_all
	@echo "*********** testing matrixdriver ***********"
	./matrixdriver 
	@echo "*********** testing triangle ***********"
	./triangle 
	@echo "*********** testing trianglep ***********"
	./trianglep 
	@echo "*********** testing matrixdriverp ***********"
	./matrixdriverp
	

# Use this target to only build all the targets
build_all: $(STAND_ALONE_TARGETS)

# Use this target to only build all the C++ targets
only_cpp: $(CPP_TARGETS)

# Use this target to only build the C targets
only_c: $(C_TARGETS)


# The following are the definitions for each target individually.

# This command will create the wrapper files and then compile all the source files
# including the wrapper code and the user supplied C/C++ code into a single executable
# binary. No shared library is created in this process.  

triangle:  triangle.c main_for_lib.c main_for_lib.h sierpinski.m
	$(MCC)  -l -o libtriangle sierpinski.m
	$(MBUILD) triangle.c main_for_lib.c  -L. -ltriangle
	@rm -f *.o*


# This command will create a shared library first and then link matrixdriver.c with the 
# resulting library. See the comments for the triangle target
# above for more details.

matrixdriver:  matrixdriver.c addmatrix.m eigmatrix.m multiplymatrix.m
	$(MCC)  -l -o libmatrix addmatrix.m eigmatrix.m multiplymatrix.m
	$(MBUILD) matrixdriver.c -L. -lmatrix
	@rm -f *.o*

# The next two targets are for the CPP shared library target example
trianglep:  triangle.cpp main_for_lib.c main_for_lib.h  sierpinski.m
	$(MCC) -B cpplib:libtrianglep sierpinski.m
	$(MBUILD) triangle.cpp main_for_lib.c -L. -ltrianglep -output trianglep
	@rm -f *.o*

matrixdriverp:  matrixdriver.cpp addmatrix.m eigmatrix.m multiplymatrix.m
	$(MCC) -B cpplib:libmatrixp addmatrix.m eigmatrix.m multiplymatrix.m
	$(MBUILD) matrixdriver.cpp -L. -lmatrixp -output matrixdriverp
	@rm -f *.o*

clean:
	rm -f $(STAND_ALONE_TARGETS)

reallyclean:
	rm -rf $(STAND_ALONE_TARGETS)				\
		magicsquare.h magicsquare.c magicsquare_main.c	\
		libPkg* libMult* libtriangle* libmatrix*	\
		*.xml *.ctf *_mcr 	\
		*_main.c *.exports lib*                         \
		collect_one_external.h mccExcludedFIles.log  \
		output	
