#include <iostream>
using namespace std;

#include <gmp.h>
// #include <gmpxx.h>

#ifdef __cplusplus
  extern "C" {
    #include <tepla/ec.h>
  }
#endif


class AggSig {
  public:
    AggSig();
    ~AggSig();
    void key_gen();
    void sign( const char* , size_t );
    bool vrfy(const char* , size_t );

  private:
    EC_PAIRING prg;
    EC_POINT G1, G2, g2, v, h, s;
    Field f ;
    Element x;
    mpz_t tmp;

    void get_g2();
    void point_print( const char* , const EC_POINT );
    void element_print( const char* , const Element );
    void set_mpz_from_element( mpz_t , Element );

};

AggSig::AggSig()
{
  mpz_init(tmp);

  pairing_init(prg, "ECBN254");
  point_init(G1, prg->g1);
  point_init(G2, prg->g2);
  point_init(g2, prg->g2);
  point_init(v , prg->g2);
  point_init(h , prg->g1);
  point_init(s , prg->g1);

  get_g2();

  field_init(f, "bn254_fp");
  // gmp_printf ("%s is an mpz %Zd\n", "order of f", f->order);
  element_init(x, f);

  key_gen();

}

AggSig::~AggSig()
{
  element_clear(x);

  field_clear(f);

  point_clear(G1);
  point_clear(G2);
  point_clear(g2);
  point_clear(v);
  point_clear(h);
  point_clear(s);

  pairing_clear(prg);

  mpz_clear(tmp);
}

// Key Generation
void AggSig::key_gen()
{
  // *** generation of secret key 'x' (random value) ***
  element_random(x);
  // gmp_printf ("%s is an mpz %Zd\n", "order of x", x->field->order);
  // element_print("value of x: ", x);

  // *** generation of public key 'v' ***
  // ?? v = g_2^x ⇔ v = x g_2
  set_mpz_from_element(tmp, x); // tmp(mpz_t) ← x(Element)
  point_mul(v, tmp, g2);  // v ← g_2^x
}

// Signing
void AggSig::sign( const char* m, size_t slen)
{
  // *** h ← H(M) ***
  point_map_to_point(h, m, slen, 128);

  // *** generation of signature 's' ***
  set_mpz_from_element(tmp, x); // tmp(mpz_t) ← x(Element)
  point_mul(s, tmp, h); // s ← h^x
}

// Verification
bool AggSig::vrfy(const char* m, size_t slen)
{
  // *** initialization ***
  Element t1, t2;
  element_init(t1, prg->g3);
  element_init(t2, prg->g3);
  bool rslt = false;


  // *** h ← H(M) ***
  point_map_to_point(h, m, slen, 128);


  // *** pairing computation ***
  // *** e(s,g2), e(h,v)     ***
  pairing_map(t1, s, g2, prg);
  pairing_map(t2, h, v,  prg);
  rslt = element_cmp(t1, t2) == 0 ? true : false;

  // *** finalization ***
  element_clear(t1);
  element_clear(t2);

  return rslt;
}

void AggSig::get_g2()
{
  // init
  mpz_set(tmp, prg->g2->cofactor);
  // gmp_printf ("%s is an mpz %Zd\n", "cofactor", tmp);
  EC_POINT P;
  point_init(P, prg->g2);

  do {
    point_random(P);
    point_mul(g2, tmp, P);
    // cout << point_is_infinity(g2) << endl;
  } while (point_is_infinity(g2) != 0) ;   // exit not infinity
  // point_print("g2", g2);

  // clear
  point_clear(P);
}

void  AggSig::set_mpz_from_element(mpz_t tmp, Element x)
{
  int size = element_get_str_length(x);
  char *str = new char[size];

  element_get_str(str, x);

  mpz_set_str(tmp, str, 16);
  // gmp_printf ("%s is an mpz %Zx\n", "value of x", tmp);

  delete [] str;
}

void AggSig::point_print(const char *s, const EC_POINT x)
{
  int size = point_get_str_length(x);
  char *str = (char*)malloc(sizeof(char)*size);

  point_get_str(str, x);
  printf("%s: %s\n", s, str);

  free(str);
}

void AggSig::element_print(const char *s, const Element x)
{
  int size = element_get_str_length(x);
  char *str = new char[size];

  element_get_str(str, x);
  cout << s << str << endl;

  delete [] str;
}
