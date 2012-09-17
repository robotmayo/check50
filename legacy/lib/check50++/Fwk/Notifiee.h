#ifndef LIB_CHECK50_FWK_NOTIFIEE_H
#define LIB_CHECK50_FWK_NOTIFIEE_H

#include <string>

#include "PtrInterface.h"
#include "Ptr.h"

using namespace std;

namespace Fwk {

class RootNotifiee : public PtrInterface<RootNotifiee> {
	/* Deliberately empty */
};

template<typename Notifier>
class BaseNotifiee : public RootNotifiee {	
public:
    BaseNotifiee(Notifier* n = NULL) : _notifier(n) {
	    if (n != NULL) {
			n->last_notifiee_is(
				static_cast<typename Notifier::Notifiee *>(this)
			);
	    }
	}
	
	~BaseNotifiee() {
	    if (_notifier != NULL) {
			_notifier->last_notifiee_is(0);
	    }
	}
	
	Ptr<Notifier> notifier() const {
	    return _notifier;
	}
	
	void notifier_is(Ptr<Notifier> n) {
	    if (_notifier != n) {
			if (_notifier != NULL) {
			    _notifier->last_notifiee_is(0);
			}
			_notifier = n;
			if (n != NULL) {
			    n->last_notifiee_is(
			    	static_cast<typename Notifier::Notifiee *>(this)
			    );
			}
		}
	}
	
private:
	Ptr<Notifier> _notifier;
	
};

}
#endif /* LIB_CHECK50_FWK_NOTIFIEE_H */
    