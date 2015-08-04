アグリゲート署名実装
====

TEPLA(University of Tsukuba
Elliptic Curve and Pairing Library)を利用したアグリゲート署名の実装

## 詳細
TEPLAで128bitセキュリティのBoneh等のアグリゲート署名を実装したため公開する。
> "D. Boneh and M.K. Franklin, “Identity-based encryption from the Weil pairing,” CRYPTO, ed. by J. Kilian, vol.2139, pp.213–229, Lecture Notes in Computer Science, Springer, 2001."
あくまで、研究の評価を行うために作成したためClass等のインターフェースはまだまだ不十分であると思われるため要望があれば順次対応するつもりである。  
また、バグ等の指摘も歓迎します。  
簡単な実装の解説PDFも添付しています。(*[agg_sig.pdf](agg_sig.pdf)*)  
また、適当な平文に署名をつけ、検証を行う実装速度の測定プログラムも作成しています。

## 環境
* gcc
* TEPLA1.0  
  DockerのTEPLAイメージファイルを作成したためこれを利用すると環境導入が容易になる。
<https://registry.hub.docker.com/u/natsu/tepla/>


## 使い方

### サンプルコードの実行
```sh
$ make
$ ./main
```

**サンプルコード**  
使い方はサンプルコードを参照されたし。

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
```

### 速度測定方法
```sh
$ cd speed
$ make
$ ./speed_check_agg
$ ./speed_check_gdh
```
平文の個数は*speed_check_agg.cpp*と*speed_check_gdh.cpp*のそれぞれの*num*で指定、平文の文字列は*str_len*を変更すると良い。

## ライセンス

This software is released under the MIT License, see [LICENCE.txt](LICENCE.txt)

## 参考文献

* [TEPLA](http://www.cipher.risk.tsukuba.ac.jp/tepla/)
