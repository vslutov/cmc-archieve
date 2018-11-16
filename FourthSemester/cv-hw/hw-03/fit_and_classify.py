import sklearn.svm

def fit_and_classify(train_features, train_labels, test_features):
    return sklearn.svm.LinearSVC(C=0.5).fit(train_features, train_labels).predict(test_features)