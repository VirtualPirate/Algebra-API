#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <initializer_list>

#include "Substitutor.hpp"
#include "Operand.hpp"
#include "Constant.hpp"
#include "Variable.hpp"
#include "OperandList.hpp"
#include "Expression.hpp"
#include "Operation_Functions.hpp"

#include "Tests.hpp"

// std::vector<Operand> OperandList::internal_simplify() const {
// 	std::vector<Operand> result;
// 	for(const auto& each: fields)
// 		result.push_back(each.simplify());
// 	return result;
// }
OperandList::OperandList() : fields{}, power{ (double)1 }, is_simplified{ false }, is_null{ true }{}

OperandList::OperandList(const OperandList& ref): fields{ref.fields}, power{ref.power}, is_simplified{ref.is_simplified}, is_null{ref.is_null}{}
OperandList& OperandList::operator=(const OperandList& ref){
	fields = ref.fields;
	power = ref.power;
	is_simplified = ref.is_simplified;
	is_null = ref.is_null;
	return *this;
}
OperandList::OperandList(OperandList&& ref): fields{std::move(ref.fields)}, power{std::move(ref.power)}, is_simplified{ref.is_simplified}, is_null{ref.is_null}{
	ref.is_null = true;
}
OperandList& OperandList::operator=(OperandList&& ref){
	fields = std::move(ref.fields);
	power = std::move(ref.power);
	is_simplified = ref.is_simplified;
	is_null = ref.is_null;

	ref.is_null = true;
	return *this;
}



OperandList::OperandList(const Constant& ref): fields{{Operand{ref}}}, power{(double)1}, is_simplified{true}, is_null{false}{}
OperandList& OperandList::operator=(const Constant& ref){
	fields.clear();
	fields.push_back(Operand{ref});
	power = (double)1;
	is_simplified = true;
	is_null = false;
	return *this;
}
OperandList::OperandList(const Variable& ref): fields{{Operand{ref}}}, power{(double)1}, is_simplified{true}, is_null{false}{}
OperandList& OperandList::operator=(const Variable& ref){
	fields.clear();
	fields.push_back(Operand{ref});
	power = (double)1;
	is_simplified = true;
	is_null = false;
	return *this;
}
OperandList::OperandList(const Operand& ref) : fields{ {ref} }, power{ (double)1 }, is_simplified{ true }, is_null{ false }{}
OperandList& OperandList::operator=(const Operand& ref){
	fields.clear();
	fields.push_back(ref);
	power = (double)1;
	is_simplified = true;
	is_null = false;
	return *this;
}

OperandList::OperandList(const std::vector<Operand>& ref): fields{ref}, power{(double)1}, is_simplified{false}, is_null{false}{}
OperandList::OperandList(const std::vector<Operand>& ref, const Operand& power_) : fields{ ref }, power{ power_ }, is_simplified{ false }, is_null{ false }{}
OperandList::OperandList(std::initializer_list<Operand> list): fields{list}, power{(double)1}, is_simplified{false}, is_null{false}{}
OperandList::OperandList(std::initializer_list<Operand> list, const Operand& power_) : fields{ list }, power{ power_ }, is_simplified{ false }, is_null{ false }{}
OperandList& OperandList::operator=(std::initializer_list<Operand> list){
	fields = list;
	power = (double)1;
	is_simplified = false;
	is_null = false;
	return *this;
}

// Getter methods
const std::vector<Operand>& OperandList::getFields() const {
	return fields;
}
const Operand& OperandList::getPower() const {
	return power;
}
const bool& OperandList::isSimplified() const {
	return is_simplified;
}
const bool& OperandList::isNull() const {
	return is_null;
}

//Setter Methods
void OperandList::setNull(const bool& ref){
	is_null = ref;
}
void OperandList::setPower(const Operand& ref){
	is_simplified = false;
	power = ref;
}

//field modifiers
void OperandList::insert(const Constant& ref){
	is_simplified = false;
	fields.push_back(Operand{ref});
}
void OperandList::insert(const Variable& ref){
	is_simplified = false;
	fields.push_back(Operand{ref});
}
void OperandList::insert(const Term& ref){
	is_simplified = false;
	fields.push_back(Operand{ref});
}
void OperandList::insert(const Expression& ref){
	is_simplified = false;
	fields.push_back(Operand{ref});
}
void OperandList::insert(const Operand& ref){
	is_simplified = false;
	fields.push_back(ref);
}
void OperandList::insert(Operand&& ref){
	is_simplified = false;
	fields.push_back(std::move(ref));
}

void OperandList::erase(const const_iterator& iter) {
	fields.erase(fields.begin() + iter.getIndex());
}

void OperandList::insert_front(const Operand& ref){
	is_simplified = false;
	fields.insert(fields.begin(), ref);
}
void OperandList::insert_front(Operand&& ref){
	is_simplified = false;
	fields.insert(fields.begin(), std::move(ref));
}

//Iterator functions
OperandList::iterator OperandList::begin(DataType type){
	if(fields.at(0).getType() == type)
		return OperandList::iterator{this, 0, type};
	return OperandList::iterator{this, 0, type} + 1;
}
OperandList::iterator OperandList::end(DataType type) {
	return OperandList::iterator{this, (int)this->fields.size(), type};
}

OperandList::const_iterator OperandList::cbegin(DataType type) const {
	if(fields.at(0).getType() == type)
		return OperandList::const_iterator{this, 0, type};
	return OperandList::const_iterator(this, 0, type) + 1;
}
OperandList::const_iterator OperandList::cend(DataType type) const {
	return OperandList::const_iterator{this, (int)this->fields.size(), type};
}

std::vector<Operand> OperandList::substitute(const std::vector<Substitutor>& ref) const {
	std::vector<Operand> vec = fields;
	for(Operand& each: vec){
		auto iter = std::find(ref.begin(), ref.end(), each);
		if(iter != ref.end())
			each = iter->getSubstitute();
	}
	return vec;
}

unsigned int OperandList::count(DataType type) const {
	unsigned int count = 0;
	for(auto each=cbegin(type); each!=cend(type); each++)
		count++;
	return count;
}

OperandList::operator bool() const {return is_null;}
// std::ostream& operator<<(std::ostream& os, const OperandList& ref){
// 	if(!ref.is_null){
// 	os << *(ref.fields.begin());
// 	for(auto each=ref.fields.begin()+1; each!=ref.fields.end(); each++)
// 		if(each->getType() == DataType::Constant)
// 			os << " * " << *each;
// 		else
// 			os << *each;
// 	}
// 	else{
// 		os << "null_term{}";
// 	}
// 	return os;
// }

// OperandList::iterator definition
OperandList::iterator::iterator(): index{0}{}
OperandList::const_iterator::const_iterator(): index{0}{}
OperandList::iterator::iterator(const OperandList::iterator& ref): ref{ref.ref}, index{ref.index}, iterative{ref.iterative}{}
OperandList::const_iterator::const_iterator(const OperandList::const_iterator& ref): ref{ref.ref}, index{ref.index}, iterative{ref.iterative}{}
OperandList::const_iterator::const_iterator(const OperandList::iterator& ref) : ref{ ref.ref }, index{ ref.index }, iterative{ ref.iterative }{}
OperandList::iterator::iterator(OperandList* point, int index_, DataType type): ref{point}, index{index_}, iterative{type}{}
OperandList::const_iterator::const_iterator(const OperandList* point, int index_, DataType type): ref{point}, index{index_}, iterative{type}{}

OperandList::iterator& OperandList::iterator::operator=(const OperandList::iterator& ref){
	this->ref = ref.ref;
	index = ref.index;
	return *this;
}
OperandList::const_iterator& OperandList::const_iterator::operator=(const OperandList::const_iterator& ref){
	this->ref = ref.ref;
	index = ref.index;
	return *this;
}

void OperandList::iterator::set_iterative(const DataType& type){
	iterative = type;
	index = 0;
}
void OperandList::const_iterator::set_iterative(const DataType& type){
	iterative = type;
	index = 0;
}

int OperandList::iterator::getIndex() const {
	return index;
}
int OperandList::const_iterator::getIndex() const {
	return index;
}

//OperandList::iterator operations
int OperandList::iterator::operate_add(int other){
	int abs_index = this->index + 1;
	int rel_index = 0;
	while(abs_index < ref->fields.size() && rel_index < other){
		if(this->ref->fields.at(abs_index).getType() == iterative){
			rel_index++;
			if(rel_index == other)
				break;
		}
		abs_index++;
	}
	return abs_index;
}
int OperandList::const_iterator::operate_add(int other){
	int abs_index = this->index + 1;
	int rel_index = 0;
	while(abs_index < ref->fields.size() && rel_index < other){
		if(this->ref->fields.at(abs_index).getType() == iterative){
			rel_index++;
			if(rel_index == other)
				break;
		}
		abs_index++;
	}
	return abs_index;
}
int OperandList::iterator::operate_sub(int other){
	int abs_index = this->index - 1;
	int rel_index = 0;
	while(abs_index > 0 && rel_index < other){
		if(this->ref->fields.at(abs_index).getType() == iterative){
			rel_index++;
			if(rel_index == other)
				break;
		}
		abs_index--;
	}
	return abs_index;
}
int OperandList::const_iterator::operate_sub(int other){
	int abs_index = this->index - 1;
	int rel_index = 0;
	while(abs_index > 0 && rel_index < other){
		if(this->ref->fields.at(abs_index).getType() == iterative){
			rel_index++;
			if(rel_index == other)
				break;
		}
		abs_index--;
	}
	return abs_index;
}
OperandList::iterator OperandList::iterator::operator+(int other){
	return OperandList::iterator{this->ref, operate_add(other), this->iterative};
}
OperandList::const_iterator OperandList::const_iterator::operator+(int other){
	return OperandList::const_iterator{this->ref, operate_add(other), this->iterative};
}

OperandList::iterator OperandList::iterator::operator-(int other){
	return OperandList::iterator{this->ref, operate_sub(other), this->iterative};
}
OperandList::const_iterator OperandList::const_iterator::operator-(int other){
	return OperandList::const_iterator{this->ref, operate_sub(other), this->iterative};
}

OperandList::iterator& OperandList::iterator::operator+=(int other){
	this->index = operate_add(other);
	return *this;
}
OperandList::const_iterator& OperandList::const_iterator::operator+=(int other){
	this->index = operate_add(other);
	return *this;
}

OperandList::iterator& OperandList::iterator::operator-=(int other){
	this->index = operate_sub(other);
	return *this;
}
OperandList::const_iterator& OperandList::const_iterator::operator-=(int other){
	this->index = operate_sub(other);
	return *this;
}

//Pre-Increment methods
OperandList::iterator& OperandList::iterator::operator++(){
	this->index = operate_add(1);
	return *this;
}
OperandList::const_iterator& OperandList::const_iterator::operator++(){
	this->index = operate_add(1);
	return *this;
}

OperandList::iterator& OperandList::iterator::operator--(){
	this->index = operate_sub(1);
	return *this;
}
OperandList::const_iterator& OperandList::const_iterator::operator--(){
	this->index = operate_sub(1);
	return *this;
}

//Post-Increment methods
OperandList::iterator OperandList::iterator::operator++(int other){
	++(*this);
	return (*this) - 1;
}
OperandList::const_iterator OperandList::const_iterator::operator++(int other){
	++(*this);
	return (*this) - 1;
}
OperandList::iterator OperandList::iterator::operator--(int other){
	++(*this);
	return (*this) + 1;
}
OperandList::const_iterator OperandList::const_iterator::operator--(int other){
	++(*this);
	return (*this) + 1;
}

Operand& OperandList::iterator::operator*(){
	return ref->fields.at(index);
}
Operand* OperandList::iterator::operator->() {
	return &(ref->fields.at(index));
}


const Operand& OperandList::const_iterator::operator*() const {
	return ref->fields.at(index);
}
const Operand* OperandList::const_iterator::operator->() const {
	return &(ref->fields.at(index));
}

//Iterator comparison operators
bool OperandList::iterator::operator==(const OperandList::iterator& ref) const {
	return this->index == ref.index;
}
bool OperandList::const_iterator::operator==(const OperandList::const_iterator& ref) const {
	return this->index == ref.index;
}

bool OperandList::iterator::operator!=(const OperandList::iterator& ref) const {
	return this->index != ref.index;
}
bool OperandList::const_iterator::operator!=(const OperandList::const_iterator& ref) const {
	return this->index != ref.index;
}

void OperandList::internal_info() const {
	for(const auto& each: fields){
		std::cout << '(' << return_type_string(each.getType()) << ')' << each << " -- ";
	}
	std::cout << std::endl;
}