SRC_DIR = src/
SRC_EXT_DIR = $(SRC_DIR)external/
LIB_DIR = lib32/
LIB = $(LIB_DIR)steamwm.so
SHARE_DIR = share/
SHARE_DIR_SKINS = $(SHARE_DIR)steam/
SHARE_DIR_SSM = $(SHARE_DIR)steam-skin-manager/

BINARY_NAME = steamskinmanager
FLAGS_S = -std=c++11 -Wall
FLAGS_E = `pkg-config --cflags --libs gtk+-3.0 gtkmm-3.0` `pkg-config --cflags --libs webkitgtk-3.0`


buildall: $(BINARY_NAME).64 $(BINARY_NAME).32 lib $(LIB)
	
build64: $(BINARY_NAME).64 lib $(LIB)
$(BINARY_NAME).64: $(SRC_DIR)main.cpp
	g++ -m64 $(FLAGS_S) $(SRC_DIR)main.cpp -o $(BINARY_NAME).64 $(FLAGS_E)

build32: $(BINARY_NAME).32 lib $(LIB)
$(BINARY_NAME).32: $(SRC_DIR)main.cpp
	g++ -m32 $(FLAGS_S) $(SRC_DIR)main.cpp -o $(BINARY_NAME).32 $(FLAGS_E)

lib: $(LIB_DIR)lib.cpp
	bash $(LIB_DIR)lib.cpp

clean:
	rm -f $(BINARY_NAME).64
	rm -f $(BINARY_NAME).32
	rm -f $(LIB)

run64: $(BINARY_NAME).64 lib $(LIB)
	./$(BINARY_NAME).64
	 
run32: $(BINARY_NAME).32 lib $(LIB)
	 ./$(BINARY_NAME).32

