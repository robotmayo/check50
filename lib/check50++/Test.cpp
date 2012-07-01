#include "Test.h"

namespace Check50 {

using std::string;

Test::Test(const string& name):
    _name(name),
    _notifiee(NULL),
    _config_filename(NULL),
    _output(NULL),
    _received(0.0),
    _possible(0.0),
    _explanation(NULL),
    _source_code(NULL),
    _status(Test::_incomplete) {}


void
Test::output_is(const string& output)
{
    _output = output;

    // parse the output
    // set Points _received
    // set std::string _explanation
    // this->status_is(Test::complete());
}

void
Test::source_code_is(const string& source_code)
{
    _source_code = source_code;

    // if (_precompile != NULL) {
    //     // queue up a Clang precompile activity
    // } else {
    //     this->status_is(Test::ready_for_execution());
    // }
}

void
Test::status_is(Status status)
{
    _status = status;
}

} /* end namespace */