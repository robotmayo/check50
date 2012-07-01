#ifndef LIB_CHECK50_EXECUTIONMANAGER_H
#define LIB_CHECK50_EXECUTIONMANAGER_H

#include "Fwk/ActivityManager.h"
#include "Fwk/Ptr.h"
#include "Fwk/PtrInterface.h"

#include "ExecutionActivity.h"

namespace Check50 {

using std::string;

class ExecutionManager : public Fwk::PtrInterface<ExecutionManager> {
public:
    typedef Fwk::Ptr<ExecutionManager> Ptr;

    enum Type {
        _remote,
        _local
    };
    static inline Type remote() { return _remote; }
    static inline Type local() { return _local; }
    Type type() const { return _type; }

protected:
    ExecutionManager(const string& name, ExecutionManagerType type);
    ExecutionManager(const ExecutionManager&);

private:
    string _name;
    Type _type;
};

class LocalExecutionManager : public Fwk::PtrInterface<LocalExecutionManager> {
public:
    typedef Fwk::Ptr<LocalExecutionManager> Ptr;

    static LocalExecutionManager::Ptr LocalExecutionManagerNew(const string& name) {
        Ptr lem = new LocalExecutionManager(name);
        return lem;
    }

protected:
    LocalExecutionManager(const string& name);
    LocalExecutionManager(const LocalExecutionManager&);
};

class RemoteExecutionManager : public Fwk::PtrInterface<RemoteExecutionManager> {
public:
    typedef Fwk::Ptr<LocalExecutionManager> Ptr;

    static RemoteExecutionManager::Ptr RemoteExecutionManagerNew(const string& name) {
        Ptr rem = new RemoteExecutionManager(name);
        return rem;
    }

protected:
    RemoteExecutionManager(const string& name);
    RemoteExecutionManager(const RemoteExecutionManager&);
};

}      /* end namespace */

#endif /* LIB_CHECK50_EXECUTIONMANAGER_H */