#include "Operand.hpp"
#include "Substitutor.hpp"
#include "Constant.hpp"
#include "Variable.hpp"
#include "Operation_Functions.hpp"
#include <cstdlib>

Variable_Subtitutor::Variable_Subtitutor() : name{ '\0' }, value{ 0.0 }{}

Variable_Subtitutor::Variable_Subtitutor(char name_, double value_) {
	if (isalpha(name_)) {
		name = name_;
		value = value_;
	}
	else {
		name = '\0';
		value = 0.0;
	}
}

bool Variable_Subtitutor::operator==(char name_) const {
	return name == name_;
}
bool Variable_Subtitutor::operator!=(char name_) const {
	return name != name_;
}
bool Variable_Subtitutor::operator==(const Variable_Subtitutor& ref) const {
	return name == ref.name;
}
bool Variable_Subtitutor::operator!=(const Variable_Subtitutor& ref) const {
	return name != ref.name;
}

char Variable_Subtitutor::getName() const { return name; }
double Variable_Subtitutor::getValue() const { return value; }







Variable_Subtitutor_List::Variable_Subtitutor_List(std::initializer_list<Variable_Subtitutor> list) : subtitute{} { this->extend(list); }
Variable_Subtitutor_List& Variable_Subtitutor_List::operator=(std::initializer_list<Variable_Subtitutor> list) {
	subtitute = std::vector<Variable_Subtitutor>{};
	this->extend(list);
	return *this;
}
Variable_Subtitutor_List::Variable_Subtitutor_List(const std::vector<Variable_Subtitutor>& vec) : subtitute{} { this->extend(vec); }
Variable_Subtitutor_List& Variable_Subtitutor_List::operator=(const std::vector<Variable_Subtitutor>& vec) {
	subtitute = std::vector<Variable_Subtitutor>{};
	this->extend(vec);
	return *this;
}

void Variable_Subtitutor_List::append(const Variable_Subtitutor& val) {
	if (std::find(subtitute.begin(), subtitute.end(), val) == subtitute.end())
		subtitute.push_back(val);
}

void Variable_Subtitutor_List::replace(const Variable_Subtitutor& val) {
	auto iter = std::find(subtitute.begin(), subtitute.end(), val);
	if (iter == subtitute.end())
		subtitute.push_back(val);
	else
		*iter = val;
}

void Variable_Subtitutor_List::extend(std::initializer_list<Variable_Subtitutor> list) {
	for (auto each = list.begin(); each != list.end(); each++)
		this->append(*each);
}
void Variable_Subtitutor_List::extend(const std::vector<Variable_Subtitutor>& vec) {
	for (const Variable_Subtitutor& each : vec)
		this->append(each);
}

void Variable_Subtitutor_List::replace_all(std::initializer_list<Variable_Subtitutor> list) {
	for (auto each = list.begin(); each != list.end(); each++)
		this->replace(*each);
}
void Variable_Subtitutor_List::replace_all(const std::vector<Variable_Subtitutor>& vec) {
	for (const Variable_Subtitutor& each : vec)
		this->append(each);
}

void Variable_Subtitutor_List::erase(size_t index) {
	subtitute.erase(subtitute.begin() + index);
}
void Variable_Subtitutor_List::erase(const Variable_Subtitutor& ref) {
	auto iter = std::find(subtitute.begin(), subtitute.end(), ref);
	if (iter != subtitute.end())
		subtitute.erase(iter);
}

inline void Variable_Subtitutor_List::clear() {
	subtitute.clear();
}

size_t Variable_Subtitutor_List::size() const {
	return subtitute.size();
}

const Variable_Subtitutor& Variable_Subtitutor_List::get_substitute(char name_) const {
	auto iter = std::find(subtitute.begin(), subtitute.end(), name_);
	if (iter != subtitute.end())
		return *iter;
	else
		return CONSTANTS::VAR_NULL_SUBTITUTE;
}









Substitutor::Substitutor(const Operand& k_, const Operand& s_): key_value{k_}, substitute{s_}{}
Substitutor::Substitutor(const Substitutor& ref): key_value{ref.key_value}, substitute{ref.substitute}{}
Substitutor::Substitutor(Substitutor&& ref): key_value{std::move(ref.key_value)}, substitute{std::move(ref.substitute)}{}
Substitutor::Substitutor(): key_value{}, substitute{}{}

bool Substitutor::operator==(const Operand& other) const {
	if(key_value == other)
		return true;
	return false;
}

bool Substitutor::operator!=(const Operand& other) const {
	if(key_value != other)
		return true;
	return false;
}

bool Substitutor::operator==(const Substitutor& other) const {
	if(key_value == other.key_value)
		return true;
	return false;
}
bool Substitutor::operator!=(const Substitutor& other) const {
	if(key_value != other.key_value)
		return true;
	return false;
}
Substitutor& Substitutor::operator=(const Substitutor& ref){
	key_value = ref.key_value;
	substitute = ref.substitute;
	return *this;
}
Substitutor& Substitutor::operator=(Substitutor&& ref){
	key_value = std::move(ref.key_value);
	substitute = std::move(ref.substitute);
	return *this;
}

const Operand& Substitutor::getSubstitute() const {
	return substitute;
}

const Operand& Substitutor::getKey() const {
	return key_value;
}

inline Substitutor::operator bool() const{
	return key_value && substitute;

}

//Substitutor_List

Substitutor_List::Substitutor_List(std::initializer_list<Substitutor> list): sub_{}{this->extend(list);}
Substitutor_List& Substitutor_List::operator=(std::initializer_list<Substitutor> list){
	sub_ = std::vector<Substitutor>{};
	this->extend(list);
	return *this;
}

Substitutor_List::Substitutor_List(const std::vector<Substitutor>& vec): sub_{}{this->extend(vec);}
Substitutor_List& Substitutor_List::operator=(const std::vector<Substitutor>& vec){
	sub_ = std::vector<Substitutor>{};
	this->extend(vec);
	return *this;
}

//Substitutor_List modifers
void Substitutor_List::append(const Substitutor& val){
	if(std::find(sub_.begin(), sub_.end(), val) == sub_.end())
		sub_.push_back(val);
}
void Substitutor_List::replace(const Substitutor& val){
	auto iter = std::find(sub_.begin(), sub_.end(), val);
	if(iter == sub_.end())
		sub_.push_back(val);
	else
		*iter = val;
}

void Substitutor_List::extend(std::initializer_list<Substitutor> list){
	for(auto each=list.begin(); each!= list.end(); each++)
		this->append(*each);
}
void Substitutor_List::extend(const std::vector<Substitutor>& vec){
	for(const Substitutor& each: vec)
		this->append(each);
}
void Substitutor_List::replace_all(std::initializer_list<Substitutor> list){
	for(auto each=list.begin(); each!=list.end(); each++)
		this->replace(*each);
}
void Substitutor_List::replace_all(const std::vector<Substitutor>& vec){
	for(const Substitutor& each: vec)
		this->append(each);
}

void Substitutor_List::erase(size_t index){
	sub_.erase(sub_.begin() + index);
}
void Substitutor_List::erase(const Substitutor& ref){
	auto iter = std::find(sub_.begin(), sub_.end(), ref);
	if(iter != sub_.end())
		sub_.erase(iter);
}

inline void Substitutor_List::clear(){
	sub_.clear();
}

size_t Substitutor_List::size() const {
	return sub_.size();
}

const Substitutor& Substitutor_List::get(const Substitutor& ref) const{
	auto iter = std::find(sub_.begin(), sub_.end(), ref);
	if(iter != sub_.end())
		return *iter;
	else
		return CONSTANTS::null_substitutor;
}
const Operand& Substitutor_List::get_substitute(const Substitutor& ref) const{
	auto iter = std::find(sub_.begin(), sub_.end(), ref);
	if(iter != sub_.end())
		return iter->substitute;
	else
		return CONSTANTS::NULL_OPERAND;
}