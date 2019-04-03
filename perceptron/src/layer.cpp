#include "layer.h"

using namespace std;

HiddenLayer::HiddenLayer(Layer &l, const size_t n_count)
{
  for(size_t i = 0; i < n_count; ++i)
  {
//    HiddenLayer ll = static_cast<HiddenLayer&>(l);
    shared_ptr<HiddenNeuron> n = make_shared<HiddenNeuron>(l.neurons);
//    shared_ptr<HiddenNeuron> n (l.neurons);
    neurons.push_back(n);
  }
}

HiddenLayer::HiddenLayer(Layer &l, const std::vector<std::vector<double> > &weights, const size_t n_count)
{
  for(size_t i = 0; i < n_count; ++i)
  {
    shared_ptr<HiddenNeuron> n = make_shared<HiddenNeuron>(l.neurons, weights[i]);
    neurons.push_back(n);
  }
}

FirstLayer::FirstLayer(std::vector<std::shared_ptr<FirstLayerNeuron> > v_)
{
  for(auto &el : v_)
  {
    neurons.push_back(el);
  }
}
