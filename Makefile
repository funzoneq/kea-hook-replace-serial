
KEA_MSG_COMPILER ?= kea-msg-compiler
KEA_INCLUDE ?= /usr/include/kea
KEA_LIB ?= /usr/lib

OBJECTS = src/load_unload.o src/version.o src/pkt4_send.o
DEPS = $(OBJECTS:.o=.d)
CXXFLAGS = -I $(KEA_INCLUDE) -fPIC -Wno-deprecated -std=c++11
LDFLAGS = -L $(KEA_LIB) -shared -lkea-dhcpsrv -lkea-dhcp++ -lkea-hooks -lkea-log -lkea-util -lkea-exceptions

kea-hook-replace-serial.so: $(OBJECTS)
    $(CXX) -o $@ $(CXXFLAGS) $(LDFLAGS) $(OBJECTS)

%.o: %.cc
    $(CXX) -MMD -MP -c $(CXXFLAGS) -o $@ $<

clean:
    rm -f src/*.o
    rm -f kea-hook-replace-serial.so

-include $(DEPS)
