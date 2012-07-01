#include "Engine.h"

#include <map>

#include "Fwk/Exception.h"

#include "Test.h"
#include "TestFileReader.h"

namespace Check50 {

Engine::Engine(const std::string& name,
               std::string& source_code,
               Engine::EngineMode engine_mode,
               Engine::TestFileFormat test_file_format):
    _name(name),
    _source_code(source_code),    
	_notifiee(NULL),
    _engine_mode(engine_mode),
    _test_file_format(test_file_format)
{
    // TODO: prob need to do something more sophisticated with source_code

    if (test_file_format == Engine::yaml()) {
        _test_file_reader = YamlTestFileReader::YamlTestFileReaderNew("name");   
    }
}

Test::Ptr
Engine::test_is(const std::string& test_filename)
{
    if (!_test[test_filename]) {
        if (_test_file_reader == NULL) {
            // uh-oh...this can't be good
        }

        Test::Ptr test;
        
        try {
            test = _test_file_reader->test_new(test_filename);
        } catch (Fwk::Exception e) {
            // TODO: fix what we're catching here
        }

        test->source_code_is(source_code);

        _test[test_filename] = test;
    }

    return _test[test_filename];
}

Test::Ptr
Engine::test_new(const std::string& test_filename)
{
    if (_test[test_filename]) {
        throw Fwk::NameInUseException("Already have \"" + test_filename + "\"");
    }

    return NULL;
}

void
Engine::test_del(const std::string& name)
{
    std::map<std::string, Test::Ptr>::iterator it = _test.find(name);
    if (it == _test.end()) return;

    _test.erase(it);
}

} /* end namespace */
