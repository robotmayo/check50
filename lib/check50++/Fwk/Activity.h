#ifndef LIB_CHECK50_FWK_ACTIVITY_H
#define LIB_CHECK50_FWK_ACTIVITY_H

#include <string>

#include "Nominal.h"
#include "Notifiee.h"

#include "Ptr.h"
#include "PtrInterface.h"

namespace Fwk {

class ActivityManager;

class Activity : public Fwk::PtrInterface<Activity> {
public:
    typedef Fwk::Ptr<Activity> Ptr;

    virtual std::string name() const { return _name; }

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

    class Notifiee: public Fwk::BaseNotifiee<Activity> {
    public:
        typedef Fwk::Ptr<Notifiee> Ptr;

        virtual void on_status_is(Status s) {}

        Notifiee(Activity* activity) : Fwk::BaseNotifiee<Activity>(activity) {}
    };
    virtual Fwk::Ptr<Notifiee> notifiee() const { return _notifiee; }
    virtual void last_notifiee_is(Notifiee* n) {
        Activity *me = const_cast<Activity *>(this);
        me->_notifiee = n;
    }

    virtual Status status() const { return _status; }
    virtual void status_is(Status s) {
        _status = s;
        if (_notifiee != NULL) {
            _notifiee->on_status_is(_status);
        }
    }

    ActivityManager *activity_manager() const { return _activity_manager; }

protected:
    Activity(const std::string &name, ActivityManager *activity_manager):
        _name(name),
        _activity_manager(manager),
        _notifiee(NULL),
        _status(_free) {}

    Activity(const Activity&);

private:
    std::string _name;
    ActivityManager *_activity_manager;
    Notifiee *_notifiee;
    Status _status;
};

}

#endif /* LIB_CHECK50_FWK_ACTIVITY_H */
