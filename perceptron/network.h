#ifndef NETWORK_H
#define NETWORK_H

#include <vector>

#include <layer.h>
#include <neuron.h>

struct Network
{
  Network() = default;
  Network(const std::vector<double> & inp);
  void setInput(const std::vector<double> & inp);
  void addFirstLayer(const size_t neurons_count);
  void addLayer(const size_t neurons_count);
  void run();
  double err(const std::vector<double> true_ans) const;

  void train(const std::vector<double> true_output, const double nu);

  std::vector<double> input;
  std::vector<double> output;
  FirstLayer fl;
  std::vector<HiddenLayer> layers;
};

#endif // NETWORK_H
