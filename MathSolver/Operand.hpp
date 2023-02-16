#ifndef Operand_H
#define Operand_H

#include <functional>
#include <iostream>

#define O_ARRAY_SIZE 18
#define TYPE_COUNT 4

enum class DataType
{Constant=0, Variable=1, Term=2, Expression=3, None=-1};
// const unsigned O_ARRAY_SIZE = 18;
unsigned func_hash(DataType, DataType);

class Variable_Subtitutor_List;
class Operand;

using GetBaseFunction = std::function<Operand(const Operand*)>;
using GetPowerFunction = std::function<const Operand&(const Operand*)>;
using SetPowerFunction = std::function<void(Operand*, const Operand&)>;
using OperationFunction = Operand (*)(const Operand&, const Operand&);
using ComparisonFunction = bool (*)(const Operand&, const Operand&);
using CheckFunction = std::function<bool(const Operand*)>;
using PowerPrintFunc = std::function<std::string(const Operand*)>;
using AllocateFunction = std::function<void*(const Operand*)>;
using DeallocateFunction = std::function<void(const Operand*)>;
using CoutFunction = std::function<std::ostream&(std::ostream&, const Operand&)>;
using SimplifyFunction = std::function<Operand(const Operand*)>;
using VariableSubtitutorFunction = std::function<Operand(const Operand*, const Variable_Subtitutor_List&)>;

class Constant;
class Variable;
class Term;
class Expression;


class Operand
{
	void* value;
	DataType type;
	bool is_null;

	friend class Constant;
	friend class Variable;
	friend class Term;
	friend class Expression;

	friend Operand Operand_to_Operand_pow(const Operand&, const Operand&);

	static OperationFunction add_functions[O_ARRAY_SIZE];
	static OperationFunction sub_functions[O_ARRAY_SIZE];
	static OperationFunction mul_functions[O_ARRAY_SIZE];
	static OperationFunction div_functions[O_ARRAY_SIZE];
	static OperationFunction pow_functions[O_ARRAY_SIZE];

	static ComparisonFunction eq_functions[O_ARRAY_SIZE];
	static ComparisonFunction neq_functions[O_ARRAY_SIZE];

	static GetBaseFunction get_base_functions[TYPE_COUNT];
	static GetPowerFunction get_power_functions[TYPE_COUNT];
	static SetPowerFunction set_power_functions[TYPE_COUNT];
	static CheckFunction is_negative_functions[TYPE_COUNT];
	static CheckFunction negative_power_functions[TYPE_COUNT];
	static PowerPrintFunc power_print_functions[TYPE_COUNT];
	static AllocateFunction allocate_functions[TYPE_COUNT];
	static DeallocateFunction deallocate_functions[TYPE_COUNT];
	static CoutFunction operator_cout_functions[TYPE_COUNT];
	static SimplifyFunction simplify_functions[TYPE_COUNT];
	static VariableSubtitutorFunction variable_subtitutor_function[TYPE_COUNT];

	template<typename Type>
	Type& get_nonconst() const{
		if(value != nullptr)
			return *((Type*)value);
		throw std::runtime_error{"get<Type>() on nullptr"};
	}

	template <typename Type>
	Operand getBase() const {
		return get<Type>().getBase();
	}
	template <typename Type>
	const Operand& getPower() const {
		return get<Type>().getPower();
	}
	template <typename Type>
	void setPower(const Operand& pow){
		return get_nonconst<Type>().setPower(pow);
	}
	template <typename Type>
	bool is_negative() const {
		return get<Type>().is_negative();
	}
	template <typename Type>
	bool negative_power() const {
		return get<Type>().negative_power();
	}
	template <typename Type>
	std::string power_print() const {
		return get<Type>().power_print();
	}
	template <typename Type>
	void* allocate() const {
		return (void *)(new Type{get<Type>()});
	}
	template <typename Type>
	void deallocate() const {
		delete (Type*)value;
	}
	template <typename Type>
	static std::ostream& cout_(std::ostream& os, const Operand& ref){
		os << ref.get<Type>();
		return os;
	}
	template <typename Type>
	Operand simplify() const {
		return get<Type>().simplify();
	}
	template <typename Type>
	Operand subtitute(const Variable_Subtitutor_List& list) const {
		return get<Type>().subtitute(list);
	}

	void* allocate() const;
	void deallocate() const;
public:
	Operand();

	Operand(double);
	Operand& operator=(double);
	Operand(char);
	Operand& operator=(char);
	Operand(const Constant&);
	Operand& operator=(const Constant&);
	Operand(const Variable&);
	Operand& operator=(const Variable&);
	Operand(const Term&);
	Operand& operator=(const Term&);
	Operand(const Expression&);
	Operand& operator=(const Expression&);

	Operand(const std::string&);
	Operand& operator=(const std::string&);

	// Copy and Move semantics
	Operand(const Operand&);
	Operand(Operand&&);
	Operand& operator=(const Operand&);
	Operand& operator=(Operand&&);

	//Destructor
	~Operand();

	//Getters and Setters
	const DataType& getType() const;
	bool isNull() const;

	template<typename Type>
	const Type& get() const{
		if(value != nullptr)
			return *((Type*)value);
		throw std::runtime_error{"get<Type>() on nullptr"};
	}

	Operand getBase() const;
	const Operand& getPower() const;
	void setPower(const Operand&);
	const Operand& setPowerN_return(const Operand&);
	std::string power_print() const;
	bool is_negative() const;
	bool negative_power() const;
	Operand substitute() const;
	Operand simplify() const;
	Operand subtitute(const Variable_Subtitutor_List&) const;
	Operand multiplicative_inverse() const;
	Operand additive_inverse() const;

	// Operand to Operand arithmetic operators
	Operand operator+(const Operand&) const;
	Operand operator-(const Operand&) const;
	Operand operator*(const Operand&) const;
	Operand operator/(const Operand&) const;
	Operand raise_pow(const Operand&) const;

	// Operand to double arithmetic operators
	Operand operator+(const double&) const;
	Operand operator-(const double&) const;
	Operand operator*(const double&) const;
	Operand operator/(const double&) const;
	Operand raise_pow(const double&) const;

	// Operand to Constant arithmetic operators
	Operand operator+(const Constant&) const;
	Operand operator-(const Constant&) const;
	Operand operator*(const Constant&) const;
	Operand operator/(const Constant&) const;
	Operand raise_pow(const Constant&) const;

	// Operand to Variable arithmetic operators
	Operand operator+(const Variable&) const;
	Operand operator-(const Variable&) const;
	Operand operator*(const Variable&) const;
	Operand operator/(const Variable&) const;
	Operand raise_pow(const Variable&) const;

	//Operand to Term arithmetic operators
	Operand operator+(const Term&) const;
	Operand operator-(const Term&) const;
	Operand operator*(const Term&) const;
	Operand operator/(const Term&) const;
	Operand raise_pow(const Term&) const;

	//Operand to Expression arithmetic operators
	Operand operator+(const Expression&) const;
	Operand operator-(const Expression&) const;
	Operand operator*(const Expression&) const;
	Operand operator/(const Expression&) const;
	Operand raise_pow(const Expression&) const;

	//Operand to Constant +=
	Operand& operator+=(const Constant&);
	Operand& operator-=(const Constant&);
	Operand& operator*=(const Constant&);
	Operand& operator/=(const Constant&);

	//Operand to Variable +=
	Operand& operator+=(const Variable&);
	Operand& operator-=(const Variable&);
	Operand& operator*=(const Variable&);
	Operand& operator/=(const Variable&);

	//Operand to Term +=
	Operand& operator+=(const Term&);
	Operand& operator-=(const Term&);
	Operand& operator*=(const Term&);
	Operand& operator/=(const Term&);

	//Operand to Expression +=
	Operand& operator+=(const Expression&);
	Operand& operator-=(const Expression&);
	Operand& operator*=(const Expression&);
	Operand& operator/=(const Expression&);

	//Operand to Operand +=
	Operand& operator+=(const Operand&);
	Operand& operator-=(const Operand&);
	Operand& operator*=(const Operand&);
	Operand& operator/=(const Operand&);

	Operand operator+() const;
	Operand operator-() const;
	//Operand to Operand comparison operators
	bool operator==(const Operand&) const;
	bool operator!=(const Operand&) const;

	//Operand to double comparison operators
	bool operator==(const double&) const;
	bool operator!=(const double&) const;

	//Operand to Constant comparison operators
	bool operator==(const Constant&) const;
	bool operator!=(const Constant&) const;

	//Operand to Variable comparison operators
	bool operator==(const Variable&) const;
	bool operator!=(const Variable&) const;

	//Operand to Term comparison operators
	bool operator==(const Term&) const;
	bool operator!=(const Term&) const;

	//Operand to Expression comparison operators
	bool operator==(const Expression&) const;
	bool operator!=(const Expression&) const;

	bool is_constant() const;
	bool is_variable() const;
	bool is_term() const;
	bool is_expression() const;

	operator bool() const;
	bool is_type(const Operand&) const;
	bool is_type(const DataType&) const;

	friend std::ostream& operator<<(std::ostream&, const Operand&);


};


//Operand to Operand operations
template <class Type_1, class Type_2>
Operand Operand_Operand_add(const Operand& first, const Operand& second){
	return Operand{first.get<Type_1>() + second.get<Type_2>()};}

template <class Type_1, class Type_2>
Operand Operand_Operand_sub(const Operand& first, const Operand& second){
	return Operand{first.get<Type_1>() - second.get<Type_2>()};}

template <class Type_1, class Type_2>
Operand Operand_Operand_mul(const Operand& first, const Operand& second){
	return Operand{first.get<Type_1>() * second.get<Type_2>()};}

template <class Type_1, class Type_2>
Operand Operand_Operand_div(const Operand& first, const Operand& second){
	return Operand{first.get<Type_1>() / second.get<Type_2>()};}

template <class Type_1, class Type_2>
Operand Operand_Operand_pow(const Operand& first, const Operand& second){
	return Operand{(first.get<Type_1>()).raise_pow(second.get<Type_2>())};}

//Operand to Operand comparison
template <class Type_1, class Type_2>
bool Operand_Operand_eq(const Operand& first, const Operand& second){
	return (first.get<Type_1>()) == (second.get<Type_2>());}

template <class Type_1, class Type_2>
bool Operand_Operand_neq(const Operand& first, const Operand& second){
	return (first.get<Type_1>()) != (second.get<Type_2>());}


//Operand is_negative template

#endif