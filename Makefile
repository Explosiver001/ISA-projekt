GCC = g++
CFLAGS = 
EXE = dhcp-stats

default: main.o optparser.o pcaphandler.o
	$(GCC) $(CFLAGS) $? -o $(EXE) -lpcap

clean:
	rm -rf *.o

main.o: main.cpp
	$(GCC) $(CFLAGS) -c $< -o $@

optparser.o: optparser.cpp optparser.h
	$(GCC) $(CFLAGS) -c $< -o $@

pcaphandler.o: pcaphandler.cpp pcaphandler.h
	$(GCC) $(CFLAGS) -c $< -o $@