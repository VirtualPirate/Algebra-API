#ifndef Tests_H
#define Tests_H

#include <variant>
#include <vector>
#include <utility>

class Operand;
class Constant;
class Variable;
class OperandList;
class Term;
class Expression;

class Variable_Subtitutor_List;

using Token = std::variant<double, char>;
using TokensConstIterator = std::vector<Token>::const_iterator;
using TokensConstIteratorPair = std::pair<TokensConstIterator, TokensConstIterator>;

using OperandAndTokensIterator = std::pair<Operand, TokensConstIterator>;

#define DEBUG 0
#if DEBUG == 1
    #define OPERATION_LOG(TYPE_1, TYPE_2, operation) std::cout << TYPE_1 << "-" << TYPE_2 << " arithmetic " << operation << " is invoked" << std::endl;
    #define OPERANDS(FIRST, SECOND) std::cout << "first = " << FIRST << "\t" << "second = " << SECOND << std::endl;
    #define RESULT(result) std::cout << "result = " << result << std::endl;
    #define LOG_CONSTRUCTOR(CONSTRUCTOR_TEXT) std::cout << "[Constructor]:" << CONSTRUCTOR_TEXT << std::endl;
    #define LOG_DESTRUCTOR(DESTRUCTOR_TEXT)	std::cout << "[Destructor]:" << DESTRUCTOR_TEXT << std::endl;
    #define LOG_MOVEMENT(MOVE_TEXT) std::cout << "[MOVE]:" << MOVE_TEXT << std::endl;

    #define LOG_CONSTRUCT() LOG_CONSTRUCTOR(__FUNCTION__);
    #define LOG_DESTRUCT() LOG_DESTRUCTOR(__FUNCTION__);
    #define LOG_MOVE() LOG_MOVEMENT(__FUNCTION__)

#else
    #define OPERATION_LOG(TYPE_1, TYPE_2, operation)
    #define OPERANDS(FIRST, SECOND)
    #define RESULT(result)
    #define LOG_CONSTRUCTOR(CONSTRUCTOR_TEXT)
    #define LOG_DESTRUCTOR(DESTRUCTOR_TEXT)
    #define LOG_MOVEMENT(MOVE_TEXT)

    #define LOG_CONSTRUCT(CONSTRUCTOR_SIGNATURE)
    #define LOG_DESTRUCT()
    #define LOG_MOVE()
#endif

#define CONSTANT_LOGS 0
#if CONSTANT_LOGS == 1
    #define CONSTANT_LOG_CONSTRUCT() LOG_CONSTRUCT()
    #define CONSTANT_LOG_DESTRUCT() LOG_DESTRUCT()
    #define CONSTANT_LOG_MOVE() LOG_MOVE()
#else
    #define CONSTANT_LOG_CONSTRUCT()
    #define CONSTANT_LOG_DESTRUCT()
    #define CONSTANT_LOG_MOVE()
#endif

#define VARIABLE_LOGS 0
#if VARIABLE_LOGS == 1
    #define VARIABLE_LOG_CONSTRUCT() LOG_CONSTRUCT()
    #define VARIABLE_LOG_DESTRUCT() LOG_DESTRUCT()
    #define VARIABLE_LOG_MOVE() LOG_MOVE()
#else
    #define VARIABLE_LOG_CONSTRUCT()
    #define VARIABLE_LOG_DESTRUCT()
    #define VARIABLE_LOG_MOVE()
#endif

#define TERM_LOGS 0
#if TERM_LOGS == 1
    #define TERM_LOG_CONSTRUCT() LOG_CONSTRUCT()
    #define TERM_LOG_DESTRUCT() LOG_DESTRUCT()
    #define TERM_LOG_MOVE() LOG_MOVE()
#else
    #define TERM_LOG_CONSTRUCT()
    #define TERM_LOG_DESTRUCT()
    #define TERM_LOG_MOVE()
#endif

#define EXPRESSION_LOGS 0
#if EXPRESSION_LOGS == 1
    #define EXPRESSION_LOG_CONSTRUCT() LOG_CONSTRUCT()
    #define EXPRESSION_LOG_DESTRUCT() LOG_DESTRUCT()
    #define EXPRESSION_LOG_MOVE LOG_MOVE()
#else
    #define EXPRESSION_LOG_CONSTRUCT()
    #define EXPRESSION_LOG_DESTRUCT()
    #define EXPRESSION_LOG_MOVE()
#endif

#define OPERAND_LOGS 0
#if OPERAND_LOGS == 1
    #define OPERAND_LOG_CONSTRUCT() LOG_CONSTRUCT()
    #define OPERAND_LOG_DESTRUCT() LOG_DESTRUCT()
    #define OPERAND_LOG_MOVE() LOG_MOVE()
#else
    #define OPERAND_LOG_CONSTRUCT()
    #define OPERAND_LOG_DESTRUCT()
    #define OPERAND_LOG_MOVE()
#endif

#endif