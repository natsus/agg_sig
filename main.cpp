#undef NDEBUG
#include <iostream>
#include <string.h>
#include <assert.h>

#include "agg_sig.h"

int main()
{
  AggSig hoge;
  char* m = "hogehoge";
  bool r1, r2;

  hoge.sign(m, strlen(m));
  r1 = hoge.vrfy(m, strlen(m));
  r2 = hoge.vrfy("hogehogf", strlen("hogehogf"));
  assert( true  == r1 );
  assert( false == r2 );

  return 0;
}
