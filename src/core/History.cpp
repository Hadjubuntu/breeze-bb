/*
 * History.cpp
 *
 *  Created on: Sep 15, 2015
 *      Author: adrien
 */

#include <string>
#include "../math/common/FastMath.h"
#include "History.h"


template<class T>
void History<T>::add(T element)
{
	if (_history.size() < _size)
	{
		_history.push_back(element);
	}
	else {
		_history.at(currentIdx) = element;
	}

	currentIdx ++ ;

	if (currentIdx >= _size)
	{
		currentIdx = 0;
	}
	//	// Erase too old elements
	//	if (_history.size() > _size) {
	//		_history.erase(_history.begin(), _history.begin()+(_history.size()-_size));
	//		_history.clear();
	//	}
}

template<class T>
std::vector<T> History<T>::toVector()
{
	std::vector<T> orderedVector;

	if (_history.size() >= _size) {
		for (int i=currentIdx; i < _size; i ++) {
			orderedVector.push_back(_history.at(i));
		}
	}

	for (int j=0; j < currentIdx; j ++)
	{
		orderedVector.push_back(_history.at(j));
	}

	return orderedVector;
}

template<class T>
void History<T>::clear()
{
	_history.clear();
}


template class History<std::string>;
template class History<int>;
template class History<float>;

