#include <iostream>
#include <vector>
#include <memory>
#include <random>
//#include <thread>
#include <chrono>

#include "neuron.h"
#include "layer.h"
#include "network.h"

using namespace std;
using FLN = FirstLayerNeuron;

static random_device rd;
static default_random_engine rand_eng(rd());

int main()
{
//  uniform_real_distribution<double> rand (0,100);
  uniform_int_distribution<int> rand (0,1);

//  FLN fl {1};

  Network n {{1, 0}};
//  n.addLayer(5);
//  n.addLayer(4);
//  n.addLayer(3);
//  n.addLayer(2);
  n.addLayer(1);

  auto begin = chrono::steady_clock::now();

  uint64_t counterr = 0;
  while(true && counterr++ <= 10000)
  {
    static size_t counter = 0;
    int cur_v1 = rand(rand_eng);
    int cur_v2 = rand(rand_eng);
//    cout << cur_v1 << " " << cur_v2 << endl;
    vector<double> v {(double)cur_v1, (double)cur_v2};
    n.setInput(v);

//    double true_ans = (cur_v1 == 1 && cur_v2 == 1) ? 1.0 : 0;
    double true_ans = 0;
    if(cur_v1 == 1 && cur_v2 == 1)
      true_ans = 1;
    double err = n.train({true_ans}, 1);

    cout << counter++ << ":err: " << err << endl;

    if (err < 0.01)
      break;
  }

  auto end = chrono::steady_clock::now();

  cout << "time for training: " << chrono::duration_cast<chrono::milliseconds>(end - begin).count() << endl;

//  cout << "OUTPUT SIZE " << n.output.size() << endl;

  n.setInput({1,0});
  n.run();
//  cout << "OUTPUT SIZE after run " << n.output.size() << endl;
  cout << "ans: " << n.output[0] << endl;

  n.setInput({1,1});
  n.run();
//  cout << "OUTPUT SIZE after run " << n.output.size() << endl;
  cout << "ans: " << n.output[0] << endl;
}
