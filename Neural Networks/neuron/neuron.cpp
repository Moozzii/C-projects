#include <cmath>
#include <string>
#include <vector>
#include <map>
#include <functional>
#include <iostream>
#include <algorithm>

class Neuron {
    std::vector<float> x, w;
    float b;
    std::function<float(float)> activation;

    inline static std::map<std::string, std::function<float(float)>> activations = {
        {"ReLU", [](float v){ return std::max(0.0f, v); }},
        {"Sigmoid", [](float v){ return 1.0f / (1.0f + std::exp(-v)); }},
        {"tanh", [](float v){ return std::tanh(v); }}
    };

public:
    Neuron(const std::vector<float>& xVal, const std::vector<float>& wVal, float bVal, const std::string& activationType)
        : x(xVal), w(wVal), b(bVal) 
    {
        if (activations.find(activationType) == activations.end())
            throw std::invalid_argument("Unknown activation function");
        activation = activations[activationType];
    }

    std::vector<float> compute() const {
        std::vector<float> linearOutputs(x.size());
        for (size_t i = 0; i < x.size(); ++i)
            linearOutputs[i] = w[i] * x[i] + b;

        std::vector<float> activatedOutputs(x.size());
        std::transform(linearOutputs.begin(), linearOutputs.end(), activatedOutputs.begin(), activation);
        return activatedOutputs;
    }
};