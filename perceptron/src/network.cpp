#include "network.h"

#include <memory>
#include <cmath>
#include <iterator>
#include <fstream>
#include <sstream>

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
  output.resize(hidden_layers.back().neurons.size());
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
    size_t i = 0;
    for(auto &inp : nn->inputs)
    {
      double out_w = inp.first->output();
      double delta = nu * gamma * out_w;
      nn->new_ws[i++] = inp.second - delta;
//      inp.second -= delta;
    }
  }


  for(auto ll = (hidden_layers.rbegin() + 1); ll != hidden_layers.rend(); ++ll)
  {
    for(auto & nn : ll->neurons)
    {
      shared_ptr<HiddenNeuron> n = dynamic_pointer_cast<HiddenNeuron>(nn);
      double n_out = n->output();
      double ho = n_out * (1 - n_out);
      double gamma = 0;
      for(auto & out_nn : (ll - 1)->neurons)
      {
        shared_ptr<HiddenNeuron> out_n = dynamic_pointer_cast<HiddenNeuron>(out_nn);
        auto f = out_n->inputs.find(nn);
        if(f != out_n->inputs.end())
        {
          double weig = out_n->inputs.find(nn)->second;
          gamma += out_n->gamma * weig;
        }
      }

      n->gamma = gamma;

      size_t i = 0;
      for(auto & inp : n->inputs)
      {
        double ifo = inp.first->output();
        double delta = nu * ifo * gamma * ho;
        n->new_ws[i++] = inp.second - delta;
//        inp.second -= delta;
      }
    }
  }


  for(auto n = hl.neurons.begin(); n != hl.neurons.end(); ++n)
  {
    shared_ptr<HiddenNeuron> nn = dynamic_pointer_cast<HiddenNeuron>(*n);
    size_t i = 0;
    for(auto &inp : nn->inputs)
    {
      inp.second = nn->new_ws[i++];
    }
  }

  for(auto ll = (hidden_layers.rbegin() + 1); ll != hidden_layers.rend(); ++ll)
  {
    for(auto & nn : ll->neurons)
    {
      shared_ptr<HiddenNeuron> n = dynamic_pointer_cast<HiddenNeuron>(nn);
      size_t i = 0;
      for(auto & inp : n->inputs)
      {
        inp.second = n->new_ws[i++];
      }
    }
  }


  return err_total * 0.5;
}

void Network::saveToFile(const string & fname) const
{
  ofstream f;
  f.open(fname);

  f << fl.neurons.size() << endl;

  f << hidden_layers.size() << endl;

  for(auto &l : this->hidden_layers)
  {
    f << l.neurons.size() << endl;
    for(auto & nn : l.neurons)
    {
      auto n = dynamic_pointer_cast<HiddenNeuron>(nn);
      for(auto &nw : n->inputs)
      {
        f << nw.second << " ";
      }
      f << endl;
    }
  }

  f.close();
}

void Network::loadFromFile(const string &fname)
{
  hidden_layers.clear();
  ifstream f;
  string s;
  stringstream ss;
  f.open(fname);

  size_t fl_size = 0;

  getline(f, s);
  ss << s;
  ss >> fl_size;

  addLayer(fl_size);

  size_t hidden_layers_count = 0;

  getline(f, s);
  ss.clear();
  ss << s;
  ss >> hidden_layers_count;

  Layer prev = fl;

  for(size_t i = 0; i < hidden_layers_count; ++i)
  {
    size_t layer_size = 0;
    getline(f, s);
    ss.clear();
    ss << s;
    ss >> layer_size;

    vector<vector<double>> vv;

    for(size_t j = 0; j < layer_size; ++j)
    {
      vector<double> v;
      getline(f, s);
      ss.clear();
      ss << s;
      for(size_t k = 0; k < fl_size + 1; ++k)
      {
        double cur = 0;
        ss >> cur;
        v.push_back(cur);
      }
      vv.push_back(v);
    }
    HiddenLayer hl{prev, vv, layer_size};
    hidden_layers.push_back(hl);
    fl_size = layer_size;
  }

  f.close();
}
