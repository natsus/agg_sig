#undef NDEBUG
#include <iostream>
using namespace std;
#include <assert.h>

#include <gmp.h>
#ifdef __cplusplus
  extern "C" {
    #include <tepla/ec.h>
  }
#endif

void element_print( const string , const Element );
void point_print( const string , const EC_POINT );

int main()
{
  EC_PAIRING prg;
  EC_POINT P, Q, R;
  Field f;
  Element a, b, c;

  // init 
  pairing_init(prg, "ECBN254");
  point_init(P, prg->g1);
  point_init(Q, prg->g1);
  point_init(R, prg->g1);
  field_init(f, "bn254_fp");
  element_init(a, f);
  element_init(b, f);
  element_init(c, f);

  // gen
  element_random(a);
  element_random(b);
  point_random(P);
  point_random(Q);

  // test

  // element test
  element_print("a: ", a);
  element_print("b: ", b);
  element_mul(c, a, b);
  element_print("a*b: ", c);
  element_mul(c, b, a);
  element_print("b*a: ", c);

  // point test
  point_print("P: ", P);
  point_print("Q: ", Q);
  point_add(R, P, Q);
  point_print("P+Q: ", R);
  point_add(R, Q, P);
  point_print("Q+P: ", R);

  // fin
  element_clear(a);
  element_clear(b);
  element_clear(c);
  field_clear(f);
  point_clear(P);
  point_clear(Q);
  point_clear(R);
  pairing_clear(prg);

  return 0;
}

void point_print(const string s, const EC_POINT x)
{
  int size = point_get_str_length(x);
  char *str = new char[size];

  point_get_str(str, x);
  cout << s << str << endl;

  delete [] str;
}

void element_print(const string s, const Element x)
{
  int size = element_get_str_length(x);
  char *str = new char[size];

  element_get_str(str, x);
  cout << s << str << endl;

  delete [] str;
}
