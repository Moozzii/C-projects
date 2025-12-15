#include <vector>
#include <string>
#include <functional>
#include <map>
#include <cmath>
#include <algorithm>
#include <stdexcept>
class Neuron {
    std::vector<float> weights;
    float bias;
    std::function<float(float)> activation;
    std::vector<float> previousInput;
    std::string activationType;
    float activatedOutput;
    float delta;
    float z;

    inline static std::map<std::string, std::function<float(float)>> activations = {
        {"relu", [](float x){ return std::max(0.0f, x); }},
        {"sigmoid", [](float x){ return 1.0f / (1.0f + std::exp(-x)); }},
        {"tanh", [](float x){ return std::tanh(x); }}
    };
    
    public:
        Neuron(std::vector<float>& w, float b, std::string& activationT)
            : weights(w), bias(b), activationType(activationT)
        {
            std::transform(activationType.begin(), activationType.end(), activationType.begin(), 
                  [](unsigned char c){ return std::tolower(c); }); 
            if (activations.find(activationType) == activations.end())
                throw std::invalid_argument("Unknown activation function");
            activation = activations[activationType];            
        }

        float forward(std::vector<float>& input) {
            previousInput = input;
             if (input.size() != weights.size())
                throw std::invalid_argument("Input size must match weights size");

            float sum = 0.0f;
            for (size_t i = 0; i < input.size(); ++i)
                sum += input[i] * weights[i];

            sum += bias;
            z = sum;
            activatedOutput = activation(z);
            return activatedOutput;
        }
        float activationDerivative() {
            float a;
            float derivativeOfa;
            if(activationType == "tanh"){
                derivativeOfa = 1 - pow(activatedOutput,2);
            }else if (activationType == "relu"){
                derivativeOfa = (z > 0 ? 1 : 0);
            } else if (activationType == "sigmoid"){
                derivativeOfa = activatedOutput * (1 - activatedOutput);
            } else {
                throw std::invalid_argument("Please enter a valid activation function name.");
            }

            return derivativeOfa;
        }
};