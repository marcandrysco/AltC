#ifndef MATH_H
#define MATH_H

/**
 * Random storage structure.
 *   @x, y, z, w: Storage data.
 */

struct m_rand_t {
	uint32_t x, y, z, w;
};


/*
 * random function declarations
 */

struct m_rand_t m_rand_init(uint32_t seed);
uint32_t m_rand_next(struct m_rand_t *rand);
double m_rand_nextd(struct m_rand_t *rand);
char m_rand_alnum(struct m_rand_t *rand);
char *m_rand_alnum_str(struct m_rand_t *rand, unsigned int len);

unsigned int m_gcd(unsigned int a, unsigned int b);
unsigned int m_lcm(unsigned int a, unsigned int b);


/**
 * Square a value.
 *   @val: The value.
 *   &returns: The value.
 */

static inline double m_sqr_double(double val)
{
	return val * val;
}

/**
 * Cube a value.
 *   @val: The value.
 *   &returns: The value.
 */

static inline double m_cube_double(double val)
{
	return val * val * val;
}


/**
 * Calculate minimum of two integers.
 *   @left: The left number.
 *   @right: The right number.
 *   &returns: The minimum.
 */

static inline int m_min_int(int left, int right)
{
	return (left < right) ? left : right;
}

/**
 * Calculate minimum of two unsigned integers.
 *   @left: The left number.
 *   @right: The right number.
 *   &returns: The minimum.
 */

static inline unsigned int m_min_uint(unsigned int left, unsigned int right)
{
	return (left < right) ? left : right;
}

/**
 * Calculate minimum of two size integers.
 *   @left: The left number.
 *   @right: The right number.
 *   &returns: The minimum.
 */

static inline size_t m_min_size(size_t left, size_t right)
{
	return (left < right) ? left : right;
}

/**
 * Calculate minimum of two doubles.
 *   @left: The left number.
 *   @right: The right number.
 *   &returns: The minimum.
 */

static inline double m_min_double(double left, double right)
{
	return (left < right) ? left : right;
}


/**
 * Calculate maximum of two integers.
 *   @left: The left number.
 *   @right: The right number.
 *   &returns: The maximum.
 */

static inline int m_max_int(int left, int right)
{
	return (left > right) ? left : right;
}

/**
 * Calculate maximum of two unsigned integers.
 *   @left: The left number.
 *   @right: The right number.
 *   &returns: The maximum.
 */

static inline unsigned int m_max_uint(unsigned int left, unsigned int right)
{
	return (left > right) ? left : right;
}

/**
 * Calculate maximum of two unsigned 16-bit integers.
 *   @left: The left number.
 *   @right: The right number.
 *   &returns: The maximum.
 */

static inline uint16_t m_max_uint16(uint16_t left, uint16_t right)
{
	return (left > right) ? left : right;
}

/**
 * Calculate maximum of two size integers.
 *   @left: The left number.
 *   @right: The right number.
 *   &returns: The maximum.
 */

static inline size_t m_max_size(size_t left, size_t right)
{
	return (left > right) ? left : right;
}

/**
 * Calculate maximum of two doubles.
 *   @left: The left number.
 *   @right: The right number.
 *   &returns: The maximum.
 */

static inline double m_max_double(double left, double right)
{
	return (left > right) ? left : right;
}


/**
 * Calculate a limited double value.
 *   @val: The value.
 *   @low: The low value.
 *   @high: The high value.
 *   &returns: The limited value.
 */

static inline double m_limit_double(double val, double low, double high)
{
	return (val < low) ? low : ((val > high) ? high : val);
}


/**
 * Unsinged integer count the number of leading zeros.
 *   @n: The number.
 *   &returns: The number of leading zeros
 */

static inline int8_t m_uintclz(unsigned int n)
{
	return __builtin_clz(n);
}

/**
 * Unsinged integer count the number of leading zeros.
 *   @n: The number.
 *   &returns: The number of leading zeros
 */

static inline int8_t m_uintctz(unsigned int n)
{
	return __builtin_ctz(n);
}

#endif
