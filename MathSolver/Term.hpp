#ifndef Term_H
#define Term_H

#include <vector>
#include "OperandList.hpp"

class Variable_Subtitutor_List;
class Substitutor;
class Substitutor_List;
class Constant;
class Variable;
class Expression;
class Operand;
class OperandList;

class Term: public OperandList{
	using OperandList::OperandList;
	friend class Constant;
	friend class Variable;
	friend class Operand;
	friend class Expression;
	friend std::ostream& operator<<(std::ostream&, const Term&);
	friend bool same_operand_set(const Term&, const Term&);

	void extend(const Term&);

	//These functions are used by Term::simplify()
	void simplify_each();
	void simplify_internal();
	void simplify_internal_terms();
	void remove_ones(); // Removes all the ones
	void raise_power(); // Raises each operand in the Term to the power
	void swap_constant(); // Swaps the constant to the front of the Term
	bool has_zero() const;
public:

	Term(const Term&);
	Term& operator=(const Term&);
	Term(Term&&);
	Term& operator=(Term&&);

	Term(const Operand&);
	Term& operator=(const Operand&);

	bool is_negative() const;
	bool negative_power() const;
	std::string power_print() const;
	Operand simplify() const;
	Operand substitute(const Substitutor_List& list) const;
	Operand subtitute(const Variable_Subtitutor_List& list) const;

	bool is_Constant() const;
	bool is_Variable() const;

	Term getBase() const;
	std::vector<Variable> getVars() const;

	Operand operator+(const double&) const;
	Operand operator-(const double&) const;
	Operand operator*(const double&) const;
	Operand operator/(const double&) const;
	Operand raise_pow(const double&) const;

	Operand operator+(const Constant&) const;
	Operand operator-(const Constant&) const;
	Operand operator*(const Constant&) const;
	Operand operator/(const Constant&) const;
	Operand raise_pow(const Constant&) const;

	//Term to Variable arithmetic operators
	Operand operator+(const Variable&) const;
	Operand operator-(const Variable&) const;
	Operand operator*(const Variable&) const;
	Operand operator/(const Variable&) const;
	Operand raise_pow(const Variable&) const;

	//Variable to Term operations
	Operand operator+(const Term&) const;
	Operand operator-(const Term&) const;
	Operand operator*(const Term&) const;
	Operand operator/(const Term&) const;
	Operand raise_pow(const Term&) const;

	Operand operator+(const Expression&) const;
	Operand operator-(const Expression&) const;
	Operand operator*(const Expression&) const;
	Operand operator/(const Expression&) const;
	Operand raise_pow(const Expression&) const;

	Operand operator+(const Operand&) const;
	Operand operator-(const Operand&) const;
	Operand operator*(const Operand&) const;
	Operand operator/(const Operand&) const;
	Operand raise_pow(const Operand&) const;

	bool operator==(const Term&) const;
	bool operator!=(const Term&) const;
};

bool same_operand_set(const Term&, const Term&);

#endif