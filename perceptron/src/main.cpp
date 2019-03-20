#include <iostream>
#include <vector>
#include <memory>
#include <random>

#include "neuron.h"
#include "layer.h"
#include "network.h"

using namespace std;
using FLN = FirstLayerNeuron;

static random_device rd;
static default_random_engine rand_eng(rd());

int main()
{
  uniform_real_distribution<double> rand (0,100);

//  FLN fl {1};

  Network n {{1}};
  n.addLayer(5);
  n.addLayer(1);

  while(true)
  {
    static size_t counter = 0;
    double cur_v = rand(rand_eng);
    n.train({cur_v}, 0.001);

    double ans = (cur_v <= 50) ? 1.0 : 0.0;

    cout << counter++ << ":err: " << n.err({ans}) << endl;

    if (n.err({ans}) < 0.01)
      break;
  }

  n.setInput({15});
  n.run();
  cout << "ans: " << n.output[0] << endl;
}
