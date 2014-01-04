ROOT_CFLAGS = $(shell root-config --cflags)
ROOT_LIBS = $(shell root-config --libs)
CXX = g++

PREFIX = GET
SRCDIR = src
INCDIR = include
TARGET = GETDecoder

LIBNAME = lib$(TARGET)
SOURCE = $(wildcard $(SRCDIR)/$(PREFIX)*.cc)
HEADER = $(wildcard $(INCDIR)/$(PREFIX)*.hh)
DICT = $(LIBNAME)Dict.cc
LINKDEF = $(LIBNAME)LinkDef.hh
FLAGS = -I./$(INCDIR) $(ROOT_CFLAGS)

$(LIBNAME).so: $(SOURCE:.cc=.o) $(DICT:.cc=.o)
	$(CXX) --shared -fPIC -o $@ $(ROOT_LIBS) $^

$(DICT): $(HEADER) $(LINKDEF)
	rootcint -f $@ -c $^
	$(CXX) -c -fPIC $@ $(FLAGS)

$(LINKDEF):
	@echo "" > LinkdefSpace
	@echo "#ifdef __CINT__" > LinkdefHeader1
	@echo "#pragma link off all globals;" > LinkdefHeader2
	@echo "#pragma link off all classes;" > LinkdefHeader3
	@echo "#pragma link off all functions;" > LinkdefHeader4
	@$(shell ls $(INCDIR) | grep ^$(PREFIX) | grep hh | awk -F. {'printf("#pragma link C++ class %s+;\n", $$1)'} > LinkdefBody)
	@echo "#endif" > LinkdefFooter
	@cat LinkdefHeader1 LinkdefSpace LinkdefHeader2 LinkdefHeader3 LinkdefHeader4 LinkdefBody LinkdefSpace LinkdefFooter > $@
	@rm -rf LinkdefSpace LinkdefHeader* LinkdefBody LinkdefFooter

clean:
	@rm -rf $(DICT:.cc=.*)
	@rm -rf *.o $(SRCDIR)/*.o
	@rm -rf $(LINKDEF)
	@rm -rf $(LIBNAME).*

%.o: %.cc
	$(CXX) -o $@ -c -fPIC $(FLAGS) $<
