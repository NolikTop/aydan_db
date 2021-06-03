#include <algorithm>
#include "List.h"
#include "Exception.h"

using namespace list;


template<typename T>
size_t List<T>::len() {
	size_t count = 0;
	for(auto i = this->first; i != nullptr; i = i->next, count++){}
	return count;
}

template<typename T>
void List<T>::push_back(T* value) {
	if(this->last == nullptr){
		this->first = this->last = new ListElement<T*>(value);
		return;
	}

	auto el = this->last->next = new ListElement<T*>(value);
	el->prev = this->last;

	this->last = el;
}

template<typename T>
void List<T>::push_front(T* value) {
	if(this->first == nullptr){
		this->first = this->last = new ListElement<T>(value);
		return;
	}

	auto el = new ListElement<T>(value);
	this->first->prev = el;
	el->next = this->first;

	this->first = el;
}

template<typename T>
void List<T>::set(size_t index, T* value) {
	auto el = this->listElementAt(index);

	delete el.element;

	el.element = value;
}

template<typename T>
void List<T>::remove(size_t index) {
	auto el = this->listElementAt(index);

	if(el->prev == nullptr){ // первый элемент
		this->first = el->next;
		el->next->prev = nullptr;
	}else if(el->next == nullptr){ // последний элемент
		this->last = el->prev;
		el->prev->next = nullptr;
	}else{ // где-то между первым и последним элементами
		el->prev->next = el->next;
		el->next->prev = el->prev;
	}

	delete el.element;
	delete el;
}

template<typename T>
T* List<T>::at(size_t index) {
	return this->listElementAt(index).element;
}

template<typename T>
ListElement<T> List<T>::listElementAt(size_t index) {
	size_t j = 0;
	auto i = this->first;
	for(; i != nullptr && j != index; i = i->next, ++j){}

	if(i == nullptr){
		throw Exception("Not found");
	}

	return i;
}

template<typename T>
void List<T>::clear() {
	for(auto i = this->first; i != nullptr; i = i->next){
		delete i->element;
		delete i;
	}
	this->first = nullptr;
	this->last = nullptr;
}
