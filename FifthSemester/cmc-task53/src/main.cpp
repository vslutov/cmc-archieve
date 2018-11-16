#include "main.h"

#include "argvparser.h"
#include "classifier.h"
#include "hog.h"
#include "image.h"
#include "linear.h"
#include "EasyBMP.h"

#include <cassert>
#include <cmath>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

// Load list of files and its labels from 'data_file' and
// stores it in 'file_list'
static TFileList
loadFileList(const std::string &data_file) {
    TFileList result;
    std::ifstream stream(data_file.c_str());

    std::string filename;
    ssize_t label;

    ssize_t char_idx = data_file.size() - 1;
    for (; char_idx >= 0; --char_idx)
        if (data_file[char_idx] == '/' || data_file[char_idx] == '\\')
            break;
    std::string data_path = data_file.substr(0,char_idx+1);

    while(!stream.eof() && !stream.fail()) {
        stream >> filename >> label;
        if (filename.size()) {
            result.push_back(std::make_pair(data_path + filename, label));
        }
    }

    stream.close();
    return result;
}

// Load images by list of files 'file_list' and store them in 'data_set'
static TDataSet
loadImages(const TFileList &file_list) {
    TDataSet result;
    for (const auto file : file_list) {
        auto image = load_image(file.first);
        result.push_back(std::make_pair(image, file.second));
    }
    return result;
}

// Save result of prediction to file
static void
savePredictions(const TFileList &file_list,
                const TLabels &labels,
                const std::string &prediction_file) {
    if (file_list.size() != labels.size()) {
        throw std::string("File list and label list hasn't equal size");
    }

    ssize_t image_count = file_list.size();
    std::ofstream stream(prediction_file.c_str());

    // Write file names and labels to stream
    for (ssize_t image_idx = 0; image_idx < image_count; ++image_idx) {
        stream << file_list[image_idx].first << " " << labels[image_idx]
               << std::endl;
    }
    stream.close();
}

// Exatract features from dataset.
static TFeatures
extractFeatures(const TDataSet &data_set) {
    TFeatures result;

    for (const auto &data : data_set) {
        // PLACE YOUR CODE HERE
        // Remove this sample code and place your feature extraction code here
        auto feature = hog_pyramid(data.first);
        result.push_back(feature);
        // End of sample code
    }

    return result;
}

// Exatract features from dataset.
static TLabels
extractLabels(const TDataSet &data_set) {
    TLabels result;
    for (const auto &data : data_set) {
        result.push_back(data.second);
    }
    return result;
}

// Train SVM classifier using data from 'data_file' and save trained model
// to 'model_file'
static void
trainClassifier(const std::string &data_file, const std::string &model_file) {
    // Load list of image file names and its labels
    auto file_list = loadFileList(data_file);
    auto data_set = loadImages(file_list);
    auto features = extractFeatures(data_set);
    auto labels = extractLabels(data_set);

    // PLACE YOUR CODE HERE
    // You can change parameters of classifier here
    auto params = ClassifierParams(0.01);
    Classifier classifier(params);
    classifier.train(features, labels);
    classifier.save(model_file);
}

// Predict data from 'data_file' using model from 'model_file' and
// save predictions to 'prediction_file'
static void
predictData(const std::string &data_file,
            const std::string &model_file,
            const std::string &prediction_file) {
    // Load list of image file names and its labels
    auto file_list = loadFileList(data_file);
    auto data_set = loadImages(file_list);
    auto features = extractFeatures(data_set);

    auto classifier = Classifier(ClassifierParams());
    classifier.load(model_file);

    auto labels = classifier.predict(features);

    savePredictions(file_list, labels, prediction_file);
}

int
main(int argc, char** argv) {
    // Command line options parser
    CommandLineProcessing::ArgvParser cmd;
    // Description of program
    cmd.setIntroductoryDescription("Machine graphics course, task 2. CMC MSU, 2014.");
    // Add help option
    cmd.setHelpOption("h", "help", "Print this help message");
    // Add other options
    cmd.defineOption("data_set", "File with dataset",
        CommandLineProcessing::ArgvParser::OptionRequiresValue |
        CommandLineProcessing::ArgvParser::OptionRequired);
    cmd.defineOption("model", "Path to file to save or load model",
        CommandLineProcessing::ArgvParser::OptionRequiresValue |
        CommandLineProcessing::ArgvParser::OptionRequired);
    cmd.defineOption("predicted_labels", "Path to file to save prediction results",
        CommandLineProcessing::ArgvParser::OptionRequiresValue);
    cmd.defineOption("train", "Train classifier");
    cmd.defineOption("predict", "Predict dataset");

    // Add options aliases
    cmd.defineOptionAlternative("data_set", "d");
    cmd.defineOptionAlternative("model", "m");
    cmd.defineOptionAlternative("predicted_labels", "l");
    cmd.defineOptionAlternative("train", "t");
    cmd.defineOptionAlternative("predict", "p");

    // Parse options
    int result = cmd.parse(argc, argv);

    // Check for errors or help option
    if (result) {
        std::cout << cmd.parseErrorDescription(result) << std::endl;
        return result;
    }

    // Get values
    std::string data_file = cmd.optionValue("data_set");
    std::string model_file = cmd.optionValue("model");
    bool train = cmd.foundOption("train");
    bool predict = cmd.foundOption("predict");

    // If we need to train classifier
    if (train)
        trainClassifier(data_file, model_file);
        // If we need to predict data
    if (predict) {
        // You must declare file to save images
        if (!cmd.foundOption("predicted_labels")) {
            std::cerr << "Error! Option --predicted_labels not found!" << std::endl;
            return 1;
        }
        // File to save predictions
        std::string prediction_file = cmd.optionValue("predicted_labels");
        predictData(data_file, model_file, prediction_file);
    }

    return 0;
}
