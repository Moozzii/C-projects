#include "../network/network.h"
#include <cmath>

class Backpropagation { 
    std::vector<float> predVector;
    Network network;
    public:
        Backpropagation(Network net) : network(net){};
        float lossFunction(std::vector<float> &prediction, std::vector<float> &actualVals) {
            float loss = 0.0f;
            for(size_t i=0; i<prediction.size(); i++){
                loss += 0.5f * (pow(prediction[i] - actualVals[i], 2));      
            }
            return loss;   
        }

        void backProp(std::vector<float> &prediction, std::vector<float> &actualVals) {
            std::vector<float> delta(prediction.size());
            size_t lastLayer = network.layers.size() - 1;
            for (size_t i = 0; i < prediction.size(); i++) {
                float slope = network.layers[lastLayer].neurons[i].activationDerivative();
                delta[i] = (prediction[i] - actualVals[i]) * slope;
            }


        }
   
};