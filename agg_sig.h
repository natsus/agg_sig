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
    void point_print(const char* , const EC_POINT );
/*
    set_message();
    key_gen();
    sign();
    verify();
*/

  private:
    EC_PAIRING prg;
    EC_POINT G1, G2, g2;
    mpz_t p;
    void get_g2(EC_POINT);

};

AggSig::AggSig(){

  pairing_init(prg, "ECBN254");
  point_init(G1, prg->g1);
  point_init(G2, prg->g2);
  point_init(g2, prg->g2);

  get_g2(g2);

  mpz_init(p);
  mpz_set(p, prg->g1->order);

}

void AggSig::get_g2(EC_POINT g2){
  // init
  mpz_t cofactor;
  mpz_init(cofactor);
  mpz_set(cofactor, prg->g2->cofactor);
  // gmp_printf ("%s is an mpz %Zd\n", "cofactor", cofactor);
  EC_POINT P;
  point_init(P, prg->g2);

  do {
    point_random(P);
    point_mul(g2, cofactor, P);
    // cout << point_is_infinity(g2) << endl;
  } while (point_is_infinity(g2) != 0) ;   // exit not infinity
  // point_print("g2", g2);

  // clear
  mpz_clear(cofactor);
  point_clear(P);
}

void AggSig::point_print(const char *s, const EC_POINT x)
{
  int size = point_get_str_length(x);
  char *str = (char*)malloc(sizeof(char)*size);

  point_get_str(str, x);
  printf("%s: %s\n", s, str);

  free(str);
}
