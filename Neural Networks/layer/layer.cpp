#include <vector>
#include <string>
#include <iostream>
#include "../neuron/neuron.h"

class Layer {
    std::vector<Neuron> neurons;
    std::string activationType;
public:
    Layer(int numNeurons, const std::vector<std::vector<float>>& weights,
          const std::vector<float>& biases, const std::string& actiType)
    {
        activationType = actiType;
        if (weights.size() != numNeurons || biases.size() != numNeurons)
            throw std::invalid_argument("Weights and biases must match number of neurons");

        for (int i = 0; i < numNeurons; ++i)
            neurons.emplace_back(weights[i], biases[i], activationType);
    }

    std::vector<float> forward(std::vector<float>& input) {
        std::vector<float> outputs(neurons.size());
        for (auto& neuron : neurons)
            outputs.push_back(neuron.forward(input, activationType));

        return outputs;
    }
};