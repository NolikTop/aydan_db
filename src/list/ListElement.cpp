#include "ListElement.h"

using namespace list;

template<typename T>
ListElement<T>::ListElement(T element): prev(nullptr), next(nullptr), element(element) {}

template<typename T>
ListElement<T>::ListElement(ListElement<T>* prev, ListElement<T>* next, T element): prev(prev), next(next), element(element) {}