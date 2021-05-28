
#pragma once

namespace list {

	template<typename T>
	class ListElement {
		ListElement<T>* prev;
		T element;
		ListElement<T>* next;

		explicit ListElement(T element);
		ListElement(ListElement<T>* prev, ListElement<T>* next, T element);
	};

}
