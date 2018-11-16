#include "classifier.h"

ClassifierParams::ClassifierParams(double in_C) :
    bias(-1),
    solver_type(L2R_L2LOSS_SVC_DUAL),
    C(in_C),
    eps(1e-4),
    nr_weight(0),
    weight_label(nullptr),
    weight(nullptr)
{
}

Classifier::Classifier(const ClassifierParams &in_params) :
    params(in_params),
    model(nullptr)
{
}

// Save model to file
void
Classifier::save(const std::string &model_file) const {
    if (this->model.use_count() == 0) {
        throw std::string("Empty model");
    }
    save_model(model_file.c_str(), this->model.get());
}

// Load model from file
void
Classifier::load(const std::string &model_file) {
    this->model = std::shared_ptr<struct model>(load_model(model_file.c_str()));
}

void
Classifier::train(const TFeatures &features, const TLabels &labels) {
    // Number of samples and features must be nonzero
    ssize_t number_of_samples = features.size();
    if (number_of_samples == 0 || features[0].size() == 0) {
        throw std::string("Number of samples and features must be nonzero");
    }
    ssize_t number_of_features = features[0].size();

    if (features.size() != labels.size()) {
        throw std::string("Feature list and label list hasn't equal size");
    }

    // Description of one problem
    struct problem prob;
    prob.l = number_of_samples;
    prob.bias = -1;
    prob.n = number_of_features;
    prob.y = new double[number_of_samples];
    prob.x = new struct feature_node*[number_of_samples];

    // Fill struct problem
    for (ssize_t sample_idx = 0; sample_idx < number_of_samples; ++sample_idx)
    {
        prob.x[sample_idx] = new struct feature_node[number_of_features + 1];
        for (unsigned int feature_idx = 0; feature_idx < number_of_features; feature_idx++)
        {
            prob.x[sample_idx][feature_idx].index = feature_idx + 1;
            prob.x[sample_idx][feature_idx].value = features[sample_idx][feature_idx];
        }
        prob.x[sample_idx][number_of_features].index = -1;
        prob.y[sample_idx] = labels[sample_idx];
    }

    // Fill param structure by values from 'this->params'
    struct parameter param;
    param.solver_type = this->params.solver_type;
    param.C = this->params.C;      // try to vary it
    param.eps = this->params.eps;
    param.nr_weight = this->params.nr_weight;
    param.weight_label = this->params.weight_label;
    param.weight = this->params.weight;

    this->model = std::shared_ptr<struct model>(::train(&prob, &param));

    // Clear param structure
    destroy_param(&param);
    // clear problem structure
    delete[] prob.y;
    for (ssize_t sample_idx = 0; sample_idx < number_of_samples; ++sample_idx)
        delete[] prob.x[sample_idx];
    delete[] prob.x;
}

TLabels
Classifier::predict(const TFeatures &features) const {
    TLabels labels;
    ssize_t number_of_samples = features.size();
    if (number_of_samples == 0 || features[0].size() == 0) {
        throw std::string("Number of samples and features must be nonzero");
    }
    ssize_t number_of_features = features[0].size();

    // Fill struct problem
    struct feature_node* x = new struct feature_node[number_of_features + 1];
    for (ssize_t sample_idx = 0; sample_idx < number_of_samples; ++sample_idx) {
        for (ssize_t feature_idx = 0; feature_idx < number_of_features; ++feature_idx) {
            x[feature_idx].index = feature_idx + 1;
            x[feature_idx].value = features[sample_idx][feature_idx];
        }
        x[number_of_features].index = -1;
        labels.push_back(::predict(this->model.get(), x));
    }
    delete[] x;

    return labels;
}
