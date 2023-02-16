#ifndef Parser_H
#define Parser_H

#include <utility>
#include <string>
#include <variant>
#include <optional>
#include <unordered_map>
#include <vector>
#include "Tests.hpp"

using Token = std::variant<double, char>;
using TokensConstIterator = std::vector<Token>::const_iterator;
using TokensConstIteratorPair = std::pair<TokensConstIterator, TokensConstIterator>;

using OperandAndTokensIterator = std::pair<Operand, TokensConstIterator>;

std::ostream& operator<<(std::ostream&, Token const&);
bool operator==(const Token&, const char&);
bool operator!=(const Token&, const char&);

//This hash struct is used by the lookup_table
struct pair_hash {
	template <class T1, class T2>
	std::size_t operator() (const std::pair<T1, T2>& pair) const {
		return std::hash<T1>()(pair.first) ^ std::hash<T2>()(pair.second);
	}
};

class Parser{

	static const std::unordered_map<std::pair<char, char>, char, pair_hash> lookup_table_a;

	std::string context;
	size_t current_index;
	std::vector<Token> tokens;

	std::optional<double> match_number();
	std::optional<char> match_char();

	inline void lstrip_operators(); // Strips out all the operators on the left
	inline void rstrip_operators(); // Strips out all the operators on the right
	void strip_operators(); // Strips out all the operators on the left and right

	void remove_whitespaces();
	void remove_redundant_operators(); // This function strips out redundant operator characters. Example: "-+" is deduced to "-" only
	void remove_redundant_operator_tokens();
	void generalize_operators(); // Generalize the operator - and / to + and *

public:
	Parser(const char*);
	Parser(const std::string&);
	std::string getUnParsed() const;

	const std::vector<Token>& getTokens() const;

	void create_tokens();
	void debug_info();
	void test_grab_group();

	static OperandAndTokensIterator match_power(TokensConstIterator, TokensConstIterator);
	static Operand Parse_Expression(TokensConstIterator, TokensConstIterator, bool = false);
	static Operand Parse_Expression(const std::string&);

	static bool is_valid_expression(const std::string&);
	static bool is_operator(const char&);
	static bool is_operator(const Token&);
	static bool is_operand(const Token&);
	inline static bool is_double(const Token&);
	inline static bool is_variable(const Token&);

	static inline bool is_brace(const Token&, const char&);
	inline static bool is_leftbrace(const Token&);
	inline static bool is_rightbrace(const Token&);

	static TokensConstIteratorPair grab_group(TokensConstIterator begin, TokensConstIterator end);
	static TokensConstIteratorPair grab_term(TokensConstIterator begin, TokensConstIterator end);

	static std::string remove_redundant_operators_2(std::string);
};

#endif