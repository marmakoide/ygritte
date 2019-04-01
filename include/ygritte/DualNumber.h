#ifndef YGRITTE_DUAL_NUMBER_H
#define YGRITTE_DUAL_NUMBER_H

#include <cmath>



namespace ygritte {
	template <class T>
	class dual_number {
	public:
		typedef T value_type;
		typedef T scalar_type;



		inline dual_number() :
			m_a(0),
			m_b(0) { }

		inline dual_number(value_type a) :
			m_a(a),
			m_b(0) { }

		inline dual_number(value_type a, value_type b) :
			m_a(a),
			m_b(b) { }

		inline dual_number(const dual_number& other) :
			m_a(other.m_a), 
			m_b(other.m_b) { }

		

		inline value_type real() const {
			return m_a;
		}

		inline value_type dual() const {
			return m_b;
		}



		inline bool
		operator == (const value_type& other) const {
			return (m_a == other) and (m_b == 0);
		}

		inline bool
		operator != (const value_type& other) const {
			return (m_a != other) or (m_b != 0);
		}

		inline bool
		operator == (const dual_number& other) const {
			return (m_a == other.m_a) and (m_b == other.m_b);
		}

		inline bool
		operator != (const dual_number& other) const {
			return (m_a != other.m_a) or (m_b != other.m_b);
		}



		inline dual_number
		operator - () const {
			return dual_number(-m_a, -m_b);
		}

		template <class U>
		inline dual_number&
		operator = (const U& other) {
			m_a = other;
			m_b = 0;
			return *this;
		}

		inline dual_number&
		operator = (const dual_number& other) {
			m_a = other.m_a;
			m_b = other.m_b;
			return *this;
		}

		template <class U>
		inline dual_number&
		operator += (const U& other) {
			m_a += other;
			return *this;
		}

		inline dual_number&
		operator += (const dual_number& other) {
			m_a += other.m_a;
			m_b += other.m_b;
			return *this;
		}

		template <class U>
		inline dual_number&
		operator -= (const U& other) {
			m_a -= other;
			return *this;
		}

		inline dual_number&
		operator -= (const dual_number& other) {
			m_a -= other.m_a;
			m_b -= other.m_b;
			return *this;
		}

		template <class U>
		inline dual_number&
		operator *= (const U& other) {
			m_a *= other;
			m_b *= other;
			return *this;
		}

		inline dual_number&
		operator *= (const dual_number& other) {
			value_type ap = m_a * other.m_a;
			value_type bp = m_a * other.m_b + m_b * other.m_a;
			
			m_a = ap;
			m_b = bp;
			return *this;
		}

		template <class U>
		inline dual_number&
		operator /= (const U& other) {
			m_a /= other;
			m_b /= other;
			return *this;
		}

		inline dual_number&
		operator /= (const dual_number& other) {
			value_type ap = m_a / other.m_a;
			value_type bp = (m_b * other.m_a - m_a * other.m_b) / (other.m_a * other.m_a);
			
			m_a = ap;
			m_b = bp;
			return *this;
		}

	private:
		value_type m_a, m_b;
	}; // class dual_number
} // namespace ygritte



template <class T>
ygritte::dual_number<T>
operator + (const ygritte::dual_number<T>& x,
	          const ygritte::dual_number<T>& y) {
	ygritte::dual_number<T> ret(x);
	ret += y;
	return ret;
}

template <class left_T, class right_T>
ygritte::dual_number<left_T>
operator + (const ygritte::dual_number<left_T>& x,
	          const right_T& y) {
	ygritte::dual_number<left_T> ret(x);
	ret += y;
	return ret;
}

template <class left_T, class right_T>
ygritte::dual_number<right_T>
operator + (const left_T& x,
	          const ygritte::dual_number<right_T>& y) {
	ygritte::dual_number<right_T> ret(y);
	ret += x;
	return ret;
}



template <class T>
ygritte::dual_number<T>
operator - (const ygritte::dual_number<T>& x,
	          const ygritte::dual_number<T>& y) {
	ygritte::dual_number<T> ret(x);
	ret -= y;
	return ret;
}

template <class T>
ygritte::dual_number<T>
operator - (const ygritte::dual_number<T>& x,
	          const T& y) {
	ygritte::dual_number<T> ret(x);
	ret -= y;
	return ret;
}

template <class T>
ygritte::dual_number<T>
operator - (const T& x,
	          const ygritte::dual_number<T>& y) {
	ygritte::dual_number<T> ret = -y;
	ret += x;
	return ret;
}



template <class T>
ygritte::dual_number<T>
operator * (const ygritte::dual_number<T>& x,
	          const ygritte::dual_number<T>& y) {
	ygritte::dual_number<T> ret(x);
	ret *= y;
	return ret;
}

template <class left_T, class right_T>
ygritte::dual_number<left_T>
operator * (const ygritte::dual_number<left_T>& x,
	          const right_T& y) {
	ygritte::dual_number<left_T> ret(x);
	ret *= y;
	return ret;
}

template <class left_T, class right_T>
ygritte::dual_number<right_T>
operator * (const left_T& x,
	          const ygritte::dual_number<right_T>& y) {
	ygritte::dual_number<right_T> ret(y);
	ret *= x;
	return ret;
}



template <class T>
ygritte::dual_number<T>
operator / (const ygritte::dual_number<T>& x,
	          const ygritte::dual_number<T>& y) {
	ygritte::dual_number<T> ret(x);
	ret /= y;
	return ret;
}

template <class T>
ygritte::dual_number<T>
operator / (const ygritte::dual_number<T>& x,
	          const T& y) {
	ygritte::dual_number<T> ret(x);
	ret /= y;
	return ret;
}

template <class T>
ygritte::dual_number<T>
operator / (const T& x,
	          const ygritte::dual_number<T>& y) {
	ygritte::dual_number<T> ret(T(1));
	ret /= y;
	ret *= x;
	return ret;
}



namespace std {
	template <class T>
	ygritte::dual_number<T>
	max(const ygritte::dual_number<T>& x,
	    const ygritte::dual_number<T>& y) {
		return
			x.real() >= y.real() ? x : y;
	}

	template <class T>
	ygritte::dual_number<T>
	max(const ygritte::dual_number<T>& x,
	    const T& y) {
		return
			x.real() >= y ? x : ygritte::dual_number<T>(y);
	}

	template <class T>
	ygritte::dual_number<T>
	max(const T& x,
	    const ygritte::dual_number<T>& y) {
		return
			x >= y.real() ? ygritte::dual_number<T>(x) : y;
	}



	template <class T>
	ygritte::dual_number<T>
	min(const ygritte::dual_number<T>& x,
	    const ygritte::dual_number<T>& y) {
		return
			x.real() <= y.real() ? x : y;
	}

	template <class T>
	ygritte::dual_number<T>
	min(const ygritte::dual_number<T>& x,
	    const T& y) {
		return
			x.real() <= y ? x : ygritte::dual_number<T>(y);
	}

	template <class T>
	ygritte::dual_number<T>
	min(const T& x,
	    const ygritte::dual_number<T>& y) {
		return
			x <= y.real() ? ygritte::dual_number<T>(x) : y;
	}



	template <class T, class U>
	ygritte::dual_number<T>
	pow(const ygritte::dual_number<T>& x,
	    const U& y) {
		return
			ygritte::dual_number<T>(std::pow(x.real(), y),
			                        y * x.dual() * std::pow(x.real(), y - 1));
	}
	


	template <class T>
	ygritte::dual_number<T>
	fabs(const ygritte::dual_number<T>& x) {
		return
			std::fabs(x.real()) == x.real() ? x : -x;
	}



	template <class T>
	ygritte::dual_number<T>
	sqrt(const ygritte::dual_number<T>& x) {
		T u = std::sqrt(x.real());
		return
			ygritte::dual_number<T>(u,
			                        x.dual() * (1 / (2 * u)));
	}



	template <class T>
	ygritte::dual_number<T>
	exp(const ygritte::dual_number<T>& x) {
		T u = std::exp(x.real());
		return
			ygritte::dual_number<T>(u,
			                        x.dual() * u);
	}

	template <class T>
	ygritte::dual_number<T>
	log(const ygritte::dual_number<T>& x) {
		return
			ygritte::dual_number<T>(std::log(x.real()),
			                        x.dual() / x.real());
	}



	template <class T>
	ygritte::dual_number<T>
	sin(const ygritte::dual_number<T>& x) {
		return
			ygritte::dual_number<T>(std::sin(x.real()),
			                        x.dual() * std::cos(x.real()));
	}

	template <class T>
	ygritte::dual_number<T>
	cos(const ygritte::dual_number<T>& x) {
		return
			ygritte::dual_number<T>(std::cos(x.real()),
			                        x.dual() * -std::sin(x.real()));
	}

	template <class T>
	ygritte::dual_number<T>
	tan(const ygritte::dual_number<T>& x) {
		T u = std::tan(x.real());
		return
			ygritte::dual_number<T>(u,
			                        x.dual() * (u * u + 1));
	}



	template <class T>
	ygritte::dual_number<T>
	acos(const ygritte::dual_number<T>& x) {
		return
			ygritte::dual_number<T>(std::acos(x.real()),
			                        -x.dual() / std::sqrt(1 - x.real() * x.real()));
	}

	template <class T>
	ygritte::dual_number<T>
	asin(const ygritte::dual_number<T>& x) {
		return
			ygritte::dual_number<T>(std::asin(x.real()),
			                        x.dual() / std::sqrt(1 - x.real() * x.real()));
	}

	template <class T>
	ygritte::dual_number<T>
	atan(const ygritte::dual_number<T>& x) {
		return
			ygritte::dual_number<T>(std::atan(x.real()),
			                        x.dual() / (1 + x.real() * x.real()));
	}
} // namespace std




#endif // YGRITTE_DUAL_NUMBER_H

