#ifndef LIB_CHECK50_ACTIVITY_H
#define LIB_CHECK50_ACTIVITY_H

#include <map>
#include <string>
#include <queue>
#include <vector>

#include "Fwk/Notifiee.h"
#include "Fwk/Ptr.h"
#include "Fwk/PtrInterface.h"

#include "Types.h"

namespace Check50 {

class Activity : public Fwk::PtrInterface<Activity> {
 public:
  typedef Fwk::Ptr<Activity> Ptr;

  virtual std::string name() const { return _name; }

  Fwk::Ptr<class ActivityManager> manager() const { return _manager; }

  class Notifiee: public Fwk::BaseNotifiee<Activity> {
   public:
    typedef Fwk::Ptr<Notifiee> Ptr;

    virtual void on_status() {}

    Notifiee(Activity *activity)
      : Fwk::BaseNotifiee<Activity>(activity) {}
  };

  virtual Notifiee::Ptr notifiee() const { return _notifiee; }
  virtual void last_notifiee_is(Notifiee *n) {
    Activity *me = const_cast<Activity *>(this);
    me->_notifiee = n;
  }

  enum Status {
    free, ready, waiting, executing, deleted
  };

  virtual Status status() const { return _status; }
  virtual void status_is(Status s) {
    _status = s;
    if (_notifiee != NULL) {
      _notifiee->on_status();
    }
  }

  virtual Priority priority() const { return _priority; }
  virtual void priority_is(Priority priority) { _priority = priority; }

 protected:
  Activity(const std::string& name, Fwk::Ptr<class ActivityManager> manager)
    : _name(name), _manager(manager), _priority(0.0) {}

  Activity(const Activity&);

 private:
  friend class ActivityManager;
  std::string _name;
  Fwk::Ptr<class ActivityManager> _manager;
  Status _status;
  Priority _priority;
  Notifiee *_notifiee;
};

class ActivityComp :
  public binary_function<Activity::Ptr, Activity::Ptr, bool> {
 public:
  ActivityComp() {}

  bool operator()(Activity::Ptr a, Activity::Ptr b) const {
    return (a->priority() > b->priority());
  }
};

class ActivityManager : public Fwk::PtrInterface<ActivityManager> {
 public:
  typedef Fwk::Ptr<ActivityManager> Ptr;

  class Notifiee : public Fwk::BaseNotifiee<ActivityManager> {
   public:
    typedef Fwk::Ptr<Notifiee> Ptr;

    virtual void on_last_activity(Activity::Ptr activity) {}

    Notifiee(ActivityManager *manager)
      : Fwk::BaseNotifiee<ActivityManager>(manager) {}
  };

  virtual Notifiee::Ptr notifiee() const { return _notifiee; }
  virtual void last_notifiee_is(Notifiee *n) {
    ActivityManager *me = const_cast<ActivityManager *>(this);
    me->_notifiee = n;
  }

  virtual Activity::Ptr activity_new(const std::string& name);
  virtual Activity::Ptr activity(const std::string& name) const;
  virtual void activity_del(const std::string& name);
  virtual void last_activity_is(Activity::Ptr activity);

  static ActivityManager::Ptr ActivityManagerNew() {
    Ptr am = new ActivityManager();
    return am;
  }

 protected:
  ActivityManager() {}
  ActivityManager(const ActivityManager&);

 private:
  std::priority_queue<Activity::Ptr, std::vector<Activity::Ptr>, ActivityComp> _scheduled_activity;
  std::map<std::string, Activity::Ptr> _activity;
  Notifiee *_notifiee;

};

}      /* end namespace Check50 */

#endif /* LIB_CHECK50_ACTIVITY_H */
