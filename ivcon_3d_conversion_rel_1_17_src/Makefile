# Project: ivcon
# Makefile for Linux compile of IVCon
# Should work under Windows too

OBJECTFILES = gmod.o hrc.o ivconv.o iv.o obj.o pov.o smf.o tds.o tec.o tmat.o tri.o txt.o ucd.o utils.o vla.o xgl.o asc.o ase.o byu.o dxf.o stl.o wrl.o swv.o sw_vehrd.o swm.o sw_maprd.o

SOURCEFILES = gmod.cpp hrc.cpp ivconv.cpp iv.cpp obj.cpp pov.cpp smf.cpp tds.cpp tec.cpp tmat.cpp tri.cpp txt.cpp ucd.cpp utils.cpp vla.cpp xgl.cpp asc.cpp ase.cpp byu.cpp dxf.cpp stl.cpp wrl.cpp swv.cpp sw_vehrd.cpp swm.cpp sw_maprd.cpp


ivcon: $(OBJECTFILES) main.cpp
	g++  $(OBJECTFILES)  main.cpp -o ivcon 
clean: 
	rm -f *.o ivcon


gmod.o: gmod.cpp
	g++ -c gmod.cpp
hrc.o: hrc.cpp
	g++ -c hrc.cpp
ivconv.o: ivconv.cpp
	g++ -c ivconv.cpp
iv.o: iv.cpp
	g++ -c iv.cpp
obj.o: obj.cpp
	g++ -c obj.cpp
pov.o: pov.cpp
	g++ -c pov.cpp
smf.o: smf.cpp
	g++ -c smf.cpp
tds.o: tds.cpp
	g++ -c tds.cpp
tec.o: tec.cpp
	g++ -c tec.cpp
tmat.o: tmat.cpp
	g++ -c tmat.cpp
tri.o: tri.cpp
	g++ -c tri.cpp
txt.o: txt.cpp
	g++ -c txt.cpp
ucd.o: ucd.cpp
	g++ -c ucd.cpp
utils.o: utils.cpp
	g++ -c utils.cpp
vla.o: vla.cpp
	g++ -c vla.cpp
xgl.o: xgl.cpp
	g++ -c xgl.cpp
asc.o: asc.cpp
	g++ -c asc.cpp
ase.o: ase.cpp
	g++ -c ase.cpp
byu.o: byu.cpp
	g++ -c byu.cpp
dxf.o: dxf.cpp
	g++ -c dxf.cpp
stl.o: stl.cpp
	g++ -c stl.cpp
wrl.o: wrl.cpp
	g++ -c wrl.cpp
swv.o: swv.cpp
	g++ -c swv.cpp
sw_vehrd.o: sw_vehrd.cpp
	g++ -c sw_vehrd.cpp
swm.o: swm.cpp
	g++ -c swm.cpp
sw_maprd.o: sw_maprd.cpp
	g++ -c sw_maprd.cpp
