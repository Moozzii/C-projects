#include <vector>
#include <string>
#include "../neuron/neuron.h"
class Layer {
    public:
        std::vector<Neuron> neurons;
        std::vector<float> outputs;
        std::string activationType;
        Layer(int numNeurons,
            const std::vector<std::vector<float>>& weights,
            const std::vector<float>& biases,
            const std::string& activationType);

        std::vector<float> forward(const std::vector<float>& input){return outputs;};
};
