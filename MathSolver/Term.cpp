#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <initializer_list>

#include "Operand.hpp"
#include "Constant.hpp"
#include "Variable.hpp"
#include "OperandList.hpp"
#include "Term.hpp"
#include "Expression.hpp"
#include "Operation_Functions.hpp"
#include "Substitutor.hpp"

#include "Tests.hpp"

Term::Term(const Term& lvalue) : OperandList(lvalue) {}
Term& Term::operator=(const Term& lvalue) {
	OperandList::operator=(lvalue);
	return *this;
}
Term::Term(Term&& rvalue) : OperandList(std::move(rvalue)) {}
Term& Term::operator=(Term&& rvalue) {
	OperandList::operator=(std::move(rvalue));
	return *this;
}

Term::Term(const Operand& ref) {
	if (ref.is_term())
		*this = ref.get<Term>();
	else
		OperandList::operator=(ref);
}

Term& Term::operator=(const Operand& ref) {
	if (ref.is_term())
		*this = ref.get<Term>();
	else
		OperandList::operator=(ref);

	return *this;
}

void Term::extend(const Term& ref){
	is_simplified = false;
	fields.insert(fields.end(), ref.fields.begin(), ref.fields.end());
}
std::ostream& operator<<(std::ostream& os, const Term& ref){
	if(!ref.isNull()){
		bool brace = false;
		if(ref.getPower() != Operand{(double)1})
			brace = true;
		const std::vector<Operand>& fields = ref.getFields();
		if(brace)
			os << '(';
		os << *(fields.begin());
		for(auto each=fields.begin()+1; each!=fields.end(); each++){
			if(each->getType() == DataType::Constant)
				os << " * " << *each;
			else
				os << *each;
		}
		if(brace)
			os << ")^" << ref.getPower().power_print();
	}

	else{
		os << "null_term{}";
	}
	return os;
}

std::string Term::power_print() const{
	std::ostringstream stream;
	stream << '(';
	stream << *this;
	stream << ')';
	return stream.str();
}

bool Term::is_negative() const {
	return fields.at(0).is_negative();
}
bool Term::negative_power() const {
	return power.is_negative();
}

void Term::simplify_each() {
	for (Operand& each : fields)
		each = std::move(each.simplify());
}

void Term::simplify_internal() {
	auto upper_iter = fields.begin();
	auto lower_iter = fields.begin() + 1;

	Operand each_operand;
	while (upper_iter != fields.end() - 1) {
		each_operand = *upper_iter * *lower_iter;
		//std::cout << "upper_iter = " << *upper_iter << std::endl;
		//std::cout << "lower_iter = " << *lower_iter << std::endl;
		//std::cout << "result = " << result << std::endl;
		if (!(each_operand.is_term()))
		{
			*lower_iter = std::move(each_operand.simplify());
			fields.erase(upper_iter);
			upper_iter = fields.begin();
			lower_iter = fields.begin() + 1;
			continue;
		}
		else if (++lower_iter == fields.end()) {
			upper_iter++;
			lower_iter = upper_iter + 1;
		}
	}
}

void Term::simplify_internal_terms() {
	Term simplified_term;
	for (size_t each = 0; each < fields.size();) {
		if (fields.at(each).is_term()) {
			simplified_term = std::move(fields.at(each).simplify());
			fields.erase(fields.begin() + each);;
			fields.insert(fields.begin() + each, simplified_term.fields.begin(), simplified_term.fields.end());
			each += simplified_term.fields.size();
		}
		else
			each++;
	}
}


void Term::remove_ones() {
	for (auto each = 0; each < fields.size();) {
		if (fields.at(each).is_constant() && fields.at(each) == CONSTANTS::ONE && fields.size() > 1)
			fields.erase(fields.cbegin() + each);
		else
			each++;
	}
}

void Term::raise_power() {
	if (power != CONSTANTS::ONE) {
		for (Operand& each_operand : fields)
			each_operand = std::move(each_operand.raise_pow(power).simplify());
		power = CONSTANTS::ONE;
	}
}

void Term::swap_constant() {
	if (count(DataType::Constant) == 1) {
		auto swap_iter = fields.begin() + cbegin(DataType::Constant).getIndex();
		if (swap_iter != fields.end())
			std::iter_swap(fields.begin(), swap_iter);
	}
}

bool Term::has_zero() const {
	for (auto each = this->cbegin(DataType::Constant); each != this->cend(DataType::Constant); each++) {
		if (each->get<Constant>() == Constant::ZERO)
			return true;
	}
	return false;
}


Operand Term::simplify() const {
	if (has_zero())
		return CONSTANTS::ZERO;

	Term result{ *this };
	result.simplify_each(); // Simplifies each operand in the Term
	result.simplify_internal_terms(); // Extends the Terms inside the Term if possible
	result.simplify_internal(); // Calculates all the operands in the Term with each other if possible
	result.remove_ones(); // Removes all the ones in the Term
	result.raise_power(); // Raises each operand in the Term to the power
	result.swap_constant(); // If Term contains one constant then it is swapped into the front of the Term

	if (has_zero()) // If the term contains a zero
		return CONSTANTS::ZERO;
	else
		return result;
}

Operand Term::subtitute(const Variable_Subtitutor_List& list) const {
	Term copy_{ *this };
	for (Operand& each : copy_.fields)
		each = std::move(each.subtitute(list));
	copy_.power = std::move(copy_.power.subtitute(list));
	return copy_;
}

bool Term::is_Constant() const {
	if(fields.size() == 1 && fields.at(0).getType() == DataType::Constant)
		return true;
	return false;
}

bool Term::is_Variable() const {
	if(fields.size() == 1 && fields.at(0).getType() == DataType::Variable)
		return true;
	return false;
}

std::vector<Variable> Term::getVars() const {
	std::vector<Variable> result;
	for(auto each=cbegin(DataType::Variable); each != cend(DataType::Variable); each++)
		result.push_back((*each).get<Variable>());
	return result;
}

Term Term::getBase() const {
	return Term{fields};
}

//Term to double arithmetic operators
Operand Term::operator+(const double& other) const {return *this + Constant{other};};
Operand Term::operator-(const double& other) const {return *this - Constant{other};};
Operand Term::operator*(const double& other) const {return *this * Constant{other};};
Operand Term::operator/(const double& other) const {return *this / Constant{other};};
Operand Term::raise_pow(const double& other) const {return this->raise_pow(Constant{other});};

//Term to Operand arithmetic operators
Operand Term::operator+(const Operand& other) const {return Operand{*this} + other;}
Operand Term::operator-(const Operand& other) const {return Operand{*this} - other;}
Operand Term::operator*(const Operand& other) const {return Operand{*this} * other;}
Operand Term::operator/(const Operand& other) const {return Operand{*this} / other;}
Operand Term::raise_pow(const Operand& other) const {return Operand{*this}.raise_pow(other);}

bool Term::operator==(const Term& ref) const {
	if(same_operand_set(*this, ref) && power == ref.power)
		return true;
	return false;
}

bool Term::operator!=(const Term& ref) const {
	return !(*this == ref);
}

bool same_operand_set(const Term& first, const Term& second){
	if(first.fields.size() == second.fields.size()){
		for(auto each=first.fields.cbegin(); each!=first.fields.cend(); each++){
			if(std::find(second.fields.cbegin(), second.fields.cend(), *each) == second.fields.cend())
				return false;
		}
		return true;
	}
	return false;
}