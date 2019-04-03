#include "neuron.h"

#include <vector>
#include <cmath>
#include <random>
#include <iostream>

using namespace std;

static double sigma(double x, bool deriv = false)
{
  if (deriv)
  {
    return sigma(x)*(1-sigma(x));
  }
  return 1/(1 + exp(-x));
}

static random_device rd;
static default_random_engine rand_eng{rd()};

HiddenNeuron::HiddenNeuron(const vector<shared_ptr<Neuron>> &v, const double bias_w){
  uniform_real_distribution<double> rand(0.001, 0.1);
  for(auto el : v)
  {
    inputs[el] = rand(rand_eng);
    new_ws.push_back(0);
  }
  auto b = make_shared<Bias>();
  inputs[b] = bias_w;
}

double HiddenNeuron::output()
{
  double res = 0;
  for(auto & el : inputs)
  {
    res += el.first->output() * el.second;
  }

  return sigma(res);
}

void HiddenNeuron::renewFl(const std::vector<std::shared_ptr<FirstLayerNeuron>> neurons)
{
  vector<double> weights;

  auto inp_it = inputs.begin();
  for(;inp_it != inputs.end(); ++inp_it)
  {
    weights.push_back(inp_it->second);
  }

  size_t i = 0;
  for(auto fl_it = neurons.begin(); fl_it != neurons.end(); ++fl_it, ++i)
  {
    inputs[*fl_it] = weights[i];
  }
}

double FirstLayerNeuron::output()
{
  return value;
}

Neuron::~Neuron()
{

}
