#ifndef Constant_H
#define Constant_H

#include <iostream>

class Operand;
class Variable;
class Term;
class Expression;
class Substitutor;
class Substitutor_List;
class Variable_Subtiutor_List;
class Constant{
	double value;
	Operand power;

	friend class Operand;
	friend class Variable;
	friend class Term;
	friend class Expression;
public:
	static Constant const ONE;
	static Constant const ZERO;

	//Constructors
	Constant(void);
	Constant(double);
	Constant(const Constant&);
	Constant(const Constant*);

	Constant(double, const Operand&);

	// Assignment operators
	Constant& operator=(double);
	Constant& operator=(const Constant&);

	//Getter Methods
	const double& getBase() const;
	const double& getValue() const;
	const Operand& getPower() const;

	//Setter Methods
	void setBase(const double&) const;
	void setValue(const double&);
	void setPower(const Operand&);

	Operand simplify() const;
	Operand substitute(const Substitutor_List& list) const;
	Operand subtitute(const Variable_Subtitutor_List&) const;
	std::string power_print() const;
	bool is_negative() const; // Checks if the Constant is negative
	bool negative_power() const;
	bool is_pure() const; // Checks if the Constant has power 1
	Constant abs() const; // Returns the absolute value of the Constant without power included

	//Constant to double arithmetic operations
	Operand operator+(const double&) const;
	Operand operator-(const double&) const;
	Operand operator*(const double&) const;
	Operand operator/(const double&) const;
	Operand raise_pow(const double&) const;

	//Constant to Constant operations
	Operand operator+(const Constant&) const;
	Operand operator-(const Constant&) const;
	Operand operator*(const Constant&) const;
	Operand operator/(const Constant&) const;
	Operand raise_pow(const Constant&) const;

	//Constant to Variable arithmetic operations
	Operand operator+(const Variable&) const;
	Operand operator-(const Variable&) const;
	Operand operator*(const Variable&) const;
	Operand operator/(const Variable&) const;
	Operand raise_pow(const Variable&) const;

	//Constant to Term arithmetic operations
	Operand operator+(const Term&) const;
	Operand operator-(const Term&) const;
	Operand operator*(const Term&) const;
	Operand operator/(const Term&) const;
	Operand raise_pow(const Term&) const;

	//Constant to Expression arithmetic operations
	Operand operator+(const Expression&) const;
	Operand operator-(const Expression&) const;
	Operand operator*(const Expression&) const;
	Operand operator/(const Expression&) const;
	Operand raise_pow(const Expression&) const;

	//Constant to Operand arithmetic operations
	Operand operator+(const Operand&) const;
	Operand operator-(const Operand&) const;
	Operand operator*(const Operand&) const;
	Operand operator/(const Operand&) const;
	Operand raise_pow(const Operand&) const;


	bool operator==(const double&) const;
	bool operator!=(const double&) const;

	bool operator==(const Constant&)const;
	bool operator!=(const Constant&)const;

	bool operator==(const Variable&) const;
	bool operator!=(const Variable&) const;

	bool operator==(const Term&) const;
	bool operator!=(const Term&) const;

	bool operator==(const Expression&) const;
	bool operator!=(const Expression&) const;

	bool operator==(const Operand&) const;
	bool operator!=(const Operand&) const;

	~Constant();


	//Unary operators 
	Operand operator+() const;
	Operand operator-() const;

	Operand multiplicative_inverse() const;

	friend std::ostream& operator<<(std::ostream&, const Constant&);

};

#endif