#include "../network/network.h"
#include <cmath>

class Backpropagation {
public:
    Network& network;

    Backpropagation(Network& net) : network(net) {}

    void backward(const std::vector<float>& target) {
        int L = network.layers.size() - 1;

        for (size_t i = 0; i < network.layers[L].neurons.size(); i++) {
            Neuron& n = network.layers[L].neurons[i];
            n.delta = (n.activatedOutput - target[i]) * n.activationDerivative();
        }

        for (int l = L - 1; l >= 0; l--) {
            Layer& current = network.layers[l];
            Layer& next = network.layers[l + 1];

            for (size_t j = 0; j < current.neurons.size(); j++) {
                float sum = 0.0f;
                
                for (size_t k = 0; k < next.neurons.size(); k++) {
                    sum += current.neurons[j].weights[k]
                         * next.neurons[k].delta;
                }

                current.neurons[j].delta =
                    current.neurons[j].activationDerivative() * sum;
            }
        }
    }

    void updateWeights() {
        for (auto& layer : network.layers) {
            for (auto& neuron : layer.neurons) {
                for (size_t i = 0; i < neuron.weights.size(); i++) {
                    neuron.weights[i] -=
                        network.learningrate *
                        neuron.previousInput[i] *
                        neuron.delta;
                }
                neuron.bias -= network.learningrate * neuron.delta;
            }
        }
    }
};
