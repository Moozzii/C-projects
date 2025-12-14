#include <vector>
#include <string>

class Neuron {
    public:
        std::vector<float> weights;
        float bias;
        std::function<float(float)> activation;
        std::vector<float> previousInput;
        std::string activationType;
        float activatedOutput;
        float z;

        Neuron(std::vector<float>& w, float b);
        float forward(std::vector<float>& input, const std::string& activationType);
        float activationDerivative() {
            float a;
            float derivativeOfa;
            return derivativeOfa;
        }
};      