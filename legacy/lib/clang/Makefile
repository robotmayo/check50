CXX := clang++
LLVMCOMPONENTS := cppbackend
RTTIFLAG := -fno-rtti
CXXFLAGS := $(shell llvm-config --cxxflags) $(RTTIFLAG) -DCLANG_2
LLVMLDFLAGS := $(shell llvm-config --ldflags --libs $(LLVMCOMPONENTS))
DDD := $(shell echo $(LLVMLDFLAGS))
SOURCES = remove-main.cpp rename.cpp

OBJECTS = $(SOURCES:.cpp=.o)
EXES = $(OBJECTS:.o=)
CLANGLIBS = \
    -lclangFrontend \
    -lclangDriver \
    -lclangSerialization \
    -lclangParse \
    -lclangSema \
    -lclangAnalysis \
    -lclangRewrite \
    -lclangAST \
    -lclangLex \
    -lclangBasic \
    -lLLVMMC \
    -lLLVMSupport

all: $(OBJECTS) $(EXES)

%: %.o
	$(CXX) -o $@ $< $(CLANGLIBS) $(LLVMLDFLAGS)

clean:
	-rm -f $(EXES) $(OBJECTS) *~ foobar .c.* .h.* .cpp.*
