from optparse import OptionParser
from tqdm import tqdm
import pandas as pd
import numpy as np
import scipy.stats
import h5py

import utils
import model


def test_generator(loaded, columns, pairs_idx):
    """
    :param loaded: features numpy array
    :param columns: columns in train set
    :param pairs_idx: target pairs indices to predict
    :yield: test dataframe and dict of targets
    """

    for start in tqdm(range(0, len(loaded) - utils.TICKS_IN_FIVE_MINUTES - utils.PREDICT_DELAY_TICKS, utils.GAP)):
        end = start + utils.TICKS_IN_FIVE_MINUTES
        test = pd.DataFrame(loaded[start:end], columns=columns, copy=True)
        target = loaded[end-1 + utils.PREDICT_DELAY_TICKS, pairs_idx] / loaded[end-1, pairs_idx] - 1
        yield test, target


def apply_on_test(model, file_name):
    """
    :param model: object of class Model
    :param file_name: hdf5 file file name to test on

    :return y_true: float array of true values, shape == test_points x len(pairs)
    :return y_pred: float array of predicted values, shape == test_points x len(pairs)
    :return pairs: list of string, pairs to predict
    """
    assert len(model.pairs) > 0, "No pairs in model.pairs ;("
    assert utils.MAIN_PAIR in model.pairs, "No main pair LTC_USDT in model.pairs ;("

    f = h5py.File(file_name, "r")

    # load hdf5 files in memory
    exch, pair, action, _ = utils.TARGET_COLUMNS[0].split("/")
    loaded = np.zeros((len(f["body"][exch][pair][action]), 0))
    columns = []
    header = [name for name in f["header"]]
    for exch, symbol, side, candles in tqdm(utils.candles_loop(f["body"])):
        loaded = np.column_stack((loaded, candles))
        columns += [exch + "/" + symbol + "/" + side + "/" + x for x in header]

    pairs_idx = []
    for pair in model.pairs:
        for i, column in enumerate(columns):
            inferred_column = "hitbtc/" + pair + "/buy/max_price"
            if inferred_column == column:
                pairs_idx += [i]

    y_true, y_pred = [], []

    for test, target in test_generator(loaded, columns, pairs_idx):
        y_true += [target]
        y_pred += [[pred for pair, pred in sorted(model.predict(test).items(), key=lambda x: x[0])]]

    return np.array(y_true), np.array(y_pred), model.pairs


def calc_score(y_true, y_pred, pairs):
    """
    Scores model

    :param y_true: float array of true values, shape == test_points x len(pairs)
    :param y_pred: float array of predicted values, shape == test_points x len(pairs)
    :param pairs: list of string, pairs to predict
    """

    for i, pair in enumerate(pairs):
        true_one_pair = y_true[:, i]
        pred_one_pair = y_pred[:, i]
        score, _ = scipy.stats.spearmanr(true_one_pair, pred_one_pair)

        print("pair: {0: <8}| score: {1:+.3f}".format(pair, score))


def make_submission(y_pred, pairs, output):
    target_index = pairs.index(utils.MAIN_PAIR)
    pd.DataFrame({"idx": range(len(y_pred)), "target": y_pred[:, target_index]}).to_csv(output, index=False)


if __name__ == "__main__":
    parser = OptionParser()
    parser.add_option("--input", dest="filename", help="input to test")
    parser.add_option("--output", dest="output", help="output for contest")

    (options, args) = parser.parse_args()

    model_ = model.Model()  # your model for testing

    y_true, y_pred, pairs = apply_on_test(model_, options.filename)
    calc_score(y_true, y_pred, pairs)
    make_submission(y_pred, pairs, options.output)
