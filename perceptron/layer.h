#ifndef LAYER_H
#define LAYER_H

#include <cstdint>
#include <vector>
#include <memory>
#include <utility>

#include "neuron.h"

struct Layer
{
  std::vector<std::shared_ptr<Neuron>> neurons;
};

struct FirstLayer : Layer
{
  FirstLayer() = default;
  FirstLayer(std::vector<std::shared_ptr<FirstLayerNeuron>> v_);
//  std::vector<std::shared_ptr<Neuron>> neurons;
};

struct HiddenLayer : Layer
{
  HiddenLayer(Layer &l, const size_t n_count);
//  HiddenLayer(FirstLayer &l, const size_t n_count);
//  std::vector<std::shared_ptr<Neuron>> neurons;
//  std::vector<std::variant<
};

#endif // LAYER_H