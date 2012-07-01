#include "ExecutionManager.h"

namespace Check50 {

ExecutionManager::ExecutionManager(const string& name,
    ExecutionManager::Type type):
    _name(name),
    _type(type) {}

LocalExecutionManager::LocalExecutionManager(const string& name):
    ExecutionManager::ExecutionManager(name, ExecutionManager::_local) {}

RemoteExecutionManager::RemoteExecutionManager(const string& name):
    ExecutionManager::ExecutionManager(name, ExecutionManager::_remote {}
}