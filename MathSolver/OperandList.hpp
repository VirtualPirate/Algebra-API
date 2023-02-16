#ifndef OperandList_H
#define OperandList_H

#include <vector>
#include <initializer_list>

class Substitutor;
class Constant;
class Variable;
class Operand;
class OperandList{
protected:
	std::vector<Operand> fields;
	Operand power;
	bool is_simplified;
	bool is_null;
	// std::vector<Operand> internal_simplify() const;
public:
	friend class iterator;
	class iterator{
		OperandList* ref;
		int index;
		DataType iterative;

		friend class OperandList;

		iterator();
		iterator(const iterator&);
		iterator(OperandList*, int, DataType);
		int operate_add(int);
		int operate_sub(int);
	public:
		iterator& operator=(const iterator&);
		void set_iterative(const DataType&);
		int getIndex() const;
		
		//operations
		iterator operator+(int);
		iterator operator-(int);
		iterator& operator+=(int);
		iterator& operator-=(int);

		iterator& operator++();
		iterator& operator--();
		iterator operator++(int);
		iterator operator--(int);

		Operand& operator*();
		Operand* operator->();

		//Comparison operator
		bool operator==(const iterator&) const;
		bool operator!=(const iterator&) const;

	};
	class const_iterator{
		const OperandList* ref;
		int index;
		DataType iterative;

		friend class OperandList;

		const_iterator();
		const_iterator(const const_iterator&);
		const_iterator(const OperandList*, int, DataType);
		int operate_add(int);
		int operate_sub(int);
	public:
		const_iterator(const iterator&);
		const_iterator& operator=(const const_iterator&);
		void set_iterative(const DataType&);
		int getIndex() const;

		//operations
		const_iterator operator+(int);
		const_iterator operator-(int);
		const_iterator& operator+=(int);
		const_iterator& operator-=(int);

		const_iterator& operator++();
		const_iterator& operator--();
		const_iterator operator++(int);
		const_iterator operator--(int);

		const Operand& operator*() const;
		const Operand* operator->() const;

		//Comparison operator
		bool operator==(const const_iterator&) const;
		bool operator!=(const const_iterator&) const;
	};

	OperandList();

	//Copy and Move semantics
	OperandList(const OperandList&);
	OperandList& operator=(const OperandList&);
	OperandList(OperandList&&);
	OperandList& operator=(OperandList&&);

	OperandList(const Constant&);
	OperandList& operator=(const Constant&);
	OperandList(const Variable&);
	OperandList& operator=(const Variable&);
	//OperandList(const Term&);
	//OperandList& operator=(const Term&);
	//OperandList(const Expression&);
	//OperandList& operator=(const Expression&);
	OperandList(const Operand&);
	OperandList& operator=(const Operand&);

	OperandList(const std::vector<Operand>&);
	OperandList(const std::vector<Operand>&, const Operand&);
	OperandList(std::initializer_list<Operand>);
	OperandList(std::initializer_list<Operand>, const Operand&);
	OperandList& operator=(std::initializer_list<Operand>);

	OperandList(std::string);


	//Getter Methods
	const std::vector<Operand>& getFields() const;
	const Operand& getPower() const;
	const bool& isSimplified() const;
	const bool& isNull() const;

	//Setter Methods
	void setNull(const bool&);
	void setPower(const Operand&);

	void insert(const Constant&);
	void insert(const Variable&);
	void insert(const Term&);
	void insert(const Expression&);
	void insert(const Operand&);
	void insert(Operand&&);

	void insert_front(const Operand&);
	void insert_front(Operand&&);

	void erase(const const_iterator&);

	// bool is_Constant() const;
	// bool is_Variable() const;
	bool var_exist(const char&) const;

	void simplify();
	std::vector<Operand> substitute(const std::vector<Substitutor>&) const;

	//Iterator functions
	iterator begin(DataType);
	iterator end(DataType);
	const_iterator cbegin(DataType) const;
	const_iterator cend(DataType) const;
	unsigned int count(DataType) const;
	/*
	// OperandList to Constant comparison operators
	bool operator==(const Constant&) const;
	bool operator!=(const Constant&) const;

	// OperandList to Variable comparison operators
	bool operator==(const Variable&) const;
	bool operator!=(const Variable&) const;

	//OperandList to OperandList comparison operators
	bool operator==(const OperandList&) const;
	bool operator!=(const OperandList&) const;

	//OperandList to Constant operations
	OperandList operator+(const Constant&) const;
	OperandList operator-(const Constant&) const;
	OperandList operator*(const Constant&) const;
	OperandList operator/(const Constant&) const;
	OperandList raise_pow(const Constant&) const;

	//OperandList to Variable operations
	OperandList operator+(const Variable&) const;
	OperandList operator-(const Variable&) const;
	OperandList operator*(const Variable&) const;
	OperandList operator/(const Variable&) const;
	OperandList raise_pow(const Variable&) const;

	//OperandList to OperandList operations
	OperandList operator+(const OperandList&) const;
	OperandList operator-(const OperandList&) const;
	OperandList operator*(const OperandList&) const;
	OperandList operator/(const OperandList&) const;
	OperandList raise_pow(const OperandList&) const;

	//Pre-increment
	OperandList operator++();
	OperandList operator--();

	//Post-increment
	*/
	operator bool() const;

	void internal_info() const;
	// std::string power_print() const;
	// friend std::ostream& operator<<(std::ostream&, const OperandList&);

};

#endif