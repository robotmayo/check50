// Framework Exception types
// Copyright(c) 1993-2006_2007, David R. Cheriton, all rights reserved.

#ifndef LIB_CHECK50_FWK_EXCEPTION_H
#define LIB_CHECK50_FWK_EXCEPTION_H

#include <iostream>
#include <string.h>

#include "Ptr.h"
#include "PtrInterface.h"

// don't want to see strerror warnings, assume we won't get buffer overflowed
#ifdef _MSC_VER
#pragma warning(disable: 4996)
#endif

namespace Fwk {

using std::string;

class Exception {
public:

    enum Id { // Enum for remote designation
        _no_exception = 0,
        _unknown_exception = 1,
        _unknown_type_exception = 2,
        _unknown_attr_exception = 3,
        _unknown_delimiter_exception = 4,
        _unknown_arg_exception = 5,
        _internal_exception = 6,
        _range_exception = 7,
        _memory_exception = 8,
        _storage_exception = 9,
        _timeout_exception = 10,
        _name_in_use_exception = 11,
        _illegal_name_exception = 12,
        _permission_exception = 13,
        _no_implementation_exception = 14,
        _rpc_exception = 15,
        _rpc_connection_exception = 16,
        _entity_not_found_exception = 17,
        _entity_not_dir_exception = 18,
        _audit_exception = 19,
        _unknown_entity_id_exception = 20,
        _entity_id_in_use_exception = 21,
        _entity_log_msg_len_exception = 22,
        _entity_log_write_to_const_exception = 23,
        _errno_exception = 24,
        _list_exception = 25,
        _memory_limit_exceeded_exception = 26,
        _no_parent_exception = 27
    };

    static Id IdInstance(unsigned long v);

    string what() const {
        return _what;
    }

    // Providing a virtual destructor enables us to catch an Exception, and
    // then dynamic_cast it to a derived exception type or fetch its typeid,
    // etc.
    virtual ~Exception();

    virtual Id id();

protected:
    Exception(char const * str) :
        _what(str) {
    }

    Exception(string str) :
        _what(str) {
    }

private:
    string _what;
};

std::ostream & operator<<(std::ostream &, Exception const &);

class UnknownTypeException: public Exception {
public:
    UnknownTypeException(string what) :
        Exception(what) {
    }

    virtual Id id();
};

class UnknownAttrException: public Exception {
public:
    UnknownAttrException(string what) :
        Exception(what) {
    }

    virtual Id id();
};

class UnknownDelimiterException: public Exception {
public:
    UnknownDelimiterException(string what) :
        Exception(what) {
    }

    virtual Id id();
};

class UnknownArgException: public Exception {
public:
    UnknownArgException(string what) :
        Exception(what) {
    }

    virtual Id id();
};

class InternalException: public Exception {
public:
    InternalException(string what) :
        Exception(what) {
    }

    virtual Id id();
};

class RangeException: public Exception {
public:
    RangeException(string what) :
        Exception(what) {
    }

    virtual Id id();
};

class MemoryException: public Exception {
public:
    MemoryException(string what) :
        Exception(what) {
    }

    virtual Id id();
};

class StorageException: public Exception {
public:
    StorageException(string what) :
        Exception(what) {
    }

    virtual Id id();
};

class TimeoutException: public Exception {
public:
    TimeoutException(string what) :
        Exception(what) {
    }

    virtual Id id();
};

class NameInUseException: public Exception {
public:
    NameInUseException(string what) :
        Exception(what) {
    }

    virtual Id id();
};

class IllegalNameException: public Exception {
public:
    IllegalNameException(string what) :
        Exception(what) {
    }

    virtual Id id();
};

class PermissionException: public Exception {
public:
    PermissionException(string what) :
        Exception(what) {
    }

    virtual Id id();
};

class NoImplementationException: public Exception {
public:
    NoImplementationException(string what /*, bool debug=true */) :
        Exception(what /*, debug */) {
    }

    virtual Id id();
};

class AttributeNotSupportedException: public NoImplementationException {
public:
    // Sets debug to false so we don't drop into the debugger when
    // python attempts to access unimplemented attributes.
    AttributeNotSupportedException(string what) :
        NoImplementationException(what /*, false*/) {
    }

    // necessary since NoImplementationException provides one?
    virtual Id id();
};

class RpcException: public Exception {
public:
    RpcException(string what) :
        Exception(what) {
    }

    virtual Id id();
};

class RpcConnectionException: public RpcException {
public:
    RpcConnectionException(string what) :
        RpcException(what) {
    }

    virtual Id id();
};

class EntityNotFoundException: public Exception {
public:
    EntityNotFoundException(string what) :
        Exception(what) {
    }

    virtual Id id();
};

class EntityNotDirException: public Exception {
public:
    EntityNotDirException(string what) :
        Exception(what) {
    }

    virtual Id id();
};

class EntityLogRecordException: public Exception {
public:
    // Throw if ptr to eid resolution during entitylog marshaling
    EntityLogRecordException(string what) :
        Exception(what) {
    }

    virtual Id id();
};

class EntityLogEntityException: public Exception {
public:
    EntityLogEntityException(string what) :
        Exception(what) {
    }

    virtual Id id();
};

class MountException: public Exception {
public:
    MountException(string what) :
        Exception(what) {
    }

    virtual Id id();
};

class AuditException: public InternalException {
public:
    AuditException(string what) :
        InternalException(what) {
    }

    virtual Id id();
};

class UnknownEntityIdException: public InternalException {
public:
    UnknownEntityIdException(string what) :
        InternalException(what) {
    }

    virtual Id id();
};

class EntityIdInUseException: public InternalException {
public:
    EntityIdInUseException(string what) :
        InternalException(what) {
    }

    virtual Id id();
};

class EntityLogMsgLenException: public InternalException {
public:
    EntityLogMsgLenException(string what) :
        InternalException(what) {
    }

    virtual Id id();
};

class EntityLogWriteToConstException: public InternalException {
public:
    EntityLogWriteToConstException(string what) :
        InternalException(what) {
    }

    virtual Id id();
};

class ErrnoException: public Exception {
public:
    ErrnoException(int error, string filename = "") :
        Exception(strerror(error)), errno_(error), filename_(filename) {
    }

    // Can't use errno() because it is a #defined macro in bits/errno.h
    int error() const {
        return errno_;
    }
    
    string filename() const {
        return filename_;
    }

    virtual Id id();

private:
    int errno_;
    string filename_;
};

class ListException: public RangeException {
public:
    ListException(string what) :
        RangeException(what) {
    }

    virtual Id id();
};

class MemoryLimitExceededException: public MemoryException {
public:
    MemoryLimitExceededException(string what) :
        MemoryException(what) {
    }

    virtual Id id();
};

class NoParentException: public Exception {
public:
    NoParentException(string what) :
        Exception(what) {
    }

    virtual Id id();
};

}

#endif /* LIB_CHECK50_FWK_EXCEPTION_H */
