#undef NDEBUG
#include <iostream>
#include <assert.h>

#include "agg_sig.h"

int main()
{
  Sig::init();
  Sig hoge;
  string m = "hogehoge";
  bool r1, r2;

  hoge.sign(m);
  r1 = hoge.vrfy(m);
  r2 = hoge.vrfy("hogehogf");
  assert( true  == r1 );
  assert( false == r2 );

  AggSig fuga;
  const int size = 2;
  string msgs[size] = { "fuga1" , "fuga2" };
  string msgs1[size] = { "fuga1" , "fuga3" };
  string msgs2[size] = { "fuga2" , "fuga1" };
  Sig sigs[size];

  for (int i=0; i< size; i++) {
    sigs[i].sign(msgs[i]);
    r1 = sigs[i].vrfy(msgs[i]);
    assert( true  == r1 );
  }
  fuga.agg( sigs, size);
  r2 = fuga.vrfy(msgs, sigs, size);
  assert( true == r2 );

  r2 = fuga.vrfy(msgs1, sigs, size);
  assert( false == r2 );
  r2 = fuga.vrfy(msgs2, sigs, size);
  assert( false == r2 );

  // apend

  // delete

  // Sig::fin();
  return 0;
}
