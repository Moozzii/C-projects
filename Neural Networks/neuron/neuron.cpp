#include <vector>
#include <string>
#include <functional>
#include <map>
#include <cmath>
#include <algorithm>
#include <stdexcept>

class Neuron {
    public:
        std::vector<float> weights;
        float bias;
        std::function<float(float)> activation;

        inline static std::map<std::string, std::function<float(float)>> activations = {
            {"ReLU", [](float v){ return std::max(0.0f, v); }},
            {"Sigmoid", [](float v){ return 1.0f / (1.0f + std::exp(-v)); }},
            {"tanh", [](float v){ return std::tanh(v); }}
        };

    Neuron(std::vector<float>& w, float b, const std::string& activationType)
        : weights(w), bias(b)
    {
        if (activations.find(activationType) == activations.end())
            throw std::invalid_argument("Unknown activation function");
        activation = activations[activationType];
    }

    float forward(std::vector<float>& input) const {
        if (input.size() != weights.size())
            throw std::invalid_argument("Input size must match weights size");

        float sum = 0.0f;
        for (size_t i = 0; i < input.size(); ++i)
            sum += input[i] * weights[i];

        sum += bias;
        return activation(sum);
    }
};