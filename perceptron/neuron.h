#ifndef HG_NEURON_H
#define HG_NEURON_H

#include <vector>
#include <map>
#include <memory>

struct Neuron
{
  virtual double output() = 0;
  virtual ~Neuron();
  double err = 0;
  double gamma = 0;
};

struct FirstLayerNeuron : Neuron
{
  FirstLayerNeuron(const double v) : value(v) {}

  virtual double output() override;

  double value;
};

struct HiddenNeuron : Neuron
{
  HiddenNeuron() = delete;
  HiddenNeuron(const std::vector<std::shared_ptr<Neuron>>);

  virtual double output() override;

  void renewFl(const std::vector<std::shared_ptr<FirstLayerNeuron>> neurons);

  std::map<std::shared_ptr<Neuron>, double> inputs;
};

struct Bias : Neuron
{
  virtual double output() override {return 1;}
};

#endif // HG_NEURON_H
