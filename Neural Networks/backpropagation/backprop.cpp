#include "../network/network.h"
#include "../neuron/neuron.h"
#include "../layer/layer.h"
#include <cmath>

class Backpropagation {
    Layer layer;
    Neuron neuron;
    Network network;

    std::vector<float> lossFunction(std::vector<float> &input,std::vector<float> actualVals) {
        std::vector<float> loses;
        std::vector<float> delta;
        std::vector<float> pred = network.forward(input);
        for(size_t i = 0; i < actualVals.size(); i++){
            loses.push_back(pow(pred[i] - actualVals[i], 2));   
            delta.push_back(2 * (pred[i] - actualVals[i]));     
        }
        return loses;
    }
    void backProp(std::vector<float> &input,std::vector<float> actualVals, int epochs, float learningRate) {
        
        
    }



};