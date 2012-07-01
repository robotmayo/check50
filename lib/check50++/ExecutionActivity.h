#ifndef LIB_CHECK50_EXECUTIONACTIVITY_H
#define LIB_CHECK50_EXECUTIONACTIVITY_H

#include "Fwk/Activity.h"

namespace Check50 {

class ExecutionActivity : public Fwk::Activity {
public:
    typedef Fwk::Ptr<ExecutionActivity> Ptr;

    Priority priority() const { return _priority; }

private:
    Priority _priority;
};

}

#endif /* LIB_CHECK50_EXECUTIONACTIVITY_H */