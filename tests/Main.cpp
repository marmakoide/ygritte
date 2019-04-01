#include <minunit.h>

#include <ygritte/Macros>
#include <ygritte/DualNumber.h>

#include <functional>
#include <iostream>

using namespace std;
using namespace ygritte;



template <class T>
class LinearRange {
public:
	class LinearRangeIterator {
	public:
		LinearRangeIterator(const LinearRange& range, 
		                    std::size_t i) :
			m_range(range),
			m_i(i) { }

		inline bool operator == (const LinearRangeIterator& other) const {
			return m_i == other.m_i;
		}

		inline bool operator != (const LinearRangeIterator& other) const {
			return m_i != other.m_i;
		}

		inline LinearRangeIterator& operator ++ () {
			m_i += 1;
			return *this;
		}

		inline T operator * () const {
			return m_range[m_i];
		}

	private:
		const LinearRange& m_range;
		std::size_t m_i;
	}; // class LinearRangeIterator



	LinearRange(T a, T b, std::size_t n) :
		m_a(a),
		m_delta((b - a) / n),
		m_n(n) { }

	inline LinearRangeIterator
	begin() const {
		return LinearRangeIterator(*this, 0);
	}	

	inline LinearRangeIterator
	end() const {
		return LinearRangeIterator(*this, m_n);
	}	

	inline std::size_t size() const {
		return m_n;
	}

	inline T operator () (std::size_t i ) const {
		return m_delta * i + m_a;
	}

	inline T operator [] (std::size_t i ) const {
		return m_delta * i + m_a;
	}

private:
	T m_a;
	T m_delta;
	std::size_t m_n;
}; // class LinearRange



// --- Auto differenciation test ----------------------------------------------

/*
 * Central difference functor
 */

template <class T>
class CentralDifferenceFunctor {
public:
	typedef std::function<T (T)> func_type;



	CentralDifferenceFunctor(const func_type& func,
	                         const T& epsilon) :
		m_func(func),
		m_epsilon(epsilon) { }

	T
	operator () (const T& x) const {
		return (m_func(x + m_epsilon) - m_func(x - m_epsilon)) / (2 * m_epsilon);
	}

private:
	const func_type& m_func;
	T m_epsilon;
}; // class CentralDifferenceFunctor



template <class T>
CentralDifferenceFunctor<T>
central_difference(const std::function<T (T)>& func,
                   const T& epsilon) {
	return CentralDifferenceFunctor<T>(func, epsilon);
}


/*
 * Automatic differenciation functor
 */

template <class T>
class AutoDifferenceFunctor {
public:
	typedef std::function<dual_number<T> (dual_number<T>)> func_type;

	AutoDifferenceFunctor(const func_type& func) :
		m_func(func) { }

	T
	operator () (const T& x) const {
		dual_number<T> u(x, 1);
		return m_func(u).dual();
	}

private:
	const func_type& m_func;
}; // class AutoDifferenceFunctor



template <class T>
AutoDifferenceFunctor<T>
auto_difference(const std::function<dual_number<T> (dual_number<T>)> & func) {
	return AutoDifferenceFunctor<T>(func);
}



/*
 * List of elementary function test
 */

template <class T>
int
dual_number_sin_derivative_test() {
	typedef dual_number<T> dual_T;

	std::function<dual_T(dual_T)> fd_sin = (dual_T(*)(dual_T))&std::sin<T>;
	auto exact_func = auto_difference<T>(fd_sin);

	std::function<T(T)> f_sin = (T(*)(T))&std::sin;
	auto estimate_func = central_difference<T>(f_sin, 1e-6);
	
	for(T theta : LinearRange<T>(0, 2 * M_PI, 16))
		mu_assert(std::fabs(exact_func(theta) - estimate_func(theta)) < 1e-6, "wrong derivative");

	// Job done
	return 0;
}



template <class T>
int
dual_number_cos_derivative_test() {
	typedef dual_number<T> dual_T;

	std::function<dual_T(dual_T)> fd_cos = (dual_T(*)(dual_T))&std::cos<T>;
	auto exact_func = auto_difference<T>(fd_cos);

	std::function<T(T)> f_cos = (T(*)(T))&std::cos;
	auto estimate_func = central_difference<T>(f_cos, 1e-6);
	
	for(T theta : LinearRange<T>(0, 2 * M_PI, 16))
		mu_assert(std::fabs(exact_func(theta) - estimate_func(theta)) < 1e-6, "wrong derivative");

	// Job done
	return 0;
}



template <class T>
int
dual_number_tan_derivative_test() {
	typedef dual_number<T> dual_T;

	std::function<dual_T(dual_T)> fd_tan = (dual_T(*)(dual_T))&std::tan<T>;
	auto exact_func = auto_difference<T>(fd_tan);

	std::function<T(T)> f_tan = (T(*)(T))&std::tan;
	auto estimate_func = central_difference<T>(f_tan, 1e-6);
	
	for(T theta : LinearRange<T>(-M_PI / 2 + 1e-1, M_PI / 2 - 1e-1, 16))
		mu_assert(std::fabs(exact_func(theta) - estimate_func(theta)) < 1e-6, "wrong derivative");

	// Job done
	return 0;
}



template <class T>
int
dual_number_atan_derivative_test() {
	typedef dual_number<T> dual_T;

	std::function<dual_T(dual_T)> fd_atan = (dual_T(*)(dual_T))&std::atan<T>;
	auto exact_func = auto_difference<T>(fd_atan);

	std::function<T(T)> f_atan = (T(*)(T))&std::atan;
	auto estimate_func = central_difference<T>(f_atan, 1e-6);
	
	for(T x : LinearRange<T>(-8, 8, 64))
		mu_assert(std::fabs(exact_func(x) - estimate_func(x)) < 1e-6, "wrong derivative");

	// Job done
	return 0;
}



template <class T>
int
dual_number_fabs_derivative_test() {
	typedef dual_number<T> dual_T;

	std::function<dual_T(dual_T)> fd_fabs = (dual_T(*)(dual_T))&std::fabs<T>;
	auto exact_func = auto_difference<T>(fd_fabs);

	std::function<T(T)> f_fabs = (T(*)(T))&std::fabs;
	auto estimate_func = central_difference<T>(f_fabs, 1e-6);
	
	for(T x : LinearRange<T>(-8, 8, 16)) {
		if (x != 0)
			mu_assert(std::fabs(exact_func(x) - estimate_func(x)) < 1e-6, "wrong derivative");
	}

	// Job done
	return 0;
}



template <class T>
int
dual_number_exp_derivative_test() {
	typedef dual_number<T> dual_T;

	std::function<dual_T(dual_T)> fd_exp = (dual_T(*)(dual_T))&std::exp<T>;
	auto exact_func = auto_difference<T>(fd_exp);

	std::function<T(T)> f_exp = (T(*)(T))&std::exp;
	auto estimate_func = central_difference<T>(f_exp, 1e-6);
	
	for(T x : LinearRange<T>(-4, 4, 16))
		mu_assert(std::fabs(exact_func(x) - estimate_func(x)) < 1e-6, "wrong derivative");

	// Job done
	return 0;
}



template <class T>
int
dual_number_log_derivative_test() {
	typedef dual_number<T> dual_T;

	std::function<dual_T(dual_T)> fd_log = (dual_T(*)(dual_T))&std::log<T>;
	auto exact_func = auto_difference<T>(fd_log);

	std::function<T(T)> f_log = (T(*)(T))&std::log;
	auto estimate_func = central_difference<T>(f_log, 1e-6);
	
	for(T x : LinearRange<T>(.1, 8, 64))
		mu_assert(std::fabs(exact_func(x) - estimate_func(x)) < 1e-6, "wrong derivative");

	// Job done
	return 0;
}



template <class T>
int
dual_number_sqrt_derivative_test() {
	typedef dual_number<T> dual_T;

	std::function<dual_T(dual_T)> fd_sqrt = (dual_T(*)(dual_T))&std::sqrt<T>;
	auto exact_func = auto_difference<T>(fd_sqrt);

	std::function<T(T)> f_sqrt = (T(*)(T))&std::sqrt;
	auto estimate_func = central_difference<T>(f_sqrt, 1e-6);
	
	for(T x : LinearRange<T>(.1, 4, 64))
		mu_assert(std::fabs(exact_func(x) - estimate_func(x)) < 1e-6, "wrong derivative");

	// Job done
	return 0;
}



template <class T>
T
test_func_a(T x) {
	return std::pow(std::sin(2 * x * x), 3);
}



template <class T>
int
dual_number_test_func_a_derivative_test() {
	typedef dual_number<T> dual_T;

	std::function<dual_T(dual_T)> fd = (dual_T(*)(dual_T))&test_func_a<dual_T>;
	auto exact_func = auto_difference<T>(fd);

	std::function<T(T)> f = (T(*)(T))&test_func_a<T>;
	auto estimate_func = central_difference<T>(f, 1e-6);

	for(T theta : LinearRange<T>(0, std::sqrt(M_PI), 16))
		mu_assert(std::fabs(exact_func(theta) - estimate_func(theta)) < 1e-6, "wrong derivative");

	// Job done
	return 0;
}


template <class T>
T
test_polynomial(T x) {
	T x2 = x * x;
	return 1 + x2 * (-8 + x2 * (3 + x2 * 5));
}


template <class T>
T
test_polynomial_derivative(T x) {
	T x2 = x * x;
	return x * (-16 + x2 * (12 + x2 * 30));
}



template <class T>
int
dual_number_test_polynomial_derivative_test() {
	typedef dual_number<T> dual_T;

	std::function<dual_T(dual_T)> fd = (dual_T(*)(dual_T))&test_polynomial<dual_T>;
	auto exact_func = auto_difference<T>(fd);

	for(T theta : LinearRange<T>(-2, 2, 16))
		mu_assert(std::fabs(exact_func(theta) - test_polynomial_derivative(theta)) < 1e-6, "wrong derivative");

	// Job done
	return 0;
}



// --- Entry point ------------------------------------------------------------

int
all_tests() {
	// Automatic differenciation tests
	mu_run_test(dual_number_sin_derivative_test<double>);
	mu_run_test(dual_number_cos_derivative_test<double>);
	mu_run_test(dual_number_tan_derivative_test<double>);
	mu_run_test(dual_number_atan_derivative_test<double>);
	mu_run_test(dual_number_fabs_derivative_test<double>);
	mu_run_test(dual_number_exp_derivative_test<double>);
	mu_run_test(dual_number_log_derivative_test<double>);
	mu_run_test(dual_number_sqrt_derivative_test<double>);
	mu_run_test(dual_number_test_func_a_derivative_test<double>);
	mu_run_test(dual_number_test_polynomial_derivative_test<double>);

	// Job done
	return 0;
}



int
main(int UNUSED_PARAM(argc), char** UNUSED_PARAM(argv)) {
	// Run the tests
	int failed = all_tests();

	// Print diagnostic
	if (!failed)
		printf("ALL TESTS PASSED\n");

	printf("Tests run: %u\n", mu_tests_run);

	// Job done
	return EXIT_SUCCESS;
}


