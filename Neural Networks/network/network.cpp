#include <iostream> 
#include <vector>
#include "../layer/layer.h"

class Network {
    std::vector<Layer> layers;
    public:
       
        std::vector<float> forward(std::vector<float> input) {
            for(auto &layer: layers) {
               input = layer.forward(input);
            }
            return input;
        }  
};