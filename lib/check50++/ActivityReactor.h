#ifndef LIB_CHECK50_ACTIVITYREACTOR_H
#define LIB_CHECK50_ACTIVITYREACTOR_H

#include "Activity.h"
#include "Test.h"

namespace Check50 {

class ExecutionActivityReactor : public Activity::Notifiee {
 public:
  ExecutionActivityReactor(Activity *activity,
                           ActivityManager::Ptr manager,
                           Test::Ptr test)
    : Notifiee(activity), _activity(activity), _manager(manager), _test(test) {}

  void on_status();

 private:
  Activity::Ptr _activity;
  ActivityManager::Ptr _manager;
  Test::Ptr _test;
};

class SourceTransformationActivityReactor : public Activity::Notifiee {
  // TODO!
};

}      /* end namespace Check50 */

#endif /* LIB_CHECK50_ACTIVITYREACTOR_H */