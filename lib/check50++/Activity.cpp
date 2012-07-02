#include "Activity.h"

namespace Check50 {

class ActivityManagerReactor : public ActivityManager::Notifiee {
 public:
  void on_last_activity(Activity::Ptr activity);

  ActivityManagerReactor(ActivityManager *manager) : Notifiee(manager) {}
};

void ActivityManagerReactor::on_last_activity(Activity::Ptr activity) {
  // for now, just run the activity
  // TODO: turn this into a threadpool
  // if (this->notifier()->running_activity() < max_concurrent_activities) {
  //   activity->status_is(Activity::executing);
  // } else {
  //   // punt; wait until a running activity ceases
  //   activity->status_is(Activity::waiting);
  // }
  activity->status_is(Activity::executing);
}

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

  if (_notifiee != NULL) {
    _notifiee->on_last_activity(activity);
  }
}

} /* end namespace Check50 */
