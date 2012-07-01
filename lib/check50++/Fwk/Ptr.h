#ifndef LIB_CHECK50_FWK_PTR_H
#define LIB_CHECK50_FWK_PTR_H

namespace Fwk {

template <class T>
class Ptr
{
public:
    Ptr(T* p = 0) : _ptr(p) { if (_ptr) _ptr->new_ref(); }
    Ptr(const Ptr<T>& mp) : _ptr(mp._ptr) { if (_ptr) _ptr->new_ref(); }
    ~Ptr() { if (_ptr) _ptr->delete_ref(); }

    Ptr<T>& operator=( const Ptr<T>& mp );
    Ptr<T>& operator=( Ptr<T>& mp );
    Ptr<T>& operator=( T* p );

    bool operator==( const Ptr<T>& mp ) const { return _ptr == mp._ptr; }
    bool operator!=( const Ptr<T>& mp ) const { return _ptr != mp._ptr; }
    bool operator==( T* p ) const { return _ptr == p; }
    bool operator!=( T* p ) const { return _ptr != p; }

    const T * operator->() const { return _ptr; }
    T * operator->() { return _ptr; }
    T * ptr() const { return _ptr; }

    template <class OtherType>
    operator Ptr<OtherType>() const { return Ptr<OtherType>( _ptr ); }

    struct PointerConversion { int valid; };
    operator int PointerConversion::*() const {
        return _ptr ? &PointerConversion::valid : 0;
    }

protected:
    T *_ptr;
};

template <class T> Ptr<T>&
Ptr<T>::operator=( const Ptr<T>& mp ) {
    const T * save = _ptr;
    _ptr = mp._ptr; 
    if( _ptr ) _ptr->new_ref();
    if( save ) save->delete_ref();
    return *this;
}

template <class T> Ptr<T>&
Ptr<T>::operator=( Ptr<T>& mp ) {
    T * save = _ptr;
    _ptr = mp._ptr; 
    if( _ptr ) _ptr->new_ref();
    if( save ) save->delete_ref();
    return *this;
}

template <class T> Ptr<T>&
Ptr<T>::operator=( T* p ) {
    T * save = _ptr;
    _ptr = p;
    if( _ptr ) _ptr->new_ref();
    if( save ) save->delete_ref();
    return *this;
}

template <class T, class U>
Ptr<T> _ptrcast(Ptr<U> mp) {
    return dynamic_cast<T*>(mp.ptr());
}

}

#endif /* LIB_CHECK50_FWK_PTR_H */
