CXXFLAGS = -Wall -pedantic -g

OBJECTS = Fwk/Exception.o Test.o TestFileReader.o Activity.o ActivityReactor.o Engine.o
          
TARGETS = main

default: $(TARGETS)

main: main.o $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^
	
clean:
	rm -f $(TARGETS) $(OBJECTS) *.o *~ core Fwk/*.o Fwk/*~ Fwk/core
