#pragma once

#include <vector>
#include <Eigen/Dense>

class NeuralNetwork {
public:
    NeuralNetwork();
    NeuralNetwork(const std::vector<int>& layersSizes, float learningRate);

    Eigen::VectorXf forward(const Eigen::VectorXf& input);
    void train(const Eigen::VectorXf& input, const Eigen::VectorXf& target);
    void trainDataset(const std::vector<Eigen::VectorXf>& inputs,
        const std::vector<int>& labels,
        int epochs,
        float* progress = nullptr);
    int predict(const Eigen::VectorXf& input);
    float evaluate(const std::vector<Eigen::VectorXf>& inputs, const std::vector<int>& labels);

    bool save(std::string& filename) const;
    bool load(std::string& filename);

private:
    std::vector<int> layers;
    float learningRate;

    std::vector<Eigen::VectorXf> neurons;  // activations par couche
    std::vector<Eigen::VectorXf> deltas;   // erreur par couche
    std::vector<Eigen::VectorXf> biases;   // biais par couche (sauf input)
    std::vector<Eigen::MatrixXf> weights;  // poids entre couches

    void initializeWeightsAndBiases();

    static float sigmoid(float x);
    static float sigmoidDerivative(float y); // y = sigmoid(x)
    
};
