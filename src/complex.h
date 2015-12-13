#ifndef COMPLEX_H
#define COMPLEX_H

/**
 * Double complex structure.
 *   @re, im: The re and imaginary components.
 */

struct z_double_t {
	double re, im;
};


/*
 * complex variables
 */

extern struct z_double_t z_double_zero;
extern struct z_double_t z_double_one;


/**
 * Complex double contructor.
 *   @re: The real component.
 *   @im: The imaginary component.
 */

static inline struct z_double_t z_double(double re, double im)
{
	return (struct z_double_t){ re, im };
}

/**
 * Complex double contructor from a real.
 *   @re: The real component.
 */

static inline struct z_double_t z_double_re(double re)
{
	return z_double(re, 0.0);
}

/**
 * Complex double contructor from an imaginary.
 *   @im: The imaginary component.
 */

static inline struct z_double_t z_double_im(double im)
{
	return z_double(0.0, im);
}

/**
 * Check if a complex double is zero.
 *   @flt: The complex double.
 *   &returns: True if zero, false otherwise.
 */

static inline bool z_double_iszero(struct z_double_t flt)
{
	return ((flt.re == 0.0) || (flt.re == -0.0)) && ((flt.im == 0.0) || (flt.im == -0.0));
}

/**
 * Check if a complex double is one.
 *   @flt: The complex double.
 *   &returns: True if one, false otherwise.
 */

static inline bool z_double_isone(struct z_double_t flt)
{
	return (flt.re == 1.0) && ((flt.im == 0.0) || (flt.im == -0.0));
}

/**
 * Check if two complex doubles are equal.
 *   @left: The left value.
 *   @right: The right value.
 *   &returns: True if equal, false otherwise.
 */

static inline bool z_double_isequal(struct z_double_t left, struct z_double_t right)
{
	return (left.re == right.re) && (left.im == right.im);
}


/**
 * Add two complex doubles together.
 *   @a: The first value.
 *   @b: The second value.
 *   &returns: The sum.
 */

static inline struct z_double_t z_double_add(struct z_double_t a, struct z_double_t b)
{
	return z_double(a.re + b.re, a.im + b.im);
}

/**
 * Subtract two complex doubles together.
 *   @a: The first value.
 *   @b: The second value.
 *   &returns: The sum.
 */

static inline struct z_double_t z_double_sub(struct z_double_t a, struct z_double_t b)
{
	return z_double(a.re - b.re, a.im - b.im);
}

/**
 * Multiply two complex doubles together.
 *   @a: The first value.
 *   @b: The second value.
 *   &returns: The product.
 */

static inline struct z_double_t z_double_mul(struct z_double_t a, struct z_double_t b)
{
	return z_double(a.re * b.re - a.im * b.im, a.im * b.re + a.re * b.im);
}

/**
 * Divide two complex doubles together.
 *   @a: The first value.
 *   @b: The second value.
 *   &returns: The quotient.
 */

static inline struct z_double_t z_double_div(struct z_double_t a, struct z_double_t b)
{
	double t = b.re * b.re + b.im * b.im;

	return z_double((a.re * b.re + a.im * b.im) / t, (a.im * b.re - a.re * b.im) / t);
}

static inline struct z_double_t z_double_exp(struct z_double_t a)
{
	double c = exp(a.re);

	return z_double(c * cos(a.im), c * sin(a.im));
}

static inline struct z_double_t z_double_expi(double im)
{
	return z_double(cos(im), sin(im));
}

/**
 * Retriev the magnitude of the complex double.
 *   @a: The complex double.
 *   &returns: The magnitude.
 */

static inline double z_double_mag(struct z_double_t a)
{
	return sqrt((a.re * a.re) + (a.im * a.im));
}

/**
 * Retriev the magnitude of the complex double.
 *   @a: The complex double.
 *   &returns: The argument.
 */

static inline double z_double_arg(struct z_double_t a)
{
	return atan2(a.im, a.re);
}

#endif
