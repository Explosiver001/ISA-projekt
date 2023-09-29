GCC = g++
CFLAGS = 
EXE = dhcp-stats

default: main.o optparser.o pcaphandler.o stats.o logger.o
	$(GCC) $(CFLAGS) $? -o $(EXE) -lpcap

clean:
	rm -rf *.o $(EXE)

main.o: main.cpp
	$(GCC) $(CFLAGS) -c $< -o $@

optparser.o: optparser.cpp optparser.h
	$(GCC) $(CFLAGS) -c $< -o $@

pcaphandler.o: pcaphandler.cpp pcaphandler.h
	$(GCC) $(CFLAGS) -c $< -o $@

stats.o: stats.cpp stats.h
	$(GCC) $(CFLAGS) -c $< -o $@

logger.o: logger.cpp logger.h
	$(GCC) $(CFLAGS) -c $< -o $@