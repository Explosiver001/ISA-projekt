GCC = g++
CFLAGS = 
EXE = dhcp-stats
SRC_DIR = src/
LINK_FLAGS = -lpcap -lncurses


default: main.o optparser.o pcaphandler.o stats.o logger.o
	$(GCC) $(CFLAGS) $? -o $(EXE) $(LINK_FLAGS)
	rm *.o


clean:
	rm -rf *.o $(EXE)

main.o: $(SRC_DIR)main.cpp $(SRC_DIR)*.h
	$(GCC) $(CFLAGS) -c $< -o $@

optparser.o: $(SRC_DIR)optparser.cpp $(SRC_DIR)*.h
	$(GCC) $(CFLAGS) -c $< -o $@

pcaphandler.o: $(SRC_DIR)pcaphandler.cpp $(SRC_DIR)*.h
	$(GCC) $(CFLAGS) -c $< -o $@

stats.o: $(SRC_DIR)stats.cpp $(SRC_DIR)*.h
	$(GCC) $(CFLAGS) -c $< -o $@

logger.o: $(SRC_DIR)logger.cpp $(SRC_DIR)*.h
	$(GCC) $(CFLAGS) -c $< -o $@

# add documentation
pack:
	tar cvf xnovak3g.tar src/* dhcp-stats.1 LICENSE Makefile README 