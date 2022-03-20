//
// Created by 75108 on 2022/3/17.
//

#include <iostream>
#include <algorithm>
#include <log.h>

typedef long long ll;

using namespace std;

auto logger = SU_LOG_ROOT;

int main(){
    SU_LOG_ERROR(logger) <<"soemt thing ";
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

   int money[12];
   for(int i = 0;i<12;i++) cin >> money[i];

   const int N = 300;
   int credit = 0 ,left = 0;

   for(int i = 0;i<12;i++){
       left += N;
       if(money[i] > left){
           cout << -i - 1 <<endl;
           return  0;
       }
       left -= money[i];
       credit += (left/100);
       left = left %100;
   }
   cout << credit * 120 + left <<endl;
}
