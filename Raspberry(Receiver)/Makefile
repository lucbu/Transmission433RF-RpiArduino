all: Example
	
Example: RCSwitch.o Receiver.o Example.o
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $+ -o $@ -lwiringPi
	
clean:
	$(RM) *.o Main