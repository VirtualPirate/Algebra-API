#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <stack>


#include "Operand.hpp"
#include "Constant.hpp"
#include "Variable.hpp"
#include "Term.hpp"
#include "Expression.hpp"
#include "Parser.hpp"
#include "Operation_Functions.hpp"

std::ostream& operator<<(std::ostream& os, Token const& v) {
	std::visit([&os](auto const& e) {os << e; }, v);
	return os;
}

bool operator==(const Token& first, const char& second) {
	if (std::holds_alternative<char>(first) && std::get<char>(first) == second)
		return true;
	return false;
}

bool operator!=(const Token& first, const char& second) {
	return !(first == second);
}

std::unordered_map<std::pair<char, char>, char, pair_hash> const Parser::lookup_table_a = {
	{{'+', '+'}, '+'},
	{{'+', '-'}, '-'},
	{{'-', '+'}, '-'},
	{{'-', '-'}, '+'},
	{{'*', '+'}, '*'},
	{{'*', '-'}, '\0'},
	{{'/', '+'}, '/'},
	{{'/', '-'}, '\0'},
	{{'^', '+'}, '^'},
	{{'^', '-'}, '\0'}
};


//Parser Implementation
Parser::Parser(const char* val) : context{ val }, current_index{ 0 }, tokens{}{
	remove_whitespaces();
	remove_redundant_operators();
	create_tokens();
	generalize_operators();
	remove_redundant_operator_tokens();
	strip_operators();
}
Parser::Parser(const std::string& val): context{val}, current_index{0}, tokens{}{
	remove_whitespaces();
	remove_redundant_operators();
	create_tokens();
	generalize_operators();
	remove_redundant_operator_tokens();
	strip_operators();
}
std::string Parser::getUnParsed() const {
	return context.substr(current_index);
}

const std::vector<Token>& Parser::getTokens() const {
	return tokens;
}
std::optional<double> Parser::match_number(){
	size_t match_length = 0;
	bool period = false;
	bool has_digit = false;
	bool matched_operator = false;
	auto iter = context.cbegin() + current_index;
	if(*iter == '-' || *iter == '+') {
		match_length++;
		iter++;
		matched_operator = true;
	}
	for(;iter!=context.cend(); iter++){
		if(isdigit(*iter)){
			match_length++;
			has_digit = true;
		}
		else if(*iter == '.' && !period){
			period = true;
			match_length++;
			has_digit = false;
		}else break;
	}
	if(has_digit){
		double value = std::stod(context.substr(current_index, match_length));
		current_index += match_length;
		if (matched_operator)
			tokens.emplace_back('+');
		return value;
	}
	return {};
}
std::optional<char> Parser::match_char(){
	auto iter = context.cbegin() + current_index;
	for(char each: "*+-/^()")
		if(*iter == each || isalpha(*iter)){
			current_index++;
			return *iter;
		}
		return {};
}

OperandAndTokensIterator Parser::match_power(TokensConstIterator begin, TokensConstIterator end) {
	OperandAndTokensIterator return_value;
	TokensConstIteratorPair group_iter;
	std::stack<Operand> power_stack;
	while (begin < end && *begin == '^') {
		if (Parser::is_double(*(begin + 1)))
			power_stack.push(std::get<double>(*(begin + 1)));
		else if (Parser::is_variable(*(begin + 1)))
			power_stack.push(std::get<char>(*(begin + 1)));
		else if (Parser::is_leftbrace(*(begin + 1))) {
			group_iter = Parser::grab_group(begin+1, end);
			//test::print_constiteratorpair(group_iter);
			//std::cout << "*(group_iter.second + 1)" << *(group_iter.second + 1) << std::endl;
			power_stack.push(Parse_Expression(group_iter.first, group_iter.second, true));
			//std::cout << "power_stack.top() = " << power_stack.top() << std::endl;
			begin = group_iter.second + 1;
			continue;
		}
		begin += 2;
	}
	//begin++;

	Operand each_operand{};
	while (!power_stack.empty()) {
		if (each_operand) 
			each_operand = power_stack.top().setPowerN_return(each_operand);
		else
			each_operand = power_stack.top().setPowerN_return((double)1);
		power_stack.pop();
	}

	return { each_operand, begin };
}

void Parser::remove_whitespaces() {
	for (char each : " \f\n\r\t\v")
		context.erase(std::remove(context.begin(), context.end(), each), context.end());
}

void Parser::remove_redundant_operators() {
	size_t expression_length = context.size();

	std::string str;
	str.reserve(expression_length);

	size_t lower_index = 0;
	size_t upper_index = 1;

	char lower_char = '\0';
	char upper_char = '\0';
	std::unordered_map<std::pair<char, char>, char, pair_hash>::const_iterator key_;
	std::pair<char, char> op_pair;

	while (upper_index < expression_length) {

		lower_char = context[lower_index];
		upper_char = context[upper_index];

		if (Parser::is_operator(lower_char) && Parser::is_operator(upper_char)) {
			op_pair.first = lower_char;
			op_pair.second = upper_char;
			key_ = Parser::lookup_table_a.find(op_pair);
			if (key_ != Parser::lookup_table_a.cend() && key_->second != '\0') {
				context[upper_index] = key_->second;
				context[lower_index] = '\0';
			}
			else if(key_ == Parser::lookup_table_a.cend())
				throw std::runtime_error{ "Error parsing - Invoked from Parser::remove_redunadant_operators()" };

		}

		upper_index++;
		lower_index++;
	}

	char each_char = '\0';
	for (size_t index = 0; index < expression_length; index++) {
		each_char = context[index];
		if (each_char != '\0')
			str.push_back(each_char);
	}

	context = std::move(str);
	//std::cout << "context = " << context << std::endl;

}

void Parser::remove_redundant_operator_tokens() {
	auto lower_index = 0;
	auto upper_index = 1;

	std::unordered_map<std::pair<char, char>, char, pair_hash>::const_iterator key_;
	std::pair<char, char> op_pair;
	while (upper_index < tokens.size()) {
		//std::cout << "lower_iter = " << tokens.at(lower_index) << std::endl;
		//std::cout << "upper_iter = " << tokens.at(upper_index) << std::endl;
		if (Parser::is_operator(tokens[upper_index]) && Parser::is_operator(tokens[lower_index])) {
			op_pair.first = std::get<char>(tokens[lower_index]);
			op_pair.second = std::get<char>(tokens[upper_index]);
			key_ = Parser::lookup_table_a.find(op_pair);
			if (key_ != Parser::lookup_table_a.cend() && key_->second != '\0') {
				tokens[upper_index] = key_->second;
				tokens.erase(tokens.begin() + lower_index);
				continue;
			}
			else if (key_ == Parser::lookup_table_a.cend())
				throw std::runtime_error{ "Error parsing - Invoked from Parser::remove_redunadant_operators()" };
		}

		lower_index++;
		upper_index++;
	}
}


void Parser::generalize_operators() {
	
	Token token_minus{ '-' };
	Token token_divide{ '/' };

	std::vector<Token> raise_minus_one{ {Token{'^'}, (double)-1} };

	bool has_operand = false;
	std::vector<Token> new_tokens;
	size_t minus_count = std::count(tokens.begin(), tokens.end(), token_minus);
	size_t div_count = std::count(tokens.begin(), tokens.end(), token_divide);
	new_tokens.reserve(tokens.size() + (minus_count + div_count) * 2);
	for (const Token& each : tokens) {
		if (each == token_minus) {
			if (has_operand) {
				new_tokens.push_back('+');
				new_tokens.push_back((double)-1);
			}
			else
				new_tokens.push_back((double)-1);
			continue;
		}
		else if (Parser::is_operand(each)) {
			has_operand = true;
		}
		else {
			has_operand = false;
		}
		new_tokens.push_back(each);
	}

	size_t index = 0;
	while (index < new_tokens.size()) {
		//If the token is a divide operator
		if (new_tokens[index] == token_divide) {
			if (index+1 >= new_tokens.size())
				break;
			// if the operand after divide operator is a group
			if (Parser::is_leftbrace(new_tokens[index + 1])) {
				new_tokens[index] = Token{ '*' };
				TokensConstIteratorPair iter_pair = Parser::grab_group(new_tokens.cbegin() + index + 1, new_tokens.cend());
				size_t end_index = std::distance(new_tokens.cbegin(), iter_pair.second) + 1;
				new_tokens.insert(new_tokens.begin() + end_index, raise_minus_one.begin(), raise_minus_one.end());
				index = end_index + raise_minus_one.size();
			}
			else {
				new_tokens[index] = new_tokens[index + 1];
				new_tokens[index + 1] = Token{ '^' };
				new_tokens.insert(new_tokens.begin() + index + 2, (double)-1);
				index += raise_minus_one.size();
			}
			continue;
		}
		index++;
	}

	tokens = std::move(new_tokens);

}


Operand Parser::Parse_Expression(TokensConstIterator begin, TokensConstIterator end, bool is_sub) {
	Expression result{};
	result.setNull(false);
	result.setSubexpression(is_sub);

	std::vector<Operand> fields;

	TokensConstIteratorPair group_iter;
	while (begin < end) {
		auto[first, second] = Parser::grab_term(begin, end);

		//std::cout << "each_term = ";
		//test::print_constiteratorpair({ first ,second });
		//std::cout << std::endl;
		begin = second;
		fields.clear();

		while (first < second) {
			if (Parser::is_double(*first)) {
				//std::cout << "Parse Expression inserted double -> " << std::get<double>(*first) << std::endl;
				fields.push_back(std::get<double>(*first));
			}
			else if (Parser::is_variable(*first)) {
				//std::cout << "Parse Expression inserted variable -> " << std::get<char>(*first) << std::endl;
				fields.push_back(std::get<char>(*first));
			}
			else if (Parser::is_leftbrace(*first)) {
				group_iter = Parser::grab_group(first, second);
				fields.push_back(Parse_Expression(group_iter.first, group_iter.second, true));
				first = group_iter.second;
				//std::cout << "remaining = ";
				//test::print_constiteratorpair({ first, second });
				continue;
			}
			else if (*first == '^') {
				OperandAndTokensIterator power_return = Parser::match_power(first, second);
				//std::cout << "power_return -> " << *(power_return.second) << std::endl;
				//std::cout << "power_return -> " << power_return.first << std::endl;
				//test::print_constiteratorpair({ first, power_return.second });
				//std::cout << "set power n return = " << fields.back().setPowerN_return(power_return.first) << std::endl;
				//std::cout << "power return + 1 -> " << *(power_return.second + 1) << std::endl;;
				if (fields.back().getPower() == CONSTANTS::ONE)
					fields.back().setPower(power_return.first);
				else {
					Term temp{ fields.back() };
					temp.setPower(power_return.first);
					fields.back() = temp;
				}
				//std::cout << "fields.back() = " << fields.back() << std::endl;
				first = power_return.second;
				continue;
			}
			first++;
		}

		//std::cout << "Term{fields} = " << Term{ fields } << std::endl;
		//std::cout << "fields.size() = " << fields.size() << std::endl;

		result.insert(Term{ fields });
	}

	//std::cout << "result = " << result << std::endl;

	return result;

}

Operand Parser::Parse_Expression(const std::string& expression) {
	Parser parser{ expression };
	if (parser.tokens.size() > 0)
		return Parser::Parse_Expression(parser.tokens.cbegin(), parser.tokens.cend());
	else
		return Operand{};
}

void Parser::create_tokens(){
	std::optional<double> number_match;
	std::optional<char> char_match;
	while(current_index < context.size()){
		if((number_match = match_number()))
			tokens.push_back(number_match.value());
		else if((char_match = match_char()))
			tokens.push_back(char_match.value());
		else
			throw std::runtime_error{"Parser::create_tokens(): Invalid token. Can't be parsed"};
	}
}

void Parser::debug_info(){
	for(std::variant<double, char>& each: tokens){
		if(std::holds_alternative<double>(each))
			std::cout << '[' << std::get<double>(each) << "] ";
		else
			std::cout << '[' << std::get<char>(each) << "] ";
	}
	std::cout << std::endl;
}

void Parser::test_grab_group() {
	auto iter = Parser::grab_group(tokens.cbegin(), tokens.cend());
	std::cout << "{ ";
	for (; iter.first < iter.second; (iter.first)++)
		std::cout << *(iter.first);
	std::cout << "}\n";
}

bool Parser::is_valid_expression(const std::string& str) {
	return static_cast<bool>(Operand{ str });
}
//Checks if a chararter is an operator
bool Parser::is_operator(const char& ref) {
	for (char each : "*+-/^") {
		if (ref == each)
			return true;
	}

	return false;
}

// Checks if a Token is an operator.
// Calls Parser::is_operator(const char& ref) internally
bool Parser::is_operator(const Token& ref) {
	if (std::holds_alternative<char>(ref))
		return Parser::is_operator(std::get<char>(ref));
	return false;
}

// Checks if a Token is double type
bool Parser::is_operand(const Token& ref) {
	if (std::holds_alternative<double>(ref))
		return true;
	else
		return isalpha(std::get<char>(ref));
}
inline bool Parser::is_double(const Token& ref) {
	return std::holds_alternative<double>(ref);
}

inline bool Parser::is_variable(const Token& ref) {
	return std::holds_alternative<char>(ref) && isalpha(std::get<char>(ref));
}

inline bool Parser::is_brace(const Token& ref, const char& brace) {
	return std::holds_alternative<char>(ref) && std::get<char>(ref) == brace;
}

// checks if a Token is '('
inline bool Parser::is_leftbrace(const Token& ref) {
	return Parser::is_brace(ref, '(');
}

// checks if a Token is ')'
inline bool Parser::is_rightbrace(const Token& ref) {
	return Parser::is_brace(ref, ')');
}


TokensConstIteratorPair Parser::grab_group(TokensConstIterator begin, TokensConstIterator end) {
	bool group_continuing = false;
	bool group_started = false;
	unsigned brace_count = 0;
	TokensConstIteratorPair group;

	for ( ;begin < end; begin++) {
		if (Parser::is_leftbrace(*begin)) {
			if (!group_started) {
				group.first = begin+1;
				group_started = true;
			}

			if (brace_count != 0)
				group.second = begin+1;
			brace_count++;
			continue;
		}
		else if (Parser::is_rightbrace(*begin))
			brace_count--;
		if (brace_count > 0) {
			group.second = begin+1;
			group_continuing = true;
		}
		else if (group_continuing)
			break;
			
	}

	return group;
}

TokensConstIteratorPair Parser::grab_term(TokensConstIterator begin, TokensConstIterator end) {
	bool term_started = false;
	TokensConstIteratorPair group;

	if (*begin == '+')
		begin++;
	for (; begin < end; begin++) {
		if (*begin != '+') {
			if (!term_started) {
				term_started = true;
				group.first = begin;
			}
			if (Parser::is_leftbrace(*begin))
				begin = Parser::grab_group(begin, end).second;
			if (begin < end)
				group.second = begin + 1;
			else {
				group.second = begin;
				break;
			}
		}
		else 
			return group;
	}

	return group;
}


inline void Parser::lstrip_operators() {
	while (tokens.begin() != tokens.end() && Parser::is_operator(*(tokens.begin())))
		tokens.erase(tokens.begin());
}

inline void Parser::rstrip_operators() {
	while (tokens.begin() != tokens.end() && Parser::is_operator(*(tokens.rbegin())))
		tokens.erase((tokens.rbegin()+1).base());
}

void Parser::strip_operators() {
	lstrip_operators();
	rstrip_operators();
}