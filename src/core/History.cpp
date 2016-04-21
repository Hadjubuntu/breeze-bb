/*
 * History.cpp
 *
 *  Created on: Sep 15, 2015
 *      Author: adrien
 */

#include <string>
#include "History.h"


template<class T>
void History<T>::add(T element)
{
	_history.push_back(element);

	// Erase too old elements
	if (_history.size() > _size) {
		_history.erase(_history.begin(), _history.begin()+(_history.size()-_size));
		_history.clear();
	}
}


template<class T>
void History<T>::clear()
{
	_history.clear();
}


template class History<std::string>;
template class History<int>;
template class History<float>;

