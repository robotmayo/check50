#include <stdlib.h>

#include <iostream>

#include "Engine.h"

using namespace std;
using namespace Check50;

int
main(int argc, const char *argv[])
{
    if (argc < 2) {
        cout << "Usage: " << argv[0] << " <source_code> <tests>" << endl;
        exit(1);
    }

    string name = string("engine");
    string source_code = string(argv[1]);

    Engine::EngineMode mode = Engine::development();
    Engine::TestFileFormat format = Engine::yaml();

    Engine::Ptr engine = Engine::EngineNew(name, source_code, mode, format);

    for (int i = 2; i < argc; i++) {
        cout << "Creating test: " << argv[i] << endl;
        //engine->test_new(argv[i]);
    }

    // read results

    return 0;
}