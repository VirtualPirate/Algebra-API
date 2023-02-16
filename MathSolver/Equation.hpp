#ifndef Equation_H
#define Equation_H

class Constant;
class Variable;
class Operand;
class Term;
class Expression;

#ifndef TYPE_COUNT
#define TYPE_COUNT 4
#endif

using HasVariableFunction = std::function<bool(const Operand&, char)>;

class Equation {
	Operand lhs;
	Operand rhs;

	static HasVariableFunction has_variable_functions[TYPE_COUNT];

	template <typename Type>
	static bool has_variable(const Operand& value, char var_name) {
		return Equation::has_variable(value.get<Type>(), var_name);
	}

	static bool has_variable(const Constant&, char);
	static bool has_variable(const Variable&, char);
	static bool has_variable(const Term&, char);
	static bool has_variable(const Expression&, char);

	static bool has_variable(const Operand&, char);
};
#endif