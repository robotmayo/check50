// Copyright (c) 2005-2006_2007 David R. Cheriton.  All rights reserved.

#include "Exception.h"
#include <assert.h>
#include <iostream>
#include <signal.h>

Fwk::Exception::Id Fwk::Exception::IdInstance(unsigned long v) {
    switch (v) {
    case _no_exception:
        return _no_exception;
    case _unknown_exception:
        return _unknown_exception;
    case _unknown_type_exception:
        return _unknown_type_exception;
    case _unknown_attr_exception:
        return _unknown_attr_exception;
    case _unknown_delimiter_exception:
        return _unknown_delimiter_exception;
    case _unknown_arg_exception:
        return _unknown_arg_exception;
    case _internal_exception:
        return _internal_exception;
    case _range_exception:
        return _range_exception;
    case _memory_exception:
        return _memory_exception;
    case _storage_exception:
        return _storage_exception;
    case _timeout_exception:
        return _timeout_exception;
    case _name_in_use_exception:
        return _name_in_use_exception;
    case _illegal_name_exception:
        return _illegal_name_exception;
    case _permission_exception:
        return _permission_exception;
    case _no_implementation_exception:
        return _no_implementation_exception;
    case _rpc_exception:
        return _rpc_exception;
    case _rpc_connection_exception:
        return _rpc_connection_exception;
    case _entity_not_found_exception:
        return _entity_not_found_exception;
    case _entity_not_dir_exception:
        return _entity_not_dir_exception;
    case _audit_exception:
        return _audit_exception;
    case _unknown_entity_id_exception:
        return _unknown_entity_id_exception;
    case _entity_id_in_use_exception:
        return _entity_id_in_use_exception;
    case _entity_log_msg_len_exception:
        return _entity_log_msg_len_exception;
    case _entity_log_write_to_const_exception:
        return _entity_log_write_to_const_exception;
    case _errno_exception:
        return _errno_exception;
    case _memory_limit_exceeded_exception:
        return _memory_limit_exceeded_exception;
    case _no_parent_exception:
        return _no_parent_exception;
    default:
        throw Fwk::RangeException("Fwk::Exception::Id");
    }
}

Fwk::Exception::~Exception() {
}

Fwk::Exception::Id Fwk::Exception::id() {
    return _unknown_exception;
}

Fwk::Exception::Id Fwk::UnknownTypeException::id() {
    return _unknown_type_exception;
}

Fwk::Exception::Id Fwk::UnknownAttrException::id() {
    return _unknown_attr_exception;
}

Fwk::Exception::Id Fwk::UnknownDelimiterException::id() {
    return _unknown_delimiter_exception;
}

Fwk::Exception::Id Fwk::UnknownArgException::id() {
    return _unknown_arg_exception;
}

Fwk::Exception::Id Fwk::InternalException::id() {
    return _internal_exception;
}

Fwk::Exception::Id Fwk::RangeException::id() {
    return _range_exception;
}

Fwk::Exception::Id Fwk::MemoryException::id() {
    return _memory_exception;
}

Fwk::Exception::Id Fwk::StorageException::id() {
    return _storage_exception;
}

Fwk::Exception::Id Fwk::TimeoutException::id() {
    return _timeout_exception;
}

Fwk::Exception::Id Fwk::NameInUseException::id() {
    return _name_in_use_exception;
}

Fwk::Exception::Id Fwk::IllegalNameException::id() {
    return _illegal_name_exception;
}

Fwk::Exception::Id Fwk::PermissionException::id() {
    return _permission_exception;
}

Fwk::Exception::Id Fwk::NoImplementationException::id() {
    return _no_implementation_exception;
}

Fwk::Exception::Id Fwk::RpcException::id() {
    return _rpc_exception;
}

Fwk::Exception::Id Fwk::RpcConnectionException::id() {
    return _rpc_connection_exception;
}

Fwk::Exception::Id Fwk::EntityNotFoundException::id() {
    return _entity_not_found_exception;
}

Fwk::Exception::Id Fwk::EntityNotDirException::id() {
    return _entity_not_dir_exception;
}

Fwk::Exception::Id Fwk::AuditException::id() {
    return _audit_exception;
}

Fwk::Exception::Id Fwk::UnknownEntityIdException::id() {
    return _unknown_entity_id_exception;
}

Fwk::Exception::Id Fwk::EntityIdInUseException::id() {
    return _entity_id_in_use_exception;
}

Fwk::Exception::Id Fwk::EntityLogMsgLenException::id() {
    return _entity_log_msg_len_exception;
}

Fwk::Exception::Id Fwk::EntityLogWriteToConstException::id() {
    return _entity_log_write_to_const_exception;
}

Fwk::Exception::Id Fwk::ErrnoException::id() {
    return _errno_exception;
}

Fwk::Exception::Id Fwk::ListException::id() {
    return _list_exception;
}

Fwk::Exception::Id Fwk::MemoryLimitExceededException::id() {
    return _memory_limit_exceeded_exception;
}
