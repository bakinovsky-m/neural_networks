#include <iostream>
#include <vector>
#include <memory>
#include <random>
#include <chrono>
#include <iomanip>

#include "neuron.h"
#include "layer.h"
#include "network.h"

using namespace std;
using FLN = FirstLayerNeuron;

static random_device rd;
static default_random_engine rand_eng(rd());

void printNNOut(const Network & n)
{
  for(auto & o : n.output)
  {
    cout << std::setprecision(1) << o << " ";
  }
  cout << endl;
}

int main()
{
  uniform_real_distribution<double> rand (0.0001,1);
//  uniform_int_distribution<int> rand (0,1);
//  uniform_int_distribution<int> rand (0,1);

//  FLN fl {1};

  Network n {{1, 0}};
//  Network n {{1}};
//  n.addLayer(5);
  n.addLayer(2);
  n.addLayer(2);
  n.addLayer(4);
//  n.addLayer(20);
//  n.addLayer(2);
  n.addLayer(1);

  auto begin = chrono::steady_clock::now();

  uint64_t counterr = 0;
  while(true && counterr <= 10000)
  {
//    int cur_v1 = rand(rand_eng);
    double cur_v1 = rand(rand_eng);
//    int cur_v2 = rand(rand_eng);
//    double cur_v2 = rand(rand_eng);
    vector<double> v {cur_v1};
//    vector<double> v {cur_v1, cur_v2};
    n.setInput(v);


    double true_ans = (cur_v1 >= 0.5) ? 1.0 : 0.0;
    double err = n.train({true_ans}, 0.5);
//    double err = n.train(v, 0.5);

    cout << counterr++ << ":err: " << err << endl;

    if (err < 0.0001)
      break;
  }

  auto end = chrono::steady_clock::now();

  cout << "time for training: " << chrono::duration_cast<chrono::milliseconds>(end - begin).count() << " msec" << endl;

//  n.setInput({0,0});
//  n.run();
//  cout << "ans (0, 0): ";
//  printNNOut(n);

//  n.setInput({1,0});
//  n.run();
//  cout << "ans (1, 0): ";
//  printNNOut(n);

//  n.setInput({0,1});
//  n.run();
//  cout << "ans (0, 1): ";
//  printNNOut(n);

//  n.setInput({1,1});
//  n.run();
//  cout << "ans (1, 1): ";
//  printNNOut(n);

  n.setInput({0.1});
  n.run();
  cout << "ans (1): ";
  printNNOut(n);

  n.setInput({0.3});
  n.run();
  cout << "ans (1): ";
  printNNOut(n);

  n.setInput({0.4});
  n.run();
  cout << "ans (1): ";
  printNNOut(n);

  n.setInput({0.6});
  n.run();
  cout << "ans (1): ";
  printNNOut(n);

  n.setInput({0.8});
  n.run();
  cout << "ans (1): ";
  printNNOut(n);

  n.saveToFile("tmp.nn");

  n.loadFromFile("tmp.nn");

  n.setInput({0.8});
  n.run();
  cout << "ans (1): ";
  printNNOut(n);

  n.saveToFile("tmp.nn");

  n.loadFromFile("tmp.nn");

  n.setInput({0.8});
  n.run();
  cout << "ans (1): ";
  printNNOut(n);
}
