/*
 * svec.hpp
 *
 *  Created on: Jan 18, 2019
 *      Author: dmarce1
 */

#ifndef SVEC_HPP_
#define SVEC_HPP_

#include <array>

template<class T>
class svec {
	std::array<T, NDIM> a_;
public:
	svec() = default;
	svec(const svec<T>&) = default;
	svec(svec<T> &&) = delete;
	svec<T>& operator=(const svec<T>&) = default;
	svec<T>& operator=(svec<T> &&) = delete;
	svec<T>& operator=(const T&);
	svec<T>(const T&);
	svec<T>& operator*(const T&);
	svec<T>& operator/(const T&);
	svec<T>& operator*=(const T&);
	svec<T>& operator/=(const T&);
	svec<T>& operator+(const svec<T>&);
	svec<T>& operator-(const svec<T>&);
	svec<T>& operator+=(const svec<T>&);
	svec<T>& operator-=(const svec<T>&);
	T& operator[](int);
	const T& operator[](int) const;
	T dot(const svec<T>&);
};

template<class T>
svec<T>& svec<T>::operator+(const svec<T>& b) {
	svec<T> c;
	for (int d = 0; d < NDIM; d++) {
		c.a_[d] = a_[d] + b.a_[d];
	}
	return c;
}

template<class T>
svec<T>& svec<T>::operator-(const svec<T>& b) {
	svec<T> c;
	for (int d = 0; d < NDIM; d++) {
		c.a_[d] = a_[d] - b.a_[d];
	}
	return c;
}

template<class T>
svec<T>& svec<T>::operator*(const T& b) {
	svec<T> c;
	for (int d = 0; d < NDIM; d++) {
		c.a_[d] = a_[d] * b;
	}
	return c;
}

template<class T>
svec<T>& svec<T>::operator/(const T& b) {
	svec<T> c;
	for (int d = 0; d < NDIM; d++) {
		c.a_[d] = a_[d] / b;
	}
	return c;
}

template<class T>
svec<T>& svec<T>::operator=(const T& b) {
	for (int d = 0; d < NDIM; d++) {
		a_[d] = b;
	}
	return *this;
}

template<class T>
svec<T>::svec(const T& b) {
	for (int d = 0; d < NDIM; d++) {
		a_[d] = b;
	}
}

template<class T>
svec<T>& svec<T>::operator+=(const svec<T>& b) {
	for (int d = 0; d < NDIM; d++) {
		a_[d] += b.a_[d];
	}
	return *this;
}

template<class T>
svec<T>& svec<T>::operator-=(const svec<T>& b) {
	for (int d = 0; d < NDIM; d++) {
		a_[d] -= b.a_[d];
	}
	return *this;
}

template<class T>
svec<T>& svec<T>::operator*=(const T& b) {
	for (int d = 0; d < NDIM; d++) {
		a_[d] *= b;
	}
	return *this;
}

template<class T>
svec<T>& svec<T>::operator/=(const T& b) {
	for (int d = 0; d < NDIM; d++) {
		a_[d] /= b;
	}
	return *this;
}

template<class T>
T& svec<T>::operator[](int i) {
	return a_[i];
}

template<class T>
const T& svec<T>::operator[](int i) const {
	return a_[i];
}

template<class T>
T svec<T>::dot(const svec<T>& b) {
	T c = a_[0] * b.a_[0];
	for (int d = 1; d < NDIM; d++) {
		c += a_[d] * b.a_[d];
	}
	return c;
}

template<class T>
svec<T> operator*(const T& a, const svec<T>& b) {
	return b * a;
}

#endif /* SVEC_HPP_ */
