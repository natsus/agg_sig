#include <iostream>
using namespace std;

#include <string>
#include <vector>
#include <gmp.h>
// #include <gmpxx.h>

#ifdef __cplusplus
  extern "C" {
    #include <tepla/ec.h>
  }
#endif

// hash size (map to point)
const unsigned hash_size = 256;


void point_print( const string , const EC_POINT );
void element_print( const string , const Element );

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

class Sig
{
  public:
    Sig();
    ~Sig();
    void key_gen();
    void sign( const string );
    bool vrfy( const string );
    void set_sig( EC_POINT );
    void set_sig_inf();
    ec_point_st* get_v();
    ec_point_st* get_sig();
    ec_point_st* get_g2();
    void sig_add( Sig* , Sig* );
    void sig_print( const string );
    static void init()
    {
      mpz_init(tmp);

      pairing_init(prg, "ECBN254");
      point_init(g2, prg->g2);

      gen_g2();
      // cout << "g2: \n";
      // point_print("", g2);

      field_init(f, "bn254_fp");
      // gmp_printf ("%s is an mpz %Zd\n", "order of f", f->order);
    }

    static void fin()
    {
      // field_clear(f);
      point_clear(g2);
      // pairing_clear(prg);
      mpz_clear(tmp);
    }


  private:
    static EC_PAIRING prg;
    static EC_POINT g2;
    EC_POINT G1, G2, v, h, s;
    static Field f ;
    Element x;
    static mpz_t tmp;

    static void gen_g2();
    void set_mpz_from_element( mpz_t , Element );

};

EC_PAIRING Sig::prg;
EC_POINT Sig::g2;
Field Sig::f;
mpz_t Sig::tmp;

Sig::Sig()
{
  point_init(G1, prg->g1);
  point_init(G2, prg->g2);
  point_init(v , prg->g2);
  point_init(h , prg->g1);
  point_init(s , prg->g1);

  element_init(x, f);

  key_gen();
}

Sig::~Sig()
{
  element_clear(x);

  point_clear(G1);
  point_clear(G2);
  point_clear(v);
  point_clear(h);
  point_clear(s);
}

void Sig::set_sig(EC_POINT s)
{
  point_set( this->s , s );
}

void Sig::set_sig_inf()
{
  point_set_infinity( this->s );
}

ec_point_st* Sig::get_v()
{
  return this->v;
}

ec_point_st* Sig::get_sig()
{
  return this->s;
}

ec_point_st* Sig::get_g2()
{
  return this->g2;
}

void Sig::sig_add(Sig* s1, Sig* s2)
{
  /*
  EC_POINT P;
  point_init(P, prg->g1);

  point_add (P, s1->get_sig(), s2->get_sig());
  point_print("s1+s2: ", P);

  point_add (P, s2->get_sig(), s1->get_sig());
  point_print("s2+s1: ", P);

  point_clear(P);
  */


  point_add (this->s, s1->get_sig(), s2->get_sig());
  // point_print("add:   ", this->s);
}

// Key Generation
void Sig::key_gen()
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
void Sig::sign( const string m )
{
  // *** h ← H(M) ***
  point_map_to_point(h, m.c_str(), m.size(), hash_size);
  // cout << m << ": ";
  // point_print("", h);


  // *** generation of signature 's' ***
  set_mpz_from_element(tmp, x); // tmp(mpz_t) ← x(Element)
  point_mul(s, tmp, h); // s ← h^x
}

// Verification
bool Sig::vrfy(const string m )
{
  // *** initialization ***
  Element t1, t2;
  element_init(t1, prg->g3);
  element_init(t2, prg->g3);
  bool rslt = false;


  // *** h ← H(M) ***
  point_map_to_point(h, m.c_str(), m.size(), hash_size);
  // cout << m << ": ";
  // point_print("", h);


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

void Sig::gen_g2()
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
  /*
  point_print("g2", g2);
  point_mul(P, *pairing_get_order(prg), g2);
  point_print("g2 * order", P);
  */

  // clear
  point_clear(P);
}

void Sig::set_mpz_from_element(mpz_t tmp, Element x)
{
  int size = element_get_str_length(x);
  char *str = new char[size];

  element_get_str(str, x);

  mpz_set_str(tmp, str, 16);
  // gmp_printf ("%s is an mpz %Zx\n", "value of x", tmp);

  delete [] str;
}

void Sig::sig_print( const string str )
{
  point_print(str, this->s);
}


class AggSig
{
  public:
    AggSig();
    ~AggSig();
    ec_point_st* get_sig();
    Sig* agg( Sig* , int );
    Sig* append( Sig*, int );
    bool vrfy( string*, Sig* , int );
    bool vrfy2( string*, Sig* , int );
  private:
    Sig asig;   // signature of aggregated
    EC_PAIRING prg;
};

AggSig::AggSig()
{
  pairing_init(prg, "ECBN254");
  asig.set_sig_inf(); // init identity element
}

AggSig::~AggSig()
{
  pairing_clear(prg);
}

ec_point_st* AggSig::get_sig()
{
  return asig.get_sig();
}

// Aggregation
Sig* AggSig::agg(Sig *sigs, int size)
{
  asig.set_sig_inf(); // init identity element
  return append(sigs, size);
}

// append
// a processing that adds a new signature after an aggregation
Sig* AggSig::append(Sig *sigs, int size)
{
  if (size==0) {
    cout << "sigunare is empty. Can't aggregate opt. \n";
    return &asig;
  }
  while (size--, size>=0) {
    // cout<< "size: " << size << endl;
    asig.sig_add(&asig, sigs+size); // asig = asig + sigs[size]
  }
  return &asig;
}

// Aggregation Verification
bool AggSig::vrfy(string *msgs, Sig *sigs, int size)
{
  
  // *** initialization ***
  Element t1, t2;
  Sig tmp;
  element_init(t1, prg->g3);
  element_init(t2, prg->g3);
  element_set_one(t1);
  element_set_one(t2);
  bool rslt = false;

  EC_POINT* hashs = new EC_POINT[size]; 
  for (int i=0; i<size; i++) {
    point_init(hashs[i], prg->g1);
  }


  // *** h ← H(M) ***
  for (int i=0; i<size; i++) {
    point_map_to_point(hashs[i], msgs[i].c_str(), msgs[i].size(), hash_size);
    // cout << msgs[i] << ": ";
    // point_print("", hashs[i]);
  }

  // *** pairing computation ***
  // *** e(s,g2), Π e(hi,vi) ***
  for (int i=0; i<size; i++) {
    // point_print("sig: \n", sigs[i].get_sig());
    pairing_map(t1, hashs[i], sigs[i].get_v(), prg); // t1 = e(hi,vi)
    /*
    element_print("t1: ", t1);
    element_print("t2: ", t2);
    Element t3;
    element_init(t3, prg->g3);

    element_mul (t3, t1, t2); // t2 = t1 * t2
    element_print("t1*t2: ", t3);
    
    element_mul (t3, t2, t1); // t2 = t2 * t1
    element_print("t2*t1: ", t3);
    element_clear(t3);
    */



    element_mul (t2, t1, t2); // t2 = t1 * t2
  }
  pairing_map(t1, asig.get_sig(), asig.get_g2(), prg);
  rslt = element_cmp(t1, t2) == 0 ? true : false;
  // *** finalization ***
  element_clear(t1);
  element_clear(t2);

  for (int i=0; i<size; i++) {
    point_clear(hashs[i]);
  }
  delete [] hashs;

  return rslt;
}

bool AggSig::vrfy2(string *msgs, Sig *sigs, int size)
{
  
  // *** initialization ***
  Element t1, t2;
  Sig tmp;
  element_init(t1, prg->g3);
  element_init(t2, prg->g3);
  element_set_one(t1);
  element_set_one(t2);
  bool rslt = false;

  EC_POINT* hashs = new EC_POINT[size]; 
  for (int i=0; i<size; i++) {
    point_init(hashs[i], prg->g1);
  }


  // *** h ← H(M) ***
  for (int i=0; i<size; i++) {
    point_map_to_point(hashs[i], msgs[i].c_str(), msgs[i].size(), hash_size);
    // cout << msgs[i] << ": ";
    // point_print("", hashs[i]);
  }

  // *** pairing computation ***
  // *** e(s,g2), Π e(hi,vi) ***
  for (int i=0; i<size; i++) {
    // point_print("sig: \n", sigs[i].get_sig());
    pairing_map(t1, hashs[i], sigs[size-i-1].get_v(), prg); // t1 = e(hi,vi)
    /*
    element_print("t1: ", t1);
    element_print("t2: ", t2);
    Element t3;
    element_init(t3, prg->g3);

    element_mul (t3, t1, t2); // t2 = t1 * t2
    element_print("t1*t2: ", t3);
    
    element_mul (t3, t2, t1); // t2 = t2 * t1
    element_print("t2*t1: ", t3);
    element_clear(t3);
    */



    element_mul (t2, t1, t2); // t2 = t1 * t2
  }
  pairing_map(t1, asig.get_sig(), asig.get_g2(), prg);
  rslt = element_cmp(t1, t2) == 0 ? true : false;
  // *** finalization ***
  element_clear(t1);
  element_clear(t2);

  for (int i=0; i<size; i++) {
    point_clear(hashs[i]);
  }
  delete [] hashs;

  return rslt;
}
