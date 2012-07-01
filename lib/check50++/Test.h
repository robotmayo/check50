#ifndef LIB_CHECK50_TEST_H
#define LIB_CHECK50_TEST_H

#include <string>

#include "Fwk/Notifiee.h"
#include "Fwk/Ptr.h"
#include "Fwk/PtrInterface.h"

#include "Types.h"

namespace Check50 {

class Test : public Fwk::PtrInterface<Test> {

public:
    typedef Fwk::Ptr<Test> Ptr;

    std::string name() const { return _name; }

    enum Status {
        _incomplete,
        _complete
    };
    static inline Status incomplete() { return _incomplete; }
    static inline Status complete() { return _complete; }

    class Notifiee : public Fwk::BaseNotifiee<Test> {
    public:
        typedef Fwk::Ptr<Notifiee> Ptr;

        virtual void on_status_is(Status s) {}

        Notifiee(Test *test) : Fwk::BaseNotifiee<Test>(test) {}
    };

    Fwk::Ptr<Notifiee> notifiee() const { return _notifiee; }
    void last_notifiee_is(Notifiee *n) {
        Test *me = const_cast<Test *>(this);
        me->_notifiee = n;
    }

    std::string config_file() const { return _config_filename; }
    virtual void config_filename_is(const std::string& config_filename) {
        _config_filename = config_filename;
    }

    std::string output() const { return _output; }
    virtual void output_is(const std::string& output);

    std::string source_code() const { return _source_code; }
    virtual void source_code_is(const std::string& source_code);

    Status status() const { return _status; }
    virtual void status_is(Status s);

    static Test::Ptr TestNew(const std::string& name) {
        Ptr t = new Test(name);
        return t;
    }

protected:
    Test(const std::string& name);
    Test(const Test&);

private:
    std::string _name;
    Notifiee *_notifiee;

    std::string _config_filename;

    std::string _output;

    Points _received, _possible;
    std::string _explanation;

    std::string _source_code;

    Status _status;
};

}      /* end namespace */

#endif /* LIB_CHECK50_TEST_H */