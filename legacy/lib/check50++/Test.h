#ifndef LIB_CHECK50_TEST_H
#define LIB_CHECK50_TEST_H

#include <string>

#include "Fwk/Notifiee.h"
#include "Fwk/Ptr.h"
#include "Fwk/PtrInterface.h"

#include "SourceCode.h"
#include "Types.h"

namespace Check50 {

class Test : public Fwk::PtrInterface<Test> {
 public:
  typedef Fwk::Ptr<Test> Ptr;

  std::string name() const { return _name; }

  class Notifiee : public Fwk::BaseNotifiee<Test> {
  public:
    typedef Fwk::Ptr<Notifiee> Ptr;

    virtual void on_output(const std::string& output) {}
    virtual void on_complete() {}

    Notifiee(Test *test) : Fwk::BaseNotifiee<Test>(test) {}
  };

  Fwk::Ptr<Notifiee> notifiee() const { return _notifiee; }
  void last_notifiee_is(Notifiee *n) {
    Test *me = const_cast<Test *>(this);
    me->_notifiee = n;
  }

  SourceCode::Ptr source_code() const { return _source_code; }
  virtual void source_code_is(const SourceCode::Ptr source_code) {
    _source_code = source_code;
  }

  std::string compile_directive() const { return _compile_directive; }
  virtual void compile_directive_is(const std::string& compile_directive) {
    _compile_directive = compile_directive;
  }

  std::string execution_directive() const { return _execution_directive; }
  virtual void execution_directive_is(const std::string& execution_directive) {
    _execution_directive = execution_directive;
  }

  Points possible() const { return _possible; }
  virtual void possible_is(Points possible) {
    _possible = possible;
  }

  std::string output() const { return _output; }
  virtual void output_is(const std::string& output);

  Points received() const { return _received; }
  virtual void received_is(Points received);

  std::string explanation() const { return _explanation; }
  virtual void explanation_is(const std::string& explanation);

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

  SourceCode::Ptr _source_code;

  // the following are read from the file
  std::string _compile_directive;
  std::string _execution_directive;
  Points _possible;

  // the following are determined by the test's execution
  std::string _output;
  Points _received;
  std::string _explanation;
};

}      /* end namespace Check50 */

#endif /* LIB_CHECK50_TEST_H */