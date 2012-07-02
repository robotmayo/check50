#include "Test.h"

#include <string>

namespace Check50 {

Test::Test(const std::string& name):
    _name(name),
    _notifiee(NULL),
    _source_code(NULL),
    _compile_directive(NULL),
    _execution_directive(NULL),
    _possible(0.0),
    _output(NULL),
    _received(0.0),
    _explanation(NULL) {}

void
Test::output_is(const std::string& output)
{
    _output = output;

    if (_notifiee != NULL) {
        // notifiee will scrape output
        // and figure everything out
        _notifiee->on_output(_output);
    }
}

void
Test::received_is(Points received)
{
    _received = received;

    if (_explanation.empty() && _notifiee != NULL) {
        _notifiee->on_complete();
    }
}

void
Test::explanation_is(const std::string& explanation)
{
    _explanation = explanation;

    // TODO: this won't work because _received is always true
    // need to change somehow (different attribute?)
    if (_received.value() != 0.0 && _notifiee != NULL) {
        _notifiee->on_complete();
    }
}

} /* end namespace */