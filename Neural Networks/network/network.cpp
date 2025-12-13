#include <iostream> 
#include <vector>
#include "../layer/layer.h"
#include <functional>
class Network {
    public:
        std::vector<Layer> layers;
        double learningrate;
        Network(double lr,  std::string activefunc): learningrate(lr){};
        std::vector<float> forward(std::vector<float> input) {
            for(auto &layer: layers) {
               input = layer.forward(input);
            }
            return input;
        }  
};