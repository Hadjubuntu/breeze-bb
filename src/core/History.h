/*
 * History.h
 *
 *  Created on: Sep 15, 2015
 *      Author: adrien
 */

#ifndef CORE_HISTORY_H_
#define CORE_HISTORY_H_

#include <vector>

/**
 * Class keeping history of a generic object T
 */
template <class T>
class History {
private:
	unsigned int _size;
	std::vector<T> _history;
public:
	/**
	 * Constructor
	 */
	History(unsigned int size) : _size(size) {
		_history.reserve(_size);
	}


	/**
	 * Add element to history
	 */
	void add(T e);

	/**
	 * Erase history
	 */
	void clear();

	int getSize() {
		return _size;
	}

	std::vector<T> toVector() {
		return _history;
	}

	T getLast() {
		return _history.at(_size - 1);
	}

	/**
	 * Create default 10-elements history
	 */
	static History<T> getDefault() {
		return create(10); // Default size : 10 elements
	}
	static History<T> create(int n) {
		History<T> e(n);
		return e;
	}
};

#endif /* CORE_HISTORY_H_ */
