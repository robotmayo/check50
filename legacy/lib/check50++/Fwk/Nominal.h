#ifndef LIB_CHECK50_FWK_NOMINAL_H
#define LIB_CHECK50_FWK_NOMINAL_H

template<class UnitType, class RepType>
	class Nominal
{
public:
	Nominal(RepType v) : _value(v) {}
	
	bool operator==(const Nominal<UnitType, RepType>& v) const
	{ return _value == v._value; }
	
	bool operator!=(const Nominal<UnitType, RepType>& v) const
	{ return _value != v._value; }
	
	const Nominal<UnitType, RepType>& operator=(const Nominal<UnitType,
															  RepType>& v)
	{ _value = v._value; return *this; }
	
	RepType value() const
	{ return _value; }
	
protected:
	RepType _value;
};

template<class UnitType, class RepType>
	class Ordinal : public Nominal<UnitType, RepType>
{
public:
	Ordinal(RepType v) : Nominal<UnitType, RepType>(v) {}
	
	bool operator<(const Ordinal<UnitType, RepType>& v) const
	{ return Nominal<UnitType, RepType>::_value < v._value; }
	
	bool operator<=(const Ordinal<UnitType, RepType>& v) const
	{ return Nominal<UnitType, RepType>::_value <= v._value; }
	
	bool operator>(const Ordinal<UnitType, RepType>& v) const
	{ return Nominal<UnitType, RepType>::_value > v._value; }

	bool operator>=(const Ordinal<UnitType, RepType>& v) const
	{ return Nominal<UnitType, RepType>::_value >= v._value; }
	
	Ordinal<UnitType, RepType> operator+(const Ordinal<UnitType,
													   RepType>& other)
	{ return (Nominal<UnitType, RepType>::_value + other._value); }
	
	Ordinal<UnitType, RepType> operator/(const Ordinal<UnitType,
													   RepType>& other)
	{ return (Nominal<UnitType, RepType>::_value / other._value); }
	
	const Ordinal<UnitType, RepType>& operator++()
	{ ++Nominal<UnitType, RepType>::_value; return *this; }
};

#endif /* LIB_CHECK50_FWK_NOMINAL_H */
