#include <vector>
#include "../layer/layer.h"

class Network {
    std::vector<Layer> layers;
    std::vector<float> forward(std::vector<float> input){};
};