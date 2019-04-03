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

//HiddenNeuron::HiddenNeuron(const std::vector<std::shared_ptr<Neuron> > &v, const std::vector<double> &weights, const double bias_w)
HiddenNeuron::HiddenNeuron(const std::vector<std::shared_ptr<Neuron> > &v, const std::vector<double> &weights)
{
  size_t i = 0;
  for(auto &el : v)
  {
    inputs[el] = weights[i++];
    new_ws.push_back(0);
  }
  auto b = make_shared<Bias>();
  inputs[b] = weights[i];
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

double FirstLayerNeuron::output()
{
  return value;
}

Neuron::~Neuron()
{

}
