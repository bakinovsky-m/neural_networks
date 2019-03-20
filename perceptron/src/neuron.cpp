#include "neuron.h"

#include <vector>
#include <cmath>
#include <random>

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

HiddenNeuron::HiddenNeuron(const vector<shared_ptr<Neuron>> v){
  uniform_real_distribution<double> rand(0, 0.1);
  for(auto el : v)
  {
    inputs.push_back(el);
    weights.push_back(rand(rand_eng));
  }
}

double HiddenNeuron::output()
{
  double res = 0;
  for(size_t i = 0; i < inputs.size(); ++i)
  {
    res += inputs[i]->output() * weights[i];
  }
  return sigma(res);
}

void FirstLayerNeuron::set_value(const double v)
{
  value = v;
}

double FirstLayerNeuron::output()
{
  return value;
}

Neuron::~Neuron()
{

}
