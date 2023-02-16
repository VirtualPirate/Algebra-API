#ifndef Variable_H
#define Variable_H

class Constant;
class Operand;
class Term;
class Expression;
class Substitutor;
class Substitutor_List;
class Variable_Subtitutor_List;

class Variable{
	char name;
	Operand power;

	friend class Constant;
	friend class Operand;
	friend class Term;
	friend class Expression;

public:
	Variable(Variable&&) = default;
	Variable(const Variable&) = default;
	~Variable() = default;
	Variable& operator=(const Variable&);
	Variable& operator=(Variable&&);
	Variable& operator=(char);

	Variable(void);
	Variable(char);
	Variable(char, double);
	Variable(char, const Operand&);
	// Variable(char, Operand&&);

	//Getter and Setter Methods
	const char& getBase() const;
	const char& getName() const;
	const Operand& getPower() const;

	void setBase(const char&);
	void setName(const char&);
	void setPower(const Operand&);

	// Variable to Variable comparison operators
	bool operator==(const Variable&) const;
	bool operator!=(const Variable&) const;
	bool issame(const Variable&) const;
	bool isnot_same(const Variable&) const;

	Operand operator+(const double&) const;
	Operand operator-(const double&) const;
	Operand operator*(const double&) const;
	Operand operator/(const double&) const;
	Operand raise_pow(const double&) const;

	//Variable to Constant operations
	Operand operator+(const Constant&) const;
	Operand operator-(const Constant&) const;
	Operand operator*(const Constant&) const;
	Operand operator/(const Constant&) const;
	Operand raise_pow(const Constant&) const;

	//Variable to Variable operations
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

	bool is_addable(const Term&) const;

	std::string power_print() const;
	bool is_negative() const;
	bool negative_power() const;
	Operand simplify() const;
	Operand substitute(const Substitutor_List& list) const;
	Operand subtitute(const Variable_Subtitutor_List& list) const;

	//Unary operators 
	Operand operator+() const;
	Operand operator-() const;

	Operand multiplicative_inverse() const;

	friend std::ostream& operator<<(std::ostream&, const Variable&);
};

#endif