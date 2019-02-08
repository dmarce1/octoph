/*
 * containers.hpp
 *
 *  Created on: Jan 31, 2019
 *      Author: dmarce1
 */

#ifndef OCTOPH_MATH_CONTAINERS_HPP_
#define OCTOPH_MATH_CONTAINERS_HPP_

#include <array>
#include <memory>

namespace math {

template<class, int, bool>
struct array_container;

template<class T, int Size>
struct array_container<T, Size, true> {
	inline array_container() :
			ptr_(std::make_shared<std::array<T, Size>>()), data_(ptr_->data()) {
	}
	inline const T& operator[](int i) const {
		return data_[i];
	}
	inline T& operator[](int i) {
		return data_[i];
	}
	inline array_container make_copy() const {
		return array_container(ptr_);
	}
private:
	inline array_container(const std::shared_ptr<std::array<T, Size>>& ptr) :
			ptr_(ptr), data_(ptr_->data()) {
	}
	std::shared_ptr<std::array<T, Size>> ptr_;
	T* const data_;
};

template<class T, int Size>
struct array_container<T, Size, false> {
	inline array_container() :
			data_() {
	}
	inline const T& operator[](int i) const {
		return data_[i];
	}
	inline T& operator[](int i) {
		return data_[i];
	}
	inline array_container make_copy() const {
		return array_container(data_);
	}
private:
	inline array_container(const std::array<T, Size>& data) :
			data_(data) {
	}
	std::array<T, Size> data_;
};

#define BINARY_OP( op )                                                                               \
		template<class B>                                                                             \
		inline auto operator op ( const B& other ) const {                                            \
			struct op_type  : public operator_base<value_type,op_type> {                              \
				op_type( const A& a, const B& b ) :                                                   \
					a_(a.make_copy()), b_(b.make_copy()), operator_base<value_type, op_type>(*this) { \
				}                                                                                     \
				inline auto operator[](int i) const {                                                 \
					return a_[i] op b_[i];                                                            \
				}                                                                                     \
				inline op_type make_copy() const {                                                    \
					return op_type(a_,b_);                                                            \
				}                                                                                     \
			private:                                                                                  \
				const A a_;                                                                           \
				const B b_;                                                                           \
			};                                                                                        \
			return op_type( a_, other.a_);                                                            \
		}                                                                                             \
		inline auto operator op ( const T& b ) const {                                                \
			struct op_type  : public operator_base<value_type,op_type> {                              \
				op_type( const A& a, const T& b ) :                                                   \
					a_(a.make_copy()), b_(b), operator_base<value_type, op_type>(*this) {             \
				}                                                                                     \
				inline auto operator[](int i) const {                                                 \
					return a_[i] op b_;                                                               \
				}                                                                                     \
				inline op_type make_copy() const {                                                    \
					return op_type(a_,b_);                                                            \
				}                                                                                     \
			private:                                                                                  \
				const A a_;                                                                           \
				const T b_;                                                                           \
			};                                                                                        \
			return op_type( a_, b);                                                                   \
		}
#define UNARY_OP( op )                                                                                \
	inline auto operator op () const {                                                                \
		struct op_type : public operator_base<value_type,op_type> {                                   \
			op_type( const A& a ) :                                                                   \
				a_(a.make_copy()), operator_base<value_type,op_type>(*this) {                         \
			}                                                                                         \
			inline auto operator[](int i) const {                                                     \
				return op a_[i];                                                                      \
			}                                                                                         \
			inline op_type make_copy() const {                                                        \
				return op_type(a_);                                                                   \
			}                                                                                         \
		private:                                                                                      \
			const A a_;                                                                               \
		};                                                                                            \
		return op_type( a_);                                                                          \
	}

template<class T, class A>
struct operator_base {
	using value_type = T;
	operator_base(const A& a) :
			a_(a) {
	}

	BINARY_OP(+);
	BINARY_OP(-);
	BINARY_OP(*);
	BINARY_OP(/);
	BINARY_OP(%);
	BINARY_OP(|);
	BINARY_OP(^);
	BINARY_OP(&);
	BINARY_OP(||);
	BINARY_OP(&&);
	BINARY_OP(>>);
	BINARY_OP(<<);
	BINARY_OP(>=);
	BINARY_OP(<=);
	BINARY_OP(>);
	BINARY_OP(<);
	BINARY_OP(==);
	BINARY_OP(!=);
	UNARY_OP(+);
	UNARY_OP(-);
	UNARY_OP(~);
	UNARY_OP(!);

private:
	const A& a_;

};

template<class T, int Size>
struct math_array: public operator_base<T,math_array<T, Size>> {
	using value_type = T;

	inline math_array() :
			a_(), operator_base<T,math_array<T, Size>>(*this) {
	}

	template<class B>
	inline math_array& operator=(const B& b) {
		for (int i = 0; i < Size; i++) {
			a_[i] = b[i];
		}
		return *this;
	}

	template<class B>
	inline math_array(const B& b) :
			a_(), operator_base<T,math_array>(*this) {
		*this = b;
	}

private:
	static constexpr bool is_dynamic = Size > 256;
	using A = array_container<T, Size, is_dynamic>;
	A a_;

	inline const auto& operator[](int i) const {
		return a_[i];
	}

	inline auto& operator[](int i) {
		return a_[i];
	}

	inline math_array(const A& a) :
			a_(a), operator_base<T,math_array>(*this) {
	}

	inline math_array make_copy() const {
		return math_array(a_.make_copy());
	}

	template<class, class>
	friend class operator_base;
};

}
#endif /* OCTOPH_MATH_CONTAINERS_HPP_ */
