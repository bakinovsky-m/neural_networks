#ifndef NETWORK_H
#define NETWORK_H

#include <vector>

#include <layer.h>
#include <neuron.h>

struct Network
{
  Network() = delete;
  Network(const std::vector<double> & inp);
  void setInput(const std::vector<double> & inp);
  void addFirstLayer(const size_t neurons_count);
  void addLayer(const size_t neurons_count);
  void run();
  double err(const std::vector<double> true_ans) const;

  double train(const std::vector<double> true_output, const double nu);

  std::vector<double> input;
  std::vector<double> output;
  FirstLayer fl;
  std::vector<HiddenLayer> hidden_layers;
//  HiddenLayer output_layer;
};

#endif // NETWORK_H
