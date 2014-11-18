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
  EC_POINT P;
  point_init(P, prg->g2);

  cout << "hoge\n";
  do {
    point_random(P);
    point_mul(g2, cofactor, P);
  } while (point_is_infinity(g2) == 0) ;   // not infinity

  cout << "hoge\n";
  // clear
  mpz_clear(cofactor);
  point_clear(P);
}
