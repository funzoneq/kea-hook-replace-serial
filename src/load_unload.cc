// load_unload.cc
using namespace isc::hooks;

extern "C" {
    int load(LibraryHandle& handle) {
        return 0;
    }

    int unload() {
        return 0;
    }
}
