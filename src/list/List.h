
#pragma once

#include <cstdlib>
#include "ListElement.h"

namespace list {

	template<typename T>
	class List {
		ListElement<T>* first;
		ListElement<T>* last;
		size_t len;

		void push_back(T value);
		void push_front(T value);

		void set(size_t index, T value);

		void remove(size_t index);
		void remove_if(bool (*check)(T));

		void clear();
	};

}


