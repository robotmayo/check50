#ifndef LIB_CHECK50_ENGINE_H
#define LIB_CHECK50_ENGINE_H

#include <string>
#include <map>

#include "Fwk/Notifiee.h"
#include "Fwk/Ptr.h"
#include "Fwk/PtrInterface.h"

#include "Activity.h"
#include "SourceCode.h"
#include "Test.h"
#include "TestFileReader.h"

namespace Check50 {

class Engine : public Fwk::PtrInterface<Engine> {
 public:
  typedef Fwk::Ptr<Engine> Ptr;

  std::string name() const { return _name; }

  // the Engine's notifiee wants to hear about new test results
  // TODO: figure out if this is indeed the best way
  class Notifiee : public Fwk::BaseNotifiee<Engine> {
  public:
    typedef Fwk::Ptr<Notifiee> Ptr;

    virtual void on_test_complete(Test::Ptr test);

    Notifiee(Engine *engine) : Fwk::BaseNotifiee<Engine>(engine) {}
  };

  Fwk::Ptr<Notifiee> notifiee() { return _notifiee; }
  void last_notifiee_is(Notifiee *n) {
    Engine *me = const_cast<Engine *>(this);
    me->_notifiee = n;
  }

  // TODO: are these necessary?
  enum EngineMode {
    development,
    production
  };
  EngineMode engine_mode() const { return _engine_mode; }
  void engine_mode_is(EngineMode engine_mode) {
    _engine_mode = engine_mode;
  }

  SourceCode::Ptr source_code() const { return _source_code; }

  // TODO: what other formats do we want to support?
  enum TestFileFormat {
    yaml
    /* add other formats here and a corresponding static accessor below */
  };
  TestFileFormat test_file_format() const { return _test_file_format; }


  Test::Ptr test(const std::string& test_filename) const;
  unsigned int tests() const { return _test.size(); }

  // test_is ensures test exists, test_new throws an exception if it does.
  // TODO: are they both necessary?
  Test::Ptr test_is(const std::string& test_filename);
  Test::Ptr test_new(const std::string& test_filename);
 
  // TODO: is this necessary?
  void test_del(const std::string& test_filename);

  // this is the general way to create an Engine
  static Engine::Ptr EngineNew(const std::string& name,
                               std::string& source_code_file,
                               EngineMode& engine_mode,
                               TestFileFormat& test_file_format) {
    Ptr e = new Engine(name, source_code_file, engine_mode, test_file_format);
    return e;
  }

 protected:
  // defined in Engine.cpp
  Engine(const std::string& name, std::string& source_code_file,
         EngineMode engine_mode, TestFileFormat test_file_format);

  // leave undefined so that it can't be copied
  Engine(const Engine&);

 private:
  std::string _name;
  SourceCode::Ptr _source_code;
  Notifiee *_notifiee;
  EngineMode _engine_mode;
  TestFileFormat _test_file_format;
  TestFileReader::Ptr _test_file_reader;
  std::map<std::string, Test::Ptr> _test;
  ActivityManager::Ptr _activity_manager;
};

} /* end namespace */

#endif /* LIB_CHECK50_ENGINE_H */
