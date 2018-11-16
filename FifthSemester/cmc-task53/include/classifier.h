#pragma once

#include "image.h"
#include "linear.h"

#include <memory>
#include <string>
#include <vector>
#include <utility>

/**
 * @file
 * @author unknown
 * @brief Define Classifier wrap around liblinear.
 *
 * @section description
 *
 * Rewrited by Lutov V. S. <vslutov@yandex.ru>
 *
 * Usage example:
 *
 *     // Train model
 *     auto classifier = Classifier(ClassifierParams());
 *     classifier.train(features, labels);
 *     classifier.save("model.txt");
 *
 *     ...
 *
 *     // Load model and predict
 *     auto classifier = Classifier(ClassifierParams());
 *     classifier.load("model.txt");
 *     auto labels = classifier.predict(features);
 */

/**
 * Parameters for classifier training.
 *
 * Read more about it in liblinear documentation.
 */
struct ClassifierParams {
    double bias;
    int solver_type;
    double C;
    double eps;
    int nr_weight;
    int* weight_label;
    double* weight;

    ClassifierParams(double C = 0.1);
};

/**
 * Model of classifier to be trained.
 *
 * Encapsulates 'struct model' from liblinear and allow to train it.
 */
struct Classifier {
private:
    ClassifierParams params;
    std::shared_ptr<struct model> model;

public:
    /**
     * Basic constructor.
     */
    Classifier(const ClassifierParams &params);

    /**
     * Save model to file.
     */
    void
    save(const std::string &filename) const;

    /**
     * Load model from file.
     */
    void
    load(const std::string &filename);

    /**
     * Train model for feature and label set.
     */
    void
    train(const TFeatures &feature_set, const TLabels &label_set);

    /**
     * Predict labels for feature set.
     */
    TLabels
    predict(const TFeatures &feature_set) const;
};
