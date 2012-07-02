#ifndef LIB_CHECK50_SOURCECODE_H
#define LIB_CHECK50_SOURCECODE_H

#include "Fwk/Ptr.h"
#include "Fwk/PtrInterface.h"

namespace Check50 {

class SourceCode : public Fwk::PtrInterface<SourceCode> {
 public:
  typedef Fwk::Ptr<SourceCode> Ptr;

  std::string name() const { return _name; }

  static SourceCode::Ptr SourceCodeNew(std::string& name) {
    Ptr sc = new SourceCode(name);
    return sc;
  }

 protected:
  SourceCode(std::string& name) : _name(name) {}
  SourceCode(const SourceCode&);

 private:
  std::string _name;

};

}      /* end namespace Check50 */

#endif /* LIB_CHECK50_SOURCECODE_H */