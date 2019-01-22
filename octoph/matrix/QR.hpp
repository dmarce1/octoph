/*
 * QR.hpp
 *
 *  Created on: Jan 21, 2019
 *      Author: dmarce1
 */

#ifndef OCTOPH_MATRIX_QR_HPP_
#define OCTOPH_MATRIX_QR_HPP_


#include <matrix/matrix.hpp>

namespace linear {

template<class T, std::size_t N>
void QR_decomposition(const T& A, T& Q, T& R) {
	using column_type = matrix<typename T::value_type,N,1>;
	column_type Acol[N];
	column_type Ucol[N];

	for (int n = 0; n < N; n++) {
		for (int m = 0; m < N; m++) {
			Acol[m](n, 1) = A(n, m);
		}
	}
	const auto proj = [](const column_type& u, const column_type& a) {
		const auto Ta = transpose(a);
		const auto p = product(u,Ta);
		return p.template get<0,0>();
	};
	for (int n = 0; n < N; n++) {
		Ucol[n] = Acol[n];
		for (int m = 0; m < n; m++) {
			const auto tmp =  proj(Ucol[m], Acol[n]);
			Ucol[n](m,1) -= tmp;
		}
	}
}
}



#endif /* OCTOPH_MATRIX_QR_HPP_ */
