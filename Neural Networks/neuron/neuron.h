#include <vector>
#include <string>

class Neuron {
    std::vector<float> x, w;
    float b;
    std::string activationType;

public:
    Neuron(std::vector<float> xVal, std::vector<float> wVal, float bVal, const std::string& activationFunctionType);
    std::vector<float> compute();
};