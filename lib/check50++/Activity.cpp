#include "Activity.h"

namespace Check50 {

Activity::Ptr ActivityManager::activity_new(const std::string& name) {
  Activity::Ptr activity = _activity[name];

  if (activity != NULL) {
    /* TODO: throw error? */
    return NULL;
  }

  activity = new Activity(name, this);

  _activity[name] = activity;

  return activity;
}

Activity::Ptr ActivityManager::activity(const std::string& name) const {
  // can't do return _activity[name]; b/c of const 
  std::map<std::string, Activity::Ptr>::const_iterator it = _activity.find(name);

  if (it != _activity.end()) {
    return (*it).second;
  }

  return NULL;
}

void ActivityManager::activity_del(const std::string& name) {
  _activity.erase(name);
}

void  ActivityManager::last_activity_is(Activity::Ptr activity) {
  _scheduled_activity.push(activity);
}

/*void
ManagerImpl::nowIs(Time t) {
  while (!scheduledActivities_.empty()) {

    Activity::Ptr nextToRun = scheduledActivities_.top();

    if (nextToRun->nextTime() > t) {
      break;
    }

    if (managerType_ == ManagerImpl::realTime()) {
      Time diff = Time(nextToRun->nextTime().value() - now_.value());
      usleep((int)(diff.value() * (scalingFactor_.value() * 1000)));
    }

    now_ = nextToRun->nextTime();

    scheduledActivities_.pop();

    nextToRun->statusIs(Activity::executing);
    nextToRun->statusIs(Activity::free);

  }

  now_ = t;
} */

} /* end namespace Check50 */
