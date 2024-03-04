CC=cc
GSL_PATH ?= /net/ens/renault/save/gsl-2.6/install
CFLAGS = -std=c99 -Wall -Wextra -fPIC -g3 -I$(GSL_PATH)/include -Isrc/ --coverage
LDFLAGS = -lm -lgsl -lgslcblas -ldl -lgcov -L$(GSL_PATH)/lib -L$(GSL_PATH)/lib64 -Wl,--rpath=${GSL_PATH}/lib

all: build test

build: client server 

server: disp.o graph.o rules.o server.o
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)
	cp ./server install
	echo "============ server created succesfully ============"

player.h: src/graph.h src/move.h

server.o: src/server/server.c src/dir.h src/graph.h src/move.h src/player.h src/server.h
	$(CC) $(CFLAGS) -o $@ -c $<

disp.o: src/disp.c src/dir.h src/graph.h src/server.h
	$(CC) $(CFLAGS) -o $@ -c $<

graph.o: src/graph.c src/graph.h src/dir.h
	$(CC) $(CFLAGS) -o $@ -c $<

rules.o: src/server/rules.c src/server.h src/player.h
	$(CC) $(CFLAGS) -o $@ -c $< 

client: The_Watcher.so The_insider.so  The_OutsiderV2.so
	echo "============ libraries created succesfully ============"

### Players

common_player.o: src/client/common_player.c src/player.h src/graph.h src/dir.h src/client/common_player.h
	$(CC) $(CFLAGS) -o $@ -c $<

yooplait.o : src/client/yooplait.c src/client/common_player.h src/graph.h src/move.h src/player.h
	$(CC) $(CFLAGS) -o $@ -c $<

yooplait.so : yooplait.o common_player.o graph.o
	$(CC) $(CFLAGS) -shared -o $@ $^

3bi9a.o : src/client/3bi9a.c src/client/common_player.h src/graph.h src/move.h src/player.h
	$(CC) $(CFLAGS) -o $@ -c $<

3bi9a.so : 3bi9a.o common_player.o graph.o
	$(CC) $(CFLAGS) -shared -o $@ $^

The_Watcher.o : src/client/The_Watcher.c src/client/common_player.h src/graph.h src/move.h src/player.h
	$(CC) $(CFLAGS) -o $@ -c $<

The_Watcher.so : The_Watcher.o common_player.o graph.o
	$(CC) $(CFLAGS) -shared -o $@ $^

The_insider.o : src/client/The_insider.c src/client/common_player.h src/graph.h src/move.h src/player.h
	$(CC) $(CFLAGS) -o $@ -c $<

The_insider.so : The_insider.o common_player.o graph.o
	$(CC) $(CFLAGS) -shared -o $@ $^

The_Outsider.o : src/client/The_Outsider.c src/client/common_player.h src/graph.h src/move.h src/player.h
	$(CC) $(CFLAGS) -o $@ -c $<

The_Outsider.so : The_Outsider.o common_player.o graph.o
	$(CC) $(CFLAGS) -shared -o $@ $^

The_OutsiderV2.o : src/client/The_OutsiderV2.c src/client/common_player.h src/graph.h src/move.h src/player.h
	$(CC) $(CFLAGS) -o $@ -c $<

The_OutsiderV2.so : The_OutsiderV2.o common_player.o graph.o
	$(CC) $(CFLAGS) -shared -o $@ $^

### Tests

test: alltests

alltests: tst_main.o common_player.o disp.o graph.o rules.o tst_disp.o tst_graph.o tst_player.o tst/tst.h
	$(CC) $(CFLAGS) --coverage -O0 $^ -o $@ $(LDFLAGS)

coverage: 
	gcov -abcfnu *.gcda

tst_main.o: tst/tst_main.c tst/tst.h
	$(CC) $(CFLAGS) -o $@ -c $<

tst_disp.o: tst/tst_disp.c src/dir.h src/graph.h src/server.h tst/tst.h
	$(CC) $(CFLAGS) -o $@ -c $<

tst_graph.o: tst/tst_graph.c src/dir.h src/server.h tst/tst.h
	$(CC) $(CFLAGS) -o $@ -c $<

tst_player.o: tst/tst_player.c src/client/common_player.h src/dir.h src/graph.h src/player.h src/server.h tst/tst.h
	$(CC) $(CFLAGS) -Isrc/client -o $@ -c $<  

install: server client alltests
	mkdir -p install
	cp ./alltests install
	cp The_OutsiderV2.so The_Watcher.so The_insider.so install

run:
	./install/server -m 10 -d ./The_insider.so ./The_OutsiderV2.so

clean:
	@rm -f *~ src/*~ *.o install/*.so install/alltests install/server *.gcno *.gcda *.so ./alltests ./server rapport/*.aux rapport/*.fls rapport/*.toc rapport/*.gz rapport/*.lof rapport/*.out rapport/*.log rapport/*.fdb_latexmk

.PHONY: client install test clean

analyse:
	sudo operf ./install/server -m 10 ./The_insider.so ./The_OutsiderV2.so
	opreport --symbols -g
