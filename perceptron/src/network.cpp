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
  HiddenLayer & hl = static_cast<HiddenLayer&>(layers.front());
  for(auto el : hl.neurons)
  {
    shared_ptr<HiddenNeuron> h = dynamic_pointer_cast<HiddenNeuron>(el);
    h->renewFl(fns_v);
  }
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
    auto l = layers.back();
    shared_ptr<Neuron> n = l.neurons[i];
    double out = n->output();
    output[i] = out;
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
    res += pow(true_ans[i] - tmp_res, 2) * 0.5;
  }
  return res;
}

double Network::train(const std::vector<double> true_output, const double nu)
{
  double err_total = 0;
  HiddenLayer & hl = layers.back();
  size_t i = 0;
  vector<double> errors;

  for(auto n = hl.neurons.begin(); n != hl.neurons.end(); ++n, ++i)
  {
    shared_ptr<HiddenNeuron> nn = dynamic_pointer_cast<HiddenNeuron>(*n);

    double n_out = nn->output();
    double err_i = 0.5 * pow(true_output[i] - n_out, 2);
    errors.push_back(err_i);
    err_total += err_i;

    double gamma = -(true_output[i] - n_out) * n_out * (1 - n_out);
    size_t j = 0;
    for(auto w = nn->weights.begin(); w != nn->weights.end(); ++w, ++j)
    {
      double out_w = nn->inputs[j]->output();
      double delta = nu * gamma * out_w;
      *w -= delta;
    }
  }
  return err_total;
}
