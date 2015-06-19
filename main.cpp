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
  const int size = 3;
  const int tmp = 1;
  string msgs[size] =  { "fuga1" , "fuga2", "fuga3" };
  string msgs1[size] = { "fuga1" , "fuga3", "hoge" };
  string msgs2[size] = { "fuga3" , "fuga2", "fuga1" };
  Sig* sigs = new Sig[size];

  for (int i=0; i< size; i++) {
    sigs[i].sign(msgs[i]);
    r1 = sigs[i].vrfy(msgs[i]);
    assert( true  == r1 );
  }
  fuga.agg( sigs, size-tmp);
  r2 = fuga.vrfy(msgs, sigs, size-tmp);
  assert( true == r2 );

  // 署名の追加
  fuga.append( (sigs+(size-tmp)), tmp );
  r2 = fuga.vrfy(msgs, sigs, size);
  assert( true == r2 );

  r2 = fuga.vrfy(msgs1, sigs, size);
  assert( false == r2 );

  // メッセージの順番を逆にした時
  r2 = fuga.vrfy2(msgs2, sigs, size);
  assert( true == r2 );

  // apend

  // delete
  delete [] sigs;

  Sig::fin();
  return 0;
}
