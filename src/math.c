#include "common.h"
#include "math.h"
#include "posix/thread.h"
#include "posix/time.h"
#include "mem.h"


/*
 * local function declarations
 */

static void def_init(void);

/*
 * loal variables
 */

static struct m_rand_t def_rand;


/**
 * Retrieve the default random number generator.
 *   &return: The random number.
 */

_export
struct m_rand_t *m_rand_def(void)
{
	static _once_t once = _ONCE_INIT;

	_thread_once(&once, def_init);

	return &def_rand;
}

/**
 * Initialize a random number generator.
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

	  if(rand == NULL)
		  rand = m_rand_def();

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

/**
 * Retrieve a random alphanumerical value.
 *   @rand: The number generator.
 *   &returns: The next character.
 */

_export
char m_rand_alnum(struct m_rand_t *rand)
{
	uint32_t val;

	val = m_rand_next(rand) % 62;
	if(val < 26)
		return val + 'a';
	else if(val < 52)
		return val + 'A' - 26;
	else
		return val + '0' - 52;
}

/**
 * Create a random alpha-numeric string.
 *   @rand: The number generator.
 *   @len: The length.
 *   &returns: The allocated string.
 */

_export
char *m_rand_alnum_str(struct m_rand_t *rand, unsigned int len)
{
	char *str;

	str = mem_alloc(len + 1);
	str[len] = '\0';

	for(len--; len != UINT_MAX; len--)
		str[len] = m_rand_alnum(rand);

	return str;
}


/**
 * Initialize the default random number generator.
 */

static void def_init(void)
{
	def_rand = m_rand_init(_utime());
}


/**
 * Greatest common divisor computation.
 *   @a: The first number.
 *   @b: The second number.
 *   &return: The greatest common divisor.
 */

_export
unsigned int m_gcd(unsigned int a, unsigned int b)
{
	unsigned int t;

	while(a != 0) {
		t = a;
		a = b % a;
		b = t;
	}

	return b;
}

/**
 * Least common multiple computation.
 *   @a: The first number.
 *   @b: The second number.
 *   &return: The least common multiple.
 */

_export
unsigned int m_lcm(unsigned int a, unsigned int b)
{
	return (a * b) / m_gcd(a, b);
}
