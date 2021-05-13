typedef int int32_t;
typedef unsigned int u_int32_t;

typedef union
{
  float value;
  u_int32_t word;
} ieee_float_shape_type;

/* Get a 32 bit int from a float.  */

#define GET_FLOAT_WORD(i,d)					\
do {								\
  ieee_float_shape_type gf_u;					\
  gf_u.value = (d);						\
  (i) = gf_u.word;						\
} while (0)

/* Set a float from a 32 bit int.  */

#define SET_FLOAT_WORD(d,i)					\
do {								\
  ieee_float_shape_type sf_u;					\
  sf_u.word = (i);						\
  (d) = sf_u.value;						\
} while (0)


int32_t	__ieee754_rem_pio2f(float x, float *y);
int	__kernel_rem_pio2f(float *x, float *y, int e0, int nx, int prec, const int32_t *ipio2);
float	__kernel_cosf(float x, float y);
float	__kernel_sinf(float x, float y, int iy);
float	__kernel_tanf(float x, float y, int iy);
float	scalbnf(float x, int n);
float	copysignf(float x, float y);
