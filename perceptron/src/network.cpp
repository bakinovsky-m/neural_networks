#include "network.h"

#include <memory>
#include <cmath>
#include <iterator>

using namespace std;

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

  size_t i = 0;
  for(auto &fl_n : fl.neurons)
  {
    shared_ptr<FirstLayerNeuron> fl_neuro = dynamic_pointer_cast<FirstLayerNeuron>(fl_n);
    fl_neuro->value = inp[i++];
  }
}

void Network::addLayer(const size_t neurons_count)
{
  if (fl.neurons.size() == 0)
  {
    throw invalid_argument("");
  }
  Layer prev;
  if (hidden_layers.size() == 0)
    prev = fl;
  else
    prev = hidden_layers.back();
  HiddenLayer hl (prev, neurons_count);
  hidden_layers.push_back(hl);
  output.resize(hl.neurons.size());
}

void Network::run()
{
  for(size_t i = 0; i < output.size(); ++i)
  {
    auto l = hidden_layers.back();
    shared_ptr<Neuron> n = l.neurons[i];
    double out = n->output();
    output[i] = out;
  }
}

double Network::train(const std::vector<double> target_output, const double nu)
{
  double err_total = 0;
  HiddenLayer & hl = hidden_layers.back();
  size_t target_number = 0;

  for(auto n = hl.neurons.begin(); n != hl.neurons.end(); ++n, ++target_number)
  {
    shared_ptr<HiddenNeuron> nn = dynamic_pointer_cast<HiddenNeuron>(*n);

    double n_out = nn->output();
    double err_i = pow(target_output[target_number] - n_out, 2);
    nn->err = err_i;
    err_total += err_i;

    double gamma = -(target_output[target_number] - n_out) * n_out * (1 - n_out);
    nn->gamma = gamma;
    for(auto &inp : nn->inputs)
    {
      double out_w = inp.first->output();
      double delta = nu * gamma * out_w;
      inp.second -= delta;
    }
  }

//  for(auto ll = (hidden_layers.rbegin() + 1); ll != hidden_layers.rend(); ++ll)
//  for(auto ll = std::next(hidden_layers.rbegin(), 2); ll != hidden_layers.rend(); ++ll)
  size_t qwe = hidden_layers.size();
  int i = qwe - 2;
  for(; i >= 0; --i)
  {
    auto & ll = hidden_layers.at(i);
//    for(auto & nn : ll->neurons)
    for(auto & nn : ll.neurons)
    {
      shared_ptr<HiddenNeuron> n = dynamic_pointer_cast<HiddenNeuron>(nn);
      double n_out = n->output();
      double ho = n_out * (1 - n_out);
      double gamma = 0;
//      for(auto & out_nn : (ll - 1)->neurons)
      for(auto & out_nn : hidden_layers.at(i+1).neurons)
      {
        shared_ptr<HiddenNeuron> out_n = dynamic_pointer_cast<HiddenNeuron>(out_nn);
        double weig = out_n->inputs.find(nn)->second;
        gamma += out_n->gamma * weig;
      }

      n->gamma = gamma;

      for(auto & inp : n->inputs)
      {
        double ifo = inp.first->output();
//        if (ifo == 0)
//          ifo = 1;
        double delta = ifo * gamma * ho;
        inp.second -= delta;
      }
    }
  }


  return err_total * 0.5;
}
