TICKS_IN_FIVE_MINUTES = 5 * 60 // 10
PREDICT_DELAY_TICKS = 30
RATIO_THRESHOLD = 0.005
GAP = 2

MAIN_PAIR = "BTC_USDT"
PAIR_NAMES = ['BCH_BTC', 'BTC_USDT', 'DASH_BTC', 'ETH_BTC', 'LTC_BTC', 'XMR_BTC', 'XRP_BTC', 'ZEC_BTC']

TARGET_COLUMNS = sorted(["hitbtc/" + pair + "/buy/max_price" for pair in PAIR_NAMES])


def candles_loop(dict_candles):
    """
    Loop over all candles arrays in hdf5 file['body']
    yileds exch_name, symbol, side and candles_array

    :param dict_candles:
    :yield: exch_name, symbol, side and candles_array
    """

    for exch, sym_dict in dict_candles.items():
        for symbol, side_candles in sym_dict.items():
            for side, candles in side_candles.items():
                yield exch, symbol, side, candles
