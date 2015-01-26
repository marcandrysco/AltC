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

struct m_rand_t m_rand_new(uint32_t seed);
uint32_t m_rand_next(struct m_rand_t *rand);
double m_rand_nextd(struct m_rand_t *rand);


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

#endif
