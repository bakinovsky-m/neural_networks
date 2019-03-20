#include "layer.h"

using namespace std;

HiddenLayer::HiddenLayer(Layer &l, const size_t n_count)
{
  for(size_t i = 0; i < n_count; ++i)
  {
    HiddenLayer ll = static_cast<HiddenLayer&>(l);
    shared_ptr<HiddenNeuron> n = make_shared<HiddenNeuron>(l.neurons);
//    for(auto nn : l.neurons)
//    {
//      n.inputs.push_back(nn);
    neurons.push_back(n);
//    }
  }
}

FirstLayer::FirstLayer(std::vector<std::shared_ptr<FirstLayerNeuron> > v_)
{
  for(auto &el : v_)
  {
    neurons.push_back(el);
  }
}
