#include <vector>
#include <functional>
#include "../layer/layer.h"

class Network {
    public:
        std::vector<Layer> layers;
        double learningrate;
        Network(double lr, std::string activefunc){};
        std::vector<float> forward(std::vector<float> input){};
    };