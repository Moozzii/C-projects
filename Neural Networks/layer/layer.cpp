#include <vector>
#include <string>
#include <iostream>
#include "../neuron/neuron.h"

class Layer {
    public:
        std::vector<Neuron> neurons;
        std::vector<float> outputs;
        std::string activationType;
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
            
            for (auto& neuron : neurons)
                outputs.push_back(neuron.forward(input, activationType));

            return outputs;
        }
};