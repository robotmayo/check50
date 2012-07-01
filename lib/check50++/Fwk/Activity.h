#ifndef LIB_CHECK50_FWK_ACTIVITY_H
#define LIB_CHECK50_FWK_ACTIVITY_H

#include <string>

#include "Notifiee.h"
#include "Ptr.h"
#include "PtrInterface.h"

namespace Fwk {

class Activity: public Fwk::PtrInterface<Activity> {
public:
  typedef Fwk::Ptr<Activity> Ptr;

  class Notifiee: public Fwk::BaseNotifiee<Activity> {
  public:
    typedef Fwk::Ptr<Notifiee> Ptr;

    virtual void on_status(Status s) {}

    Notifiee(Activity *activity) : Fwk::BaseNotifiee<Activity>(activity) {}
  };

  class Manager;

  enum Status {
    _free, 
    _waiting,
    _ready,
    _executing,
    _deleted
  };
  static inline Status free() { return _free; }
  static inline Status waiting() { return _waiting; }
  static inline Status ready() { return _ready; }
  static inline Status executing() { return _executing; }
  static inline Status deleted() { return _deleted; }

  virtual Status status() const = 0;
  virtual void status_is(Status s) = 0;


  virtual Fwk::Ptr<Notifiee> notifiee() const = 0;

  virtual void last_notifiee_is(Notifiee *n) = 0;

  virtual std::string name() const { return _name; }

protected:
  Activity(const std::string &name): _name(name) {}
  Activity(const Activity&);

private:
  std::string _name;
};

class Activity::Manager: public Fwk::PtrInterface<Activity::Manager> {
public:
  typedef Fwk::Ptr<Activity::Manager> Ptr;

  virtual Fwk::Ptr<Activity> activity_new(const std::string &name) = 0;

  virtual Fwk::Ptr<Activity> activity(const std::string &name) const = 0;

  virtual void activity_del(const std::string &name) = 0;

  virtual void last_activity_is(Activity::Ptr) = 0;
};

}      /* end namespace Fwk */

#endif /* LIB_CHECK50_FWK_ACTIVITY_H */
