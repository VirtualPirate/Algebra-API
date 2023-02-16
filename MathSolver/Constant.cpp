#include <iostream>
#include <string>
#include <sstream>
#include <cmath>

#include "Operand.hpp"
#include "Constant.hpp"
#include "Variable.hpp"
#include "Term.hpp"
#include "Expression.hpp"
#include "Operation_Functions.hpp"
#include "Substitutor.hpp"

#include "Tests.hpp"

Constant const Constant::ONE{ 1 };
Constant const Constant::ZERO{ 0.0 };

//Constructors
Constant::Constant(void):value{0}{}
Constant::Constant(double val) : value{ val }, power{}{CONSTANT_LOG_CONSTRUCT(); }
Constant::Constant(const Constant& ref) : value{ ref.value }, power{ ref.power }{CONSTANT_LOG_CONSTRUCT(); }
Constant::Constant(const Constant* point) : value{ point->value }, power{ point->power }{CONSTANT_LOG_CONSTRUCT(); }

Constant::Constant(double val, const Operand& pow): value{val}{
	setPower(pow);
	CONSTANT_LOG_CONSTRUCT();
}

// Constant to Constant assignment operators
Constant& Constant::operator=(double val) { value = val;return *this; }
Constant& Constant::operator=(const Constant& ref){value = ref.value;power=ref.power;return *this;}

//Constant to Constant comparison operators

bool Constant::operator==(const double& other) const {
	if (power == CONSTANTS::NULL_OPERAND)
		return value == other;
	return false;
}
bool Constant::operator!=(const double& other) const { return !(*this == other); }

bool Constant::operator==(const Constant& other) const {return value == other.value && (power == other.power || (!power && !other.power));}
bool Constant::operator!=(const Constant& other) const {return !(*this == other);}

//Constant to double arithmetic operators
Operand Constant::operator+(const double& other) const {return *this + Constant{other};}
Operand Constant::operator-(const double& other) const {return *this - Constant{other};}
Operand Constant::operator*(const double& other) const {return *this * Constant{other};}
Operand Constant::operator/(const double& other) const {return *this / Constant{other};}
Operand Constant::raise_pow(const double& other) const {return this->raise_pow(Constant{other});}


//Constant to Operand arithmetic operators
Operand Constant::operator+(const Operand& other) const {return Operand{*this} + other;}
Operand Constant::operator-(const Operand& other) const {return Operand{*this} - other;}
Operand Constant::operator*(const Operand& other) const {return Operand{*this} * other;}
Operand Constant::operator/(const Operand& other) const {return Operand{*this} / other;}
Operand Constant::raise_pow(const Operand& other) const {return Operand{*this}.raise_pow(other);}


// Some other functions
Operand Constant::simplify() const {
	if(this->is_pure())
		return *this;
	else {
		Operand simplified_power{ this->power.simplify() };
		if (simplified_power.is_constant() && simplified_power.get<Constant>().is_pure())
			return Constant{ pow(value, simplified_power.get<Constant>().value) };
		return Constant{ value, simplified_power };
	}
}

Operand Constant::subtitute(const Variable_Subtitutor_List& list) const {
	return Constant{this->value, power.subtitute(list)};
}

std::string Constant::power_print() const{
	std::ostringstream stream;
	stream << *this;
	return stream.str();
}
bool Constant::is_negative() const {return value < 0;}
bool Constant::negative_power() const {return false;}
bool Constant::is_pure() const { return !power; }
Constant Constant::abs() const {return Constant{fabs(value)};}


//Getter methods
const double& Constant::getValue() const {
	return value;
}
const double& Constant::getBase() const {
	return value;
}
const Operand& Constant::getPower() const {
	if(power)
		return power;
	return CONSTANTS::ONE;
}

void Constant::setValue(const double& ref){
	value = ref;
}
void Constant::setPower(const Operand& ref){
	if(ref == CONSTANTS::ONE)
		power = Operand{};
	else
		power = ref;
}


Operand Constant::operator+() const {return *this;}
Operand Constant::operator-() const {return *this * CONSTANTS::MINUS_ONE;}

Operand Constant::multiplicative_inverse() const { return this->raise_pow(CONSTANTS::MINUS_ONE); }

Constant::~Constant() {
	CONSTANT_LOG_DESTRUCT();
}

std::ostream& operator<<(std::ostream& os, const Constant& ref){
	if(ref.power && ref.power!=(double)1)
		os << ref.value << '^' << ref.getPower().power_print();
	else
		os << ref.value;
	return os;
}