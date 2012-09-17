#include "ActivityReactor.h"

#include <iostream>

namespace Check50 {

  void ExecutionActivityReactor::on_status() {
    Activity::Ptr activity = this->notifier();

    switch (activity->status()) {
    case Activity::ready:
      _manager->last_activity_is(activity);
      break;
    case Activity::waiting:
      // TODO: do we want to do anything here?
      // _manager->dont_forget_about_this(activity);
      break;
    case Activity::executing:
      // TODO: implement threading!
      // boost::thread t = new boost::thread(this->execute);
      execute();
      break;
    case Activity::deleted:
      // TODO: delete from manager's executing set
      // _manager->activity_del(activity->name());
      break;
    default:
      break;
    }
  }

  void ExecutionActivityReactor::execute() {
    std::cout << "Executing test " << _test->name() << std::endl;

    this->notifier()->status_is(Activity::deleted);
  }

} /* end namespace Check50 */