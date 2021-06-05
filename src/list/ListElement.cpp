#include <db/Row.h>
#include <parser/type/TypeToken.h>
#include "ListElement.h"

using namespace list;

template<typename T>
ListElement<T>::ListElement(T* element): prev(nullptr), next(nullptr), element(element) {}

template<typename T>
ListElement<T>::ListElement(ListElement<T>* prev, ListElement<T>* next, T* element): prev(prev), next(next), element(element) {}

template class list::ListElement<db::Row>;
template class list::ListElement<db::Column>;
template class list::ListElement<parser::UserValueBaseToken>;
template class list::ListElement<parser::type::TypeToken>;
template class list::ListElement<int>;