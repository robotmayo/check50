#ifndef LIB_CHECK50_FWK_ACTIVITYMANAGER_H
#define LIB_CHECK50_FWK_ACTIVITYMANAGER_H

#include "Activity.h"

namespace Fwk {

class ActivityManager : public Fwk::PtrInterface<ActivityManager> {
public:
    typedef Fwk::Ptr<ActivityManager> Ptr;

    virtual std::string name() const { return _name; }

    enum Mode {

    }

    virtual Activity::Ptr activity_new(const std::string &name) = 0;

    virtual Activity::Ptr activity(const std::string &name) const = 0;

    virtual void activity_del(const std::string &name) = 0;

    virtual void last_activity_is(Activity::Ptr) = 0;

protected:
    ActivityManager(const std::string& name) : _name(name) {}
    ActivityManager(const ActivityManager&);

private:
    std::string _name;
    map<string, Activity::Ptr> _activity;
};

}

#endif /* LIB_CHECK50_FWK_ACTIVITYMANAGER_H */