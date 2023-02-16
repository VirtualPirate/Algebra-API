#ifndef Operation_Functions_H
#define Operation_Functions_H

#include <sstream>

class Constant;
class Variable;
class Term;
class Operand;
class Expression;
class Substitutor;
class Variable_Subtitutor;

bool is_addable(const Term&, const Term&);
std::string return_type_string(DataType);

template <typename Type>
std::string get_cout_string(const Type& lvalue) {
	std::ostringstream stream_;
	stream_ << lvalue;
	return stream_.str();
}

std::string match_number(std::string);

//Operand expression_constant_power(const Expression&, int);
Operand expression_constant_power(const Expression&, int);
Operand expression_constant_power_minus(const Expression&, int);

namespace CONSTANTS{
	extern const Variable_Subtitutor VAR_NULL_SUBTITUTE;
	extern const Substitutor null_substitutor;
	extern const Operand ONE;
	extern const Operand ZERO;
	extern const Operand MINUS_ONE;
	extern const Operand NULL_OPERAND;
};


#endif