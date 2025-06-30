#include "NeuralNetwork.hpp"
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <fstream>

NeuralNetwork::NeuralNetwork() : learningRate(0.1f) {
    // initialisation vide ou par défaut
}

NeuralNetwork::NeuralNetwork(const std::vector<int>& layersSizes, float lr)
    : layers(layersSizes), learningRate(lr)
{
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    neurons.resize(layers.size());
    deltas.resize(layers.size());

    for (size_t i = 0; i < layers.size(); ++i) {
        neurons[i] = Eigen::VectorXf::Zero(layers[i]);
        deltas[i] = Eigen::VectorXf::Zero(layers[i]);
    }

    initializeWeightsAndBiases();
}

void NeuralNetwork::initializeWeightsAndBiases() {
    weights.clear();
    biases.clear();

    for (size_t i = 1; i < layers.size(); ++i) {
        int currSize = layers[i];
        int prevSize = layers[i - 1];

        Eigen::MatrixXf W = Eigen::MatrixXf::Random(currSize, prevSize);
        weights.push_back(W);

        Eigen::VectorXf b = Eigen::VectorXf::Random(currSize);
        biases.push_back(b);
    }
}

Eigen::VectorXf NeuralNetwork::forward(const Eigen::VectorXf& input) {
    neurons[0] = input;

    for (size_t l = 1; l < layers.size(); ++l) {
        neurons[l] = (weights[l - 1] * neurons[l - 1]) + biases[l - 1];
        neurons[l] = neurons[l].unaryExpr(&NeuralNetwork::sigmoid);
    }

    return neurons.back();
}

void NeuralNetwork::train(const Eigen::VectorXf& input, const Eigen::VectorXf& target) {
    forward(input);

    int L = static_cast<int>(layers.size()) - 1;

    deltas[L] = (target - neurons[L]).cwiseProduct(
        neurons[L].unaryExpr(&NeuralNetwork::sigmoidDerivative)
    );

    for (int l = L - 1; l > 0; --l) {
        deltas[l] = (weights[l].transpose() * deltas[l + 1]).cwiseProduct(
            neurons[l].unaryExpr(&NeuralNetwork::sigmoidDerivative)
        );
    }

    for (int l = 0; l < L; ++l) {
        biases[l] += learningRate * deltas[l + 1];
        weights[l] += learningRate * deltas[l + 1] * neurons[l].transpose();
    }
}

void NeuralNetwork::trainDataset(const std::vector<Eigen::VectorXf>& inputs,
    const std::vector<int>& labels,
    int epochs,
    float* progress)
{
    int totalSteps = epochs * static_cast<int>(inputs.size());
    int step = 0;

    int outputSize = layers.back();
    Eigen::VectorXf target = Eigen::VectorXf::Zero(outputSize);

    for (int epoch = 0; epoch < epochs; ++epoch) {
        for (size_t i = 0; i < inputs.size(); ++i) {
            target.setZero();
            target(labels[i]) = 1.0f;

            train(inputs[i], target);

            if (progress) {
                ++step;
                *progress = static_cast<float>(step) / totalSteps;
            }
        }
    }

    if (progress) *progress = 1.0f;
}

int NeuralNetwork::predict(const Eigen::VectorXf& input) {
    Eigen::VectorXf output = forward(input);
    Eigen::VectorXf::Index maxIndex;
    output.maxCoeff(&maxIndex);
    return static_cast<int>(maxIndex);
}

float NeuralNetwork::evaluate(const std::vector<Eigen::VectorXf>& inputs,
    const std::vector<int>& labels)
{
    int correct = 0;
    for (size_t i = 0; i < inputs.size(); ++i) {
        if (predict(inputs[i]) == labels[i]) ++correct;
    }
    return static_cast<float>(correct) / inputs.size();
}

float NeuralNetwork::sigmoid(float x) {
    return 1.0f / (1.0f + std::exp(-x));
}

float NeuralNetwork::sigmoidDerivative(float y) {
    return y * (1.0f - y);
}

static void mettreFormatNNO(std::string& s)
{
    if (s.size() > 4)
    {
        if (s[s.size() - 1] != 'o' || s[s.size() - 2] != 'n' || s[s.size() - 3] != 'n' || s[s.size() - 4] != '.')
        {
            s.push_back('.');
            s.push_back('n');
            s.push_back('n');
            s.push_back('o');
        }
    }
    else
    {
        s.push_back('.');
        s.push_back('p');
        s.push_back('p');
        s.push_back('m');
    }
}

bool NeuralNetwork::save(std::string& filename) const {
    mettreFormatNNO(filename);
    std::ofstream file(filename);
    if (!file.is_open()) return false;

    // Sauvegarde du nombre de couches
    file << layers.size() << "\n";

    // Sauvegarde des tailles de chaque couche
    for (auto size : layers) {
        file << size << " ";
    }
    file << "\n";

    // Sauvegarde des poids
    for (const auto& W : weights) {
        // dimensions matrice poids
        file << W.rows() << " " << W.cols() << "\n";

        for (int i = 0; i < W.rows(); ++i) {
            for (int j = 0; j < W.cols(); ++j) {
                file << W(i, j) << " ";
            }
            file << "\n";
        }
    }

    // Sauvegarde des biais
    for (const auto& b : biases) {
        file << b.size() << "\n";
        for (int i = 0; i < b.size(); ++i) {
            file << b(i) << " ";
        }
        file << "\n";
    }

    file.close();
    return true;
}

bool NeuralNetwork::load(std::string& filename) {
    mettreFormatNNO(filename);
    std::ifstream file(filename);
    if (!file.is_open()) return false;

    size_t numLayers;
    file >> numLayers;
    layers.resize(numLayers);

    for (size_t i = 0; i < numLayers; ++i) {
        file >> layers[i];
    }

    weights.clear();
    biases.clear();

    // Chargement des poids
    for (size_t i = 1; i < numLayers; ++i) {
        int rows, cols;
        file >> rows >> cols;

        Eigen::MatrixXf W(rows, cols);
        for (int r = 0; r < rows; ++r) {
            for (int c = 0; c < cols; ++c) {
                file >> W(r, c);
            }
        }
        weights.push_back(W);
    }

    // Chargement des biais
    for (size_t i = 1; i < numLayers; ++i) {
        int size;
        file >> size;
        Eigen::VectorXf b(size);
        for (int j = 0; j < size; ++j) {
            file >> b(j);
        }
        biases.push_back(b);
    }

    // Réallocation des neurones et deltas selon les tailles
    neurons.resize(numLayers);
    deltas.resize(numLayers);
    for (size_t i = 0; i < numLayers; ++i) {
        neurons[i] = Eigen::VectorXf::Zero(layers[i]);
        deltas[i] = Eigen::VectorXf::Zero(layers[i]);
    }

    file.close();
    return true;
}
