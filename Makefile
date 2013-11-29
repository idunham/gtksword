CXX = g++
GTKFLAGS = `gtk-config --cflags --libs`
SWORDFLAGS = `pkg-config --cflags --libs sword`
BUILDFLAGS = -Os -lz -g


default: cheatah

cheatah:
	${CXX} -o cheatah cheatah.cpp cheatsig.cpp ${CXXFLAGS} ${GTKFLAGS} ${SWORDFLAGS} ${BUILDFLAGS}
