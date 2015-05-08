#undef NDEBUG
#include <iostream>
#include <string>
#include <assert.h>
#include <time.h>

#include "agg_sig.h"
 
// len 文字のランダムな文字列を作成する関数
string str_gen(char len)
{
  string str_data = "";
  string material = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

  for (int i=0; i<len; i++)
    str_data.append (1, material[rand()%(material.size()-1)]);

  return str_data;
}
int main()
{
  // 乱数のシードを現在の時刻で初期化
  srand((unsigned)time(NULL));
  const unsigned int num = 1000;
  const char str_len = 10;
  string text_data[num];

  // len文字のnum個の文字列を作成する
  for (int i=0; i<num; i++)
    text_data[i] = str_gen(str_len);

  /*
  for (int i=0; i<num; i++)
    cout << text_data[i] << endl;
  /**/

  cout << "+-------------------------+\n";
  cout << "text length = " << (int)str_len << endl;
  cout << "number of text = " << (int)num << endl;
  cout << "+-------------------------+\n";


  /* normal signature speed determination */

  cout << "********* normal sig time ********* \n";
  clock_t start = clock();    // スタート時間

  Sig::init();
  Sig hoge[num];
  bool r1, r2;

  clock_t sign_st = clock();
  for (int i=0; i<num; i++) {
    hoge[i].sign(text_data[i]);
  }
  clock_t sign_et = clock();

  clock_t vrfy_st = clock();
  for (int i=0; i<num; i++) {
    // r1 = false; r2 = false;
    r1 = hoge[i].vrfy(text_data[i]);
    // assert( true  == r1 );
  }

  clock_t end = clock();     // 終了時間
  cout << "sign clock = " << (sign_et - sign_st) << endl;
  cout << "verify clock = " << (end - vrfy_st) << endl;
  cout << "entire clock = " << (end - start) << endl;
  cout << "entire duration = " << (double)(end - start) / CLOCKS_PER_SEC << "sec.\n";



  /* aggregate signature speed determination */

  cout << "********* aggregate sig time ********* \n";
  start = clock();    // スタート時間

  AggSig fuga;
  Sig* sigs = new Sig[num];

  sign_st = clock();
  for (int i=0; i< num; i++) {
    sigs[i].sign(text_data[i]);
    // r1 = sigs[i].vrfy(text_data[i]);
    // assert( true  == r1 );
  }
  sign_et = clock();

  clock_t agg_st = clock();
  fuga.agg( sigs, num);
  clock_t agg_et = clock();

  vrfy_st = clock();
  r2 = fuga.vrfy(text_data, sigs, num);
  // assert( true == r2 );

  end = clock();     // 終了時間
  cout << "sign clock = " << (sign_et - sign_st) << endl;
  cout << "aggregate clock = " << (agg_et - agg_st) << endl;
  cout << "verify clock = " << (end - vrfy_st) << endl;
  cout << "entire clock = " << (end - start) << endl;
  cout << "entire duration = " << (double)(end - start) / CLOCKS_PER_SEC << "sec.\n";

  // apend

  // delete
  delete [] sigs;

  // Sig::fin();
  
  return 0;
}
