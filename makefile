GPP = g++ -O3 -Wall -std=c++11

UTILS = ../../Utilities

A = main.o
R = pullet16assembler.o
C = codeline.o
H = hex.o
Y = symbol.o
G = globals.o
S = scanner.o
SL = scanline.o
U = utils.o

Aprog: $A $R $C $H $Y $G $S $(SL) $U
	$(GPP) -o Aprog $A $R $C $H $Y $G $S $(SL) $U

main.o: main.h main.cc
	$(GPP) -c main.cc

pullet16assembler.o: pullet16assembler.h pullet16assembler.cc
	$(GPP) -c pullet16assembler.cc

codeline.o: codeline.h codeline.cc
	$(GPP) -c codeline.cc

hex.o: hex.h hex.cc
	$(GPP) -c hex.cc

symbol.o: symbol.h symbol.cc
	$(GPP) -c symbol.cc

globals.o: globals.h globals.cc
	$(GPP) -c globals.cc

scanner.o: $(UTILS)/scanner.h $(UTILS)/scanner.cc
	$(GPP) -c $(UTILS)/scanner.cc

scanline.o: $(UTILS)/scanline.h $(UTILS)/scanline.cc
	$(GPP) -c $(UTILS)/scanline.cc

utils.o: $(UTILS)/utils.h $(UTILS)/utils.cc
	$(GPP) -c $(UTILS)/utils.cc
