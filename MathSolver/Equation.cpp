

#include "Operand.hpp"
#include "Constant.hpp"
#include "Variable.hpp"
#include "Term.hpp"
#include "Expression.hpp"
#include "Operation_Functions.hpp"
#include "Substitutor.hpp"
#include "Equation.hpp"

HasVariableFunction Equation::has_variable_functions[] = {
	Equation::has_variable<Constant>,
	Equation::has_variable<Variable>,
	Equation::has_variable<Term>,
	Equation::has_variable<Expression>
};

bool Equation::has_variable(const Constant& value, char var_name) {
	return false;
}

bool Equation::has_variable(const Variable& value, char var_name) {
	return value.getName() == var_name;
}

bool Equation::has_variable(const Term& value, char var_name) {
	for (const Operand& each : value.getFields()) {
		if (Equation::has_variable(each, var_name))
			return true;
	}
	return false;
}

bool Equation::has_variable(const Expression& value, char var_name) {
	for (const Operand& each : value.getFields()) {
		if (Equation::has_variable(each, var_name))
			return true;
	}
	return false;
}

bool Equation::has_variable(const Operand& value, char var_name) {
	int index = (int)(value.getType());
	if (index > -1)
		return Equation::has_variable_functions[index](value, var_name);
	return false;
}