#pragma once

#include "IView.hpp"
#include <string>
#include <vector>
#include <thread>
#include <Eigen/Dense>
#include "NeuralNetwork.hpp"

class CSVPickerView : public IView {
public:
    CSVPickerView();
    void render() override;

    bool loadCSV_Eigen(const char* filename, std::vector<Eigen::VectorXf>& data, std::vector<int>& labels);

private:
    char filePathTrain[1024];
    char filePathTest[1024];
    NeuralNetwork trainedNN;
    float trainProgress = 0.0f;
    bool training = false;
    std::thread trainingThread;

    void train(const std::vector<std::vector<float>>& X, const std::vector<int>& y);
    float evaluate(const std::vector<std::vector<float>>& X, const std::vector<int>& y);
};