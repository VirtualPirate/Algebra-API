#ifndef Expression_H
#define Expression_H

#include <sstream>
#include <vector>
#include <string>

#include "OperandList.hpp"

class Variable_Subtitutor_List;
class Substitutor;
class Substitutor_List;
class Constant;
class Operand;
class Variable;
class Term;
class OperandList;
class Expression: public OperandList{

	bool is_subexpression = false;

	using OperandList::OperandList;

	friend class Constant;
	friend class Operand;
	friend class Variable;
	friend class Term;
	friend std::ostream& operator<<(std::ostream&, const Expression&);

	void simplify_each(); // Simplifies each Operand in the Expression
	void simplify_internal(); // Calculates all the operands in the Expression with each other if possible
	void simplify_internal_expressions(); // Extends the Expressions inside the Expression if possible
	void remove_zeroes(); // Removes all the zeroes inside the Expression
	void constant_raise_power(); // Raises the Expression to power if the Power is a Number which has no decimal part
	Operand raise_power_if_size_is_1(); // If the size of fields in the Expression then fields.at(0).raise_pow(power;)

	void simplify_each(std::ostringstream&);
	void simplify_internal(std::ostringstream&);
	void simplify_internal_expressions(std::ostringstream&);
	void remove_zeroes(std::ostringstream&);
	void constant_raise_power(std::ostringstream&);
	Operand raise_power_if_size_is_1(std::ostringstream&);

	void simplify_each(std::vector<std::string>&);
	void simplify_internal(std::vector<std::string>&);
	void simplify_internal_expressions(std::vector<std::string>&);
	void remove_zeroes(std::vector<std::string>&);
	void constant_raise_power(std::vector<std::string>&);
	Operand raise_power_if_size_is_1(std::vector<std::string>&);

public:

	Expression(const Expression&);
	Expression& operator=(const Expression&);
	Expression(Expression&&);
	Expression& operator=(Expression&&);

	Expression(const Operand&);
	Expression& operator= (const Operand&);

	std::string power_print() const;
	bool is_negative() const;
	bool negative_power() const;

	Expression getBase() const;

	void setSubexpression(bool);
	bool isSubexpression() const;

	Operand simplify() const;
	Operand simplify(std::ostringstream&) const;
	Operand simplify(std::vector<std::string>&) const;
	Operand substitute(const Substitutor_List& list) const;
	Operand subtitute(const Variable_Subtitutor_List& list) const;

	static Operand Operand_Expression_mul(const Operand&, const Expression&);

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

	Operand operator+(const Variable&) const;
	Operand operator-(const Variable&) const;
	Operand operator*(const Variable&) const;
	Operand operator/(const Variable&) const;
	Operand raise_pow(const Variable&) const;

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

};

#endif