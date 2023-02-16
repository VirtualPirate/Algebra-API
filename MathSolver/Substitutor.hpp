#ifndef Substitutor_H
#define Substitutor_H

#include "Operand.hpp"
#include <vector>
#include <initializer_list>

class Operand;

class Variable_Subtitutor {
	char name;
	double value;

public:
	Variable_Subtitutor();
	Variable_Subtitutor(char, double);

	bool operator==(char) const;
	bool operator!=(char) const;
	bool operator==(const Variable_Subtitutor&) const;
	bool operator!=(const Variable_Subtitutor&) const;

	char getName() const;
	double getValue() const;

	inline operator bool() const;
};

class Variable_Subtitutor_List { 
	std::vector<Variable_Subtitutor> subtitute;
public:
	Variable_Subtitutor_List() = default;
	Variable_Subtitutor_List(std::initializer_list<Variable_Subtitutor>);
	Variable_Subtitutor_List& operator=(std::initializer_list<Variable_Subtitutor>);

	Variable_Subtitutor_List(const std::vector<Variable_Subtitutor>&);
	Variable_Subtitutor_List& operator=(const std::vector<Variable_Subtitutor>&);

	// Substitutor List modifier
	void append(const Variable_Subtitutor&);
	void replace(const Variable_Subtitutor&);
	void extend(std::initializer_list<Variable_Subtitutor>);
	void extend(const std::vector<Variable_Subtitutor>&);
	void replace_all(std::initializer_list<Variable_Subtitutor>);
	void replace_all(const std::vector<Variable_Subtitutor>&);

	void erase(size_t);
	void erase(const Variable_Subtitutor&);
	void clear();

	size_t size() const;

	//Getters and Setters
	const Variable_Subtitutor& get_substitute(char) const;
};

class Substitutor{
	Operand key_value;
	Operand substitute;

	friend class Substitutor_List;
public:
	Substitutor(const Operand&, const Operand&);
	Substitutor(const Substitutor&);
	Substitutor(Substitutor&&);
	Substitutor();

	bool operator==(const Operand&) const;
	bool operator!=(const Operand&) const;
	bool operator==(const Substitutor&) const;
	bool operator!=(const Substitutor&) const;

	Substitutor& operator=(const Substitutor&);
	Substitutor& operator=(Substitutor&&);

	const Operand& getSubstitute() const;
	const Operand& getKey() const;

	inline operator bool() const;
};

class Substitutor_List{
	std::vector<Substitutor> sub_;
public:
	Substitutor_List(std::initializer_list<Substitutor>);
	Substitutor_List& operator=(std::initializer_list<Substitutor>);

	Substitutor_List(const std::vector<Substitutor>&);
	Substitutor_List& operator=(const std::vector<Substitutor>&);

	// Substitutor List modifier
	void append(const Substitutor&);
	void replace(const Substitutor&);
	void extend(std::initializer_list<Substitutor>);
	void extend(const std::vector<Substitutor>&);
	void replace_all(std::initializer_list<Substitutor>);
	void replace_all(const std::vector<Substitutor>&);

	void erase(size_t);
	void erase(const Substitutor&);
	void clear();

	size_t size() const;

	//Getters and Setters
	const Substitutor& get(const Substitutor&) const;
	const Operand& get_substitute(const Substitutor&) const;
};

#endif