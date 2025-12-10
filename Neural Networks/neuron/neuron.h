#include <vector>
#include <string>

class Neuron {
    std::vector<float> w;
    float b;
    std::string activationType;

public:
    Neuron(const std::vector<float>& w, float b, const std::string& activationType);
    float forward(const std::vector<float>& input);
};