#ifndef LIB_CHECK50_TYPES_H
#define LIB_CHECK50_TYPES_H

#include "Fwk/Nominal.h"
#include "Fwk/Exception.h"

namespace Check50 {

class Points : public Ordinal<Points, float> {
public:
    Points(float p) : Ordinal<Points, float>(p) {
        if (p < 0) {
            throw Fwk::RangeException("NegativePointsException");
        }
    }
};

class Priority: public Ordinal<Priority, float> {
public:
    Priority(float p) : Ordinal<Priority, float> (p) {
        if (p < 0) {
            throw Fwk::RangeException("NegativePriorityException");
        }
    }
};

}      /* end namespace */
#endif /* LIB_CHECK50_TYPES_H */
