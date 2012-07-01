#ifndef LIB_CHECK50_TESTFILEREADER_H
#define LIB_CHECK50_TESTFILEREADER_H

#include "Fwk/Ptr.h"
#include "Fwk/PtrInterface.h"

#include "Test.h"

namespace Check50 {

class TestFileReader : public Fwk::PtrInterface<TestFileReader> {
public:
    typedef Fwk::Ptr<TestFileReader> Ptr;

    std::string name() const { return _name; }

    virtual Test::Ptr test_new(const std::string& test_filename) = 0;

protected:
    TestFileReader(const std::string& name) : _name(name) {}
    TestFileReader(const TestFileReader&);

private:
    std::string _name;

};

class YamlTestFileReader : public TestFileReader {
public:
    typedef Fwk::Ptr<YamlTestFileReader> Ptr;

    Test::Ptr test_new(const std::string& test_filename);

    static YamlTestFileReader::Ptr YamlTestFileReaderNew(const std::string& name) {
        Ptr ytfr = new YamlTestFileReader(name);
        return ytfr;
    }

protected:
    YamlTestFileReader(const std::string& name) : TestFileReader(name) {}
    YamlTestFileReader(const YamlTestFileReader&);
};

}

#endif /* LIB_CHECK50_TESTFILEREADER_H */