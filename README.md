IMPLEMENTATION OF AGGREGATE SIGNATURE
====

I implement Aggregate signature by using TEPLA(University of Tsukuba Elliptic Curve and Pairing Library).

## Detail
I publish program source codes of Aggregate signature of Boneh et al. on 128 bit security. 
> "D. Boneh and M.K. Franklin, “Identity-based encryption from the Weil pairing,” CRYPTO, ed. by J. Kilian, vol.2139, pp.213–229, Lecture Notes in Computer Science, Springer, 2001."  
I implemented this program to evaluate my proposed method in my research,
so I think it an imperfect implemantation about Class interfaces and so on.
If you had some requests, comments or bug reports, I will respond it as much as I can.
And I attach a simple explanation PDF file of my implemantation in Japanese. 
(*[agg_sig_presen.pdf](agg_sig_presen.pdf)*)  
Also I implemented speed determination programs that signs some plaintexts and verifies signatures.

## Environment
* gcc
* TEPLA1.0  
  I create TEPLA image file of Docker, so you can easily install the environment if you used this.
<https://registry.hub.docker.com/u/natsu/tepla/>


## Usage

### Execution of sample code
```sh
$ make
$ ./main
```

**Sample Code**  
Please, refer the following sample code and understand how to use this program.

```cpp
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

  // addition of a signature
  fuga.append( (sigs+(size-tmp)), tmp );
  r2 = fuga.vrfy(msgs, sigs, size);
  assert( true == r2 );

  r2 = fuga.vrfy(msgs1, sigs, size);
  assert( false == r2 );

  // generate an invalid case that reverses the order of messages
  r2 = fuga.vrfy2(msgs2, sigs, size);
  assert( true == r2 );

  // apend

  // delete
  delete [] sigs;

  Sig::fin();
  return 0;
}
```

### Speed determination way
```sh
$ cd speed
$ make
$ ./speed_check_agg
$ ./speed_check_gdh
```
You can assign the number of plaintexts and the length of plaintexts, each value is *num* and *str_len* in each source, *speed_check_agg.cpp* or *speed_check_gdh.cpp*.

## LICENCE

This software is released under the MIT License, see [LICENCE.txt](LICENCE.txt)

## Reference

* [TEPLA](http://www.cipher.risk.tsukuba.ac.jp/tepla/)
