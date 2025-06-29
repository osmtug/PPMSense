#pragma once

#include "IView.hpp"
#include <string>
#include <vector>
#include <thread>
#include <Eigen/Dense>
#include "NeuralNetwork.hpp"

class SaveNetworkView : public IView {
public:
    SaveNetworkView(const NeuralNetwork& neuralNetwork);
    void couperDernierDossier(char* path);
    void render() override;

private:
    char path[1024];
    char fileName[1024];
    NeuralNetwork trainedNN;
};