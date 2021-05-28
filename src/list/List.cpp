#include <algorithm>
#include "List.h"

using namespace list;

template<typename T>
void List<T>::push_back(T value) {
	if(this->last == nullptr){
		this->first = this->last = new ListElement<T>(value);
		return;
	}

	auto el = this->last->next = new ListElement<T>(value);
	el->prev = this->last;

	this->last = el;
}

template<typename T>
void List<T>::push_front(T value) {
	if(this->first == nullptr){
		this->first = this->last = new ListElement<T>(value);
		return;
	}
}

template<typename T>
void List<T>::set(size_t index, T value) {

}

template<typename T>
void List<T>::remove(size_t index) {

}

template<typename T>
void List<T>::remove_if(bool (*check)(T)) {

}

template<typename T>
void List<T>::clear() {

}
