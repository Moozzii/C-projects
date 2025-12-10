#include <vector>
#include <string>
#include "../neuron/neuron.h"
class Layer {
private:
    std::vector<Neuron> neurons;

public:
    Layer(int numNeurons,
          const std::vector<std::vector<float>>& weights,
          const std::vector<float>& biases,
          const std::string& activationType);

    std::vector<float> forward(const std::vector<float>& input);
};
