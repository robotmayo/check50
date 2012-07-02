#include "TestFileReader.h"

#include "Fwk/Exception.h"

#include "Test.h"

namespace Check50 {
    
Test::Ptr YamlTestFileReader::test_new(const std::string& test_filename) {
  Test::Ptr test = Test::TestNew(test_filename);

  // parse the YAML test file
  // ensure it has the appropriate extension
  // let's use one of these libraries:
  // 1) http://pyyaml.org/wiki/LibYAML
  // 2) http://code.google.com/p/yaml-cpp/
  //throw new Fwk::NoImplementationException("No implementation yet!");

  // test->compile_directive_is();
  // test->execution_directive_is();
  // test->possible(Points(2));

  return test;
}

}