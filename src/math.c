#include "common.h"
#include "math.h"


/**
 * Initialize a random number gnereator.
 *   @seed: The seed.
 *   &returns; The random generator.
 */

_export
struct m_rand_t m_rand_init(uint32_t seed)
{
	return (struct m_rand_t){ 123456789, 362436069, 521288629, seed };
}

/**
 * Retrieve the next number from the generator.
 *   @rand: The number generator.
 *   &returns: The next number.
 */

_export
uint32_t m_rand_next(struct m_rand_t *rand)
{
	  uint32_t t;

	  t = rand->x ^ (rand->x << 11);
	  rand->x = rand->y;
	  rand->y = rand->z;
	  rand->z = rand->w;
	  rand->w = rand->w ^ (rand->w >> 19) ^ (t ^ (t >> 8));

	  return rand->w;
}

/**
 * Retrieve a random number between the values zero and one.
 *   @rand: The number generator.
 *   &returns: The next number.
 */

_export
double m_rand_nextd(struct m_rand_t *rand)
{
	return m_rand_next(rand) / (double)UINT32_MAX;
}
