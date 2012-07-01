#ifndef LIB_CHECK50_FWK_PTRINTERFACE_H
#define LIB_CHECK50_FWK_PTRINTERFACE_H

namespace Fwk {

template <class T>
class PtrInterface {
public:
    PtrInterface() : _ref(0) {}
    unsigned long references() const { return _ref; }
    inline const PtrInterface * new_ref() const { ++_ref; return this; }
    inline void delete_ref() const { if( --_ref == 0 ) on_zero_references(); }
protected:
    virtual ~PtrInterface() {}
    virtual void on_zero_references() const { delete this; }
private:
    mutable long unsigned _ref;
};

}

#endif /* LIB_CHECK50_FWK_PTRINTERFACE_H */
