#include <vector>
#include <iostream>
#include <sstream>
#include <string>

#include "Operand.hpp"
#include "Constant.hpp"
#include "Variable.hpp"
#include "OperandList.hpp"
#include "Term.hpp"
#include "Expression.hpp"
#include "Operation_Functions.hpp"
#include "Substitutor.hpp"

#include "Tests.hpp"


Expression::Expression(const Expression& lvalue) : OperandList(lvalue), is_subexpression{ lvalue.is_subexpression }{}
Expression& Expression::operator=(const Expression& lvalue) {
	OperandList::operator=(lvalue);
	is_subexpression = lvalue.is_subexpression;
	return *this;
}
Expression::Expression(Expression&& rvalue) : OperandList(std::move(rvalue)), is_subexpression{ rvalue.is_subexpression }{}
Expression& Expression::operator=(Expression&& rvalue) {
	OperandList::operator=(std::move(rvalue));
	is_subexpression = rvalue.is_subexpression;
	return *this;
}

Expression::Expression(const Operand& ref) {
	if (ref.is_expression())
		*this = ref.get<Expression>();
	else 
		OperandList::operator=(ref);
}

Expression& Expression::operator=(const Operand& ref) {
	if (ref.is_expression())
		*this = ref.get<Expression>();
	else
		OperandList::operator=(ref);

	return *this;
}

std::ostream& operator<<(std::ostream& os, const Expression& ref){
	if(!ref.isNull()){
		bool brace = ref.isSubexpression();
		if(ref.getPower() != Operand{(double)1})
			brace = true;
		if(brace)
			os << '(';
		const std::vector<Operand>& fields = ref.getFields();
		os << *(fields.begin());
		for(auto each=fields.begin()+1; each!=fields.end(); each++){
			if(!each->is_negative())
				os << " + " << *each;
			else
				os << ' ' << *each;
		}
		if (brace && ref.getPower() != CONSTANTS::ONE)
			os << ")^" << ref.getPower().power_print();
		else if (brace)
			os << ')';
	}
	else{
		os << "null_expression{}";
	}
	return os;
}

std::string Expression::power_print() const{
	std::ostringstream stream;
	stream << *this;
	return stream.str();
}
bool Expression::is_negative() const {
	return false;
}
bool Expression::negative_power() const {
	return power.is_negative();
}

void Expression::simplify_each() {
	for (Operand& each : fields) {
		//std::cout << "each = " << each << std::endl;
		each = std::move(each.simplify());
	}
}

void Expression::simplify_internal() {
	auto upper_iter = fields.begin();
	auto lower_iter = fields.begin() + 1;

	Operand each_operand;
	while (upper_iter != fields.end() - 1) {
		each_operand = *upper_iter + *lower_iter;
		//std::cout << "upper_iter = " << *upper_iter << std::endl;
		//std::cout << "lower_iter = " << *lower_iter << std::endl;
		//std::cout << "result = " << result << std::endl;
		if (each_operand)
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

void Expression::simplify_internal_expressions() {
	Expression simplified_expression;
	for (size_t each = 0; each < fields.size();) {
		if (fields.at(each).is_expression() && fields.at(each).getPower() == CONSTANTS::ONE) {
			simplified_expression = std::move(fields.at(each).simplify());
			fields.erase(fields.begin() + each);;
			fields.insert(fields.begin() + each, simplified_expression.fields.begin(), simplified_expression.fields.end());
			each += simplified_expression.fields.size();
		}
		else
			each++;
	}
}

void Expression::remove_zeroes() {
	for (auto each = fields.begin(); each != fields.end(); ) {
		if (each->is_constant() && *each == CONSTANTS::ZERO && fields.size() > 1)
			fields.erase(each);
		else
			each++;
	}
}

void Expression::constant_raise_power() {
	if (power.is_constant()) {
		Operand power_simplified{ power.simplify() };
		if (power_simplified.getPower() == CONSTANTS::ONE) {
			double power_value = power.simplify().get<Constant>().value;
			if (power_value == (int)power_value && power_value > 1) {
				power = CONSTANTS::ONE;
				*this = expression_constant_power(*this, (int)power_value);
			}
			else if (power_value == (int)power_value && power_value < -1) {
				power = CONSTANTS::MINUS_ONE;
				*this = expression_constant_power_minus(*this, (int)power_value);
			}
		}
	}
}

Operand Expression::raise_power_if_size_is_1() {
	if (fields.size() == 1)
		return fields[0].raise_pow(power).simplify();
	return *this;
}

Operand Expression::simplify() const {

	Expression result{ *this };
	result.simplify_each();
	result.simplify_internal_expressions();
	result.simplify_internal();
	result.remove_zeroes();
	result.constant_raise_power();

	return result.raise_power_if_size_is_1();

}

Operand Expression::subtitute(const Variable_Subtitutor_List& list) const {
	Expression copy_{ *this };
	for (Operand& each : copy_.fields)
		each = std::move(each.subtitute(list));
	copy_.power = std::move(copy_.power.subtitute(list));
	return copy_;
}

Expression Expression::getBase() const{
	return Expression{fields};
}
void Expression::setSubexpression(bool is_sub) { is_subexpression = is_sub; }
bool Expression::isSubexpression() const { return is_subexpression; }
//Operand to double arithmetic operators
Operand Expression::operator+(const double& other) const {return *this + Operand{other};}
Operand Expression::operator-(const double& other) const {return *this - Operand{other};}
Operand Expression::operator*(const double& other) const {return *this * Operand{other};}
Operand Expression::operator/(const double& other) const {return *this / Operand{other};}
Operand Expression::raise_pow(const double& other) const {return this->raise_pow(Operand{other});}

//Expression to Operand arithmetic operators
Operand Expression::operator+(const Operand& other) const {return Operand{*this} + other;}
Operand Expression::operator-(const Operand& other) const {return Operand{*this} - other;}
Operand Expression::operator*(const Operand& other) const {return Operand{*this} * other;}
Operand Expression::operator/(const Operand& other) const {return Operand{*this} / other;}
Operand Expression::raise_pow(const Operand& other) const {return Operand{*this}.raise_pow(other);}

Operand Expression::Operand_Expression_mul(const Operand& first, const Expression& second) {
	if (second.power == CONSTANTS::ONE) {
		std::vector<Operand> result_fields;
		result_fields.reserve(second.fields.size());
		for (const Operand& each_operand : second.fields)
			result_fields.push_back(first * each_operand);
		return Expression{ result_fields };
	}
	else
		return Term{ first, second };
}

void Expression::simplify_each(std::ostringstream& output) {
	std::string prev_output = get_cout_string(*this);

	for (Operand& each : fields)
		each = std::move(each.simplify());

	std::string next_output = get_cout_string(*this);
	if (prev_output != next_output)
		output << next_output << '\n';
}

void Expression::simplify_internal(std::ostringstream& output) {
	auto upper_iter = fields.begin();
	auto lower_iter = fields.begin() + 1;

	Operand each_operand;
	while (upper_iter != fields.end() - 1) {
		each_operand = *upper_iter + *lower_iter;
		if (each_operand)
		{
			*lower_iter = std::move(each_operand.simplify());
			fields.erase(upper_iter);
			upper_iter = fields.begin();
			lower_iter = fields.begin() + 1;
			output << *this << '\n'; // Modifies the output
			continue;
		}
		else if (++lower_iter == fields.end()) {
			upper_iter++;
			lower_iter = upper_iter + 1;
		}
	}
}

void Expression::simplify_internal_expressions(std::ostringstream& output) {
	std::string prev_output = get_cout_string(*this);

	Expression simplified_expression;
	for (size_t each = 0; each < fields.size();) {
		if (fields.at(each).is_expression() && fields.at(each).getPower() == CONSTANTS::ONE) {
			simplified_expression = std::move(fields.at(each).simplify());
			fields.erase(fields.begin() + each);;
			fields.insert(fields.begin() + each, simplified_expression.fields.begin(), simplified_expression.fields.end());
			each += simplified_expression.fields.size();
		}
		else
			each++;
	}

	std::string next_output = get_cout_string(*this);
	if (prev_output != next_output)
		output << next_output << '\n';
}

void Expression::remove_zeroes(std::ostringstream& output) {

	std::string prev_output = get_cout_string(*this);

	for (auto each = fields.begin(); each != fields.end(); ) {
		if (each->is_constant() && *each == CONSTANTS::ZERO && fields.size() > 1)
			fields.erase(each);
		else
			each++;
	}

	std::string next_output = get_cout_string(*this);
	if (prev_output != next_output)
		output << next_output << '\n';
}

void Expression::constant_raise_power(std::ostringstream& output) {

	std::string prev_output = get_cout_string(*this);

	if (power.is_constant()) {
		Operand power_simplified{ power.simplify() };
		if (power_simplified.getPower() == CONSTANTS::ONE) {
			double power_value = power.simplify().get<Constant>().value;
			if (power_value == (int)power_value && power_value > 1) {
				power = CONSTANTS::ONE;
				*this = expression_constant_power(*this, (int)power_value);
			}
			else if (power_value == (int)power_value && power_value < -1) {
				power = CONSTANTS::MINUS_ONE;
				*this = expression_constant_power_minus(*this, (int)power_value);
			}
		}
	}

	std::string next_output = get_cout_string(*this);
	if (prev_output != next_output)
		output << next_output << '\n';
}

Operand Expression::raise_power_if_size_is_1(std::ostringstream& output) {
	std::string prev_output = get_cout_string(*this);
	if (fields.size() == 1) {
		Operand result{ fields[0].raise_pow(power).simplify() };
		std::string next_output = get_cout_string(result);
		if (prev_output != next_output)
			output << next_output << '\n';
		return result;
	}
	return *this;
}

Operand Expression::simplify(std::ostringstream& output) const {

	output.str(""); // Clearing the string stream
	output << *this << '\n';

	Expression result{ *this };
	result.simplify_each(output);
	result.simplify_internal_expressions(output);
	result.simplify_internal(output);
	result.remove_zeroes(output);
	result.constant_raise_power(output);

	return result.raise_power_if_size_is_1(output);

}


void Expression::simplify_each(std::vector<std::string>& vec) {
	std::string prev_output = get_cout_string(*this);

	for (Operand& each : fields)
		each = std::move(each.simplify());

	std::string next_output = get_cout_string(*this);
	if (prev_output != next_output)
		vec.push_back(next_output);
		//output << next_output << '\n';
}

void Expression::simplify_internal(std::vector<std::string>& vec) {
	auto upper_iter = fields.begin();
	auto lower_iter = fields.begin() + 1;

	Operand each_operand;
	while (upper_iter != fields.end() - 1) {
		each_operand = *upper_iter + *lower_iter;
		if (each_operand)
		{
			*lower_iter = std::move(each_operand.simplify());
			fields.erase(upper_iter);
			upper_iter = fields.begin();
			lower_iter = fields.begin() + 1;
			vec.push_back(get_cout_string(*this));
			//output << *this << '\n'; // Modifies the output
			continue;
		}
		else if (++lower_iter == fields.end()) {
			upper_iter++;
			lower_iter = upper_iter + 1;
		}
	}
}

void Expression::simplify_internal_expressions(std::vector<std::string>& vec) {
	std::string prev_output = get_cout_string(*this);

	Expression simplified_expression;
	for (size_t each = 0; each < fields.size();) {
		if (fields.at(each).is_expression() && fields.at(each).getPower() == CONSTANTS::ONE) {
			simplified_expression = std::move(fields.at(each).simplify());
			fields.erase(fields.begin() + each);;
			fields.insert(fields.begin() + each, simplified_expression.fields.begin(), simplified_expression.fields.end());
			each += simplified_expression.fields.size();
		}
		else
			each++;
	}

	std::string next_output = get_cout_string(*this);
	if (prev_output != next_output)
		vec.push_back(next_output);
		//output << next_output << '\n';
}

void Expression::remove_zeroes(std::vector<std::string>& vec) {

	std::string prev_output = get_cout_string(*this);

	for (auto each = fields.begin(); each != fields.end(); ) {
		if (each->is_constant() && *each == CONSTANTS::ZERO && fields.size() > 1)
			fields.erase(each);
		else
			each++;
	}

	std::string next_output = get_cout_string(*this);
	if (prev_output != next_output)
		vec.push_back(next_output);
		//output << next_output << '\n';
}

void Expression::constant_raise_power(std::vector<std::string>& vec) {

	std::string prev_output = get_cout_string(*this);

	if (power.is_constant()) {
		Operand power_simplified{ power.simplify() };
		if (power_simplified.getPower() == CONSTANTS::ONE) {
			double power_value = power.simplify().get<Constant>().value;
			if (power_value == (int)power_value && power_value > 1) {
				power = CONSTANTS::ONE;
				*this = expression_constant_power(*this, (int)power_value);
			}
			else if (power_value == (int)power_value && power_value < -1) {
				power = CONSTANTS::MINUS_ONE;
				*this = expression_constant_power_minus(*this, (int)power_value);
			}
		}
	}

	std::string next_output = get_cout_string(*this);
	if (prev_output != next_output)
		vec.push_back(next_output);
		//output << next_output << '\n';
}

Operand Expression::raise_power_if_size_is_1(std::vector<std::string>& vec) {
	std::string prev_output = get_cout_string(*this);
	if (fields.size() == 1) {
		Operand result{ fields[0].raise_pow(power).simplify() };
		std::string next_output = get_cout_string(result);
		if (prev_output != next_output)
			vec.push_back(next_output);
			//output << next_output << '\n';
		return result;
	}
	return *this;
}

Operand Expression::simplify(std::vector<std::string>& vec) const {

	//output.str(""); // Clearing the string stream
	//output << *this << '\n';
	vec.push_back(get_cout_string(*this));


	Expression result{ *this };
	result.simplify_each(vec);
	result.simplify_internal_expressions(vec);
	result.simplify_internal(vec);
	result.remove_zeroes(vec);
	result.constant_raise_power(vec);

	return result.raise_power_if_size_is_1(vec);

}