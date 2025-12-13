#include <vector>
#include <string>

class Neuron {
    public:
        std::vector<float> w;
        float b;
        std::string activationType;

        Neuron(std::vector<float>& w, float b);
        float forward(std::vector<float>& input, const std::string& activationType);
};