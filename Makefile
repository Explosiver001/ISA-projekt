GCC = g++
CFLAGS = 
EXE = dhcp-stats
HEADERS = optparser.h pcaphandler.h stats.h logger.h
LINK_FLAGS = -lpcap -lncurses


default: main.o optparser.o pcaphandler.o stats.o logger.o
	$(GCC) $(CFLAGS) $? -o $(EXE) $(LINK_FLAGS)

clean:
	rm -rf *.o $(EXE)

main.o: main.cpp $(HEADERS)
	$(GCC) $(CFLAGS) -c $< -o $@

optparser.o: optparser.cpp $(HEADERS)
	$(GCC) $(CFLAGS) -c $< -o $@

pcaphandler.o: pcaphandler.cpp $(HEADERS)
	$(GCC) $(CFLAGS) -c $< -o $@

stats.o: stats.cpp $(HEADERS)
	$(GCC) $(CFLAGS) -c $< -o $@

logger.o: logger.cpp $(HEADERS)
	$(GCC) $(CFLAGS) -c $< -o $@