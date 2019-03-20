#include "network.h"

#include <memory>
#include <cmath>

using namespace std;

static double sigma(double x, bool deriv = false)
{
  if (deriv)
  {
    return sigma(x)*(1-sigma(x));
  }
  return 1/(1 + exp(-x));
}

Network::Network(const std::vector<double> & inp)
{
  vector<shared_ptr<FirstLayerNeuron>> fns_v;
  for(auto el : inp)
  {
    fns_v.push_back(make_shared<FirstLayerNeuron>(el));
  }
  fl = FirstLayer(fns_v);
}

void Network::setInput(const std::vector<double> &inp)
{
  input = inp;
  vector<shared_ptr<FirstLayerNeuron>> fns_v;
  for(auto el : inp)
  {
    fns_v.push_back(make_shared<FirstLayerNeuron>(el));
  }
  fl = FirstLayer(fns_v);
}

void Network::addLayer(const size_t neurons_count)
{
  if (fl.neurons.size() == 0)
  {
    throw invalid_argument("");
  }
  Layer prev;
  if (layers.size() == 0)
    prev = fl;
  else
    prev = layers.back();
  HiddenLayer hl {prev, neurons_count};
  layers.push_back(hl);
  output.resize(hl.neurons.size());
}

void Network::run()
{
  for(size_t i = 0; i < output.size(); ++i)
  {
    output[i] = layers.back().neurons[i]->output();
  }
}

double Network::err(const std::vector<double> true_ans) const
{
  if(true_ans.size() != output.size())
    throw invalid_argument("true_ans have different dimension than net output");
  double res = 0;
  for(size_t i = 0; i < output.size(); ++i)
  {
    double tmp_res = layers.back().neurons[i]->output();
    res += pow(true_ans[i] - tmp_res, 2);
  }
  return res * 0.5;
}

void Network::train(const std::vector<double> true_output, const double nu)
{
  double last_layer_err = err(true_output);
  for(auto & nn : layers.back().neurons)
  {
    auto n = dynamic_pointer_cast<HiddenNeuron>(nn);
    n->err = last_layer_err;
  }

  auto r = layers.rend();
  for(auto it = layers.rbegin(); it != r; ++it)
  {
    for(auto nn : it->neurons)
    {
      auto n = dynamic_pointer_cast<HiddenNeuron>(nn);
      size_t i = 0;
      for(auto inner_nn : n->inputs)
      {
        inner_nn->err += n->err * n->weights[i++];
      }
    }
  }

  for(auto &l : layers)
  {
    for(auto nn : l.neurons)
    {
      auto n = dynamic_pointer_cast<HiddenNeuron>(nn);
      for(size_t i = 0; i < n->inputs.size(); ++i)
      {
        double e = 0;
        for(size_t j = 0; j < n->inputs.size(); ++j)
        {
          e += n->weights[j]*true_output[j];
        }
        n->weights[i] += nu * n->err * true_output[i] * sigma(e, true);
      }
    }
  }

  for(auto &l : layers)
  {
    for(auto nn : l.neurons)
    {
      auto n = dynamic_pointer_cast<HiddenNeuron>(nn);
      n->err = 0;
    }
  }
}
