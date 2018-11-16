import utils
import numpy as np
import pickle as pk

from utils import TICKS_IN_FIVE_MINUTES, PAIR_NAMES, TARGET_COLUMNS
DIVIDER = 7
RELEVANT_COLUMNS = ['bitfinex2/BTC_USD/buy/max_price', 'bitfinex2/BTC_USD/buy/amount', 'bitfinex2/BTC_USD/sell/max_price', 'bitfinex2/BTC_USD/sell/amount', 'bittrex/BCH_BTC/buy/max_price', 'bittrex/BCH_BTC/buy/amount', 'bittrex/BCH_BTC/sell/max_price', 'bittrex/BCH_BTC/sell/amount', 'bittrex/BTC_USDT/buy/max_price', 'bittrex/BTC_USDT/buy/amount', 'bittrex/BTC_USDT/sell/max_price', 'bittrex/BTC_USDT/sell/amount', 'bittrex/ETH_BTC/buy/max_price', 'bittrex/ETH_BTC/buy/amount', 'bittrex/ETH_BTC/sell/max_price', 'bittrex/ETH_BTC/sell/amount', 'bittrex/LTC_BTC/buy/max_price', 'bittrex/LTC_BTC/buy/amount', 'bittrex/LTC_BTC/sell/max_price', 'bittrex/LTC_BTC/sell/amount', 'bittrex/XRP_BTC/buy/max_price', 'bittrex/XRP_BTC/buy/amount', 'bittrex/XRP_BTC/sell/max_price', 'bittrex/XRP_BTC/sell/amount', 'cex/BCH_BTC/buy/max_price', 'cex/BCH_BTC/buy/amount', 'cex/BCH_BTC/sell/max_price', 'cex/BCH_BTC/sell/amount', 'cex/BTC_USD/buy/max_price', 'cex/BTC_USD/buy/amount', 'cex/BTC_USD/sell/max_price', 'cex/BTC_USD/sell/amount', 'cex/ETH_BTC/buy/max_price', 'cex/ETH_BTC/buy/amount', 'cex/ETH_BTC/sell/max_price', 'cex/ETH_BTC/sell/amount', 'cex/XRP_BTC/buy/max_price', 'cex/XRP_BTC/buy/amount', 'cex/XRP_BTC/sell/max_price', 'cex/XRP_BTC/sell/amount', 'gdax/BTC_USD/buy/max_price', 'gdax/BTC_USD/buy/amount', 'gdax/BTC_USD/sell/max_price', 'gdax/BTC_USD/sell/amount', 'gdax/ETH_BTC/buy/max_price', 'gdax/ETH_BTC/buy/amount', 'gdax/ETH_BTC/sell/max_price', 'gdax/ETH_BTC/sell/amount', 'gdax/LTC_BTC/buy/max_price', 'gdax/LTC_BTC/buy/amount', 'gdax/LTC_BTC/sell/max_price', 'gdax/LTC_BTC/sell/amount', 'hitbtc/BCH_BTC/buy/max_price', 'hitbtc/BCH_BTC/buy/amount', 'hitbtc/BCH_BTC/sell/max_price', 'hitbtc/BCH_BTC/sell/amount', 'hitbtc/BTC_USDT/buy/max_price', 'hitbtc/BTC_USDT/buy/amount', 'hitbtc/BTC_USDT/sell/max_price', 'hitbtc/BTC_USDT/sell/amount', 'hitbtc/DASH_BTC/buy/max_price', 'hitbtc/DASH_BTC/buy/amount', 'hitbtc/DASH_BTC/sell/max_price', 'hitbtc/DASH_BTC/sell/amount', 'hitbtc/ETH_BTC/buy/max_price', 'hitbtc/ETH_BTC/buy/amount', 'hitbtc/ETH_BTC/sell/max_price', 'hitbtc/ETH_BTC/sell/amount', 'hitbtc/LTC_BTC/buy/max_price', 'hitbtc/LTC_BTC/buy/amount', 'hitbtc/LTC_BTC/sell/max_price', 'hitbtc/LTC_BTC/sell/amount', 'hitbtc/XMR_BTC/buy/max_price', 'hitbtc/XMR_BTC/buy/amount', 'hitbtc/XMR_BTC/sell/max_price', 'hitbtc/XMR_BTC/sell/amount', 'hitbtc/XRP_BTC/buy/max_price', 'hitbtc/XRP_BTC/buy/amount', 'hitbtc/XRP_BTC/sell/max_price', 'hitbtc/XRP_BTC/sell/amount', 'hitbtc/ZEC_BTC/buy/max_price', 'hitbtc/ZEC_BTC/buy/amount', 'hitbtc/ZEC_BTC/sell/max_price', 'hitbtc/ZEC_BTC/sell/amount', 'poloniex/BCH_BTC/buy/max_price', 'poloniex/BCH_BTC/buy/amount', 'poloniex/BCH_BTC/sell/max_price', 'poloniex/BCH_BTC/sell/amount', 'poloniex/BTC_USDT/buy/max_price', 'poloniex/BTC_USDT/buy/amount', 'poloniex/BTC_USDT/sell/max_price', 'poloniex/BTC_USDT/sell/amount', 'poloniex/ETC_BTC/buy/max_price', 'poloniex/ETC_BTC/buy/amount', 'poloniex/ETC_BTC/sell/max_price', 'poloniex/ETC_BTC/sell/amount', 'poloniex/ETH_BTC/buy/max_price', 'poloniex/ETH_BTC/buy/amount', 'poloniex/ETH_BTC/sell/max_price', 'poloniex/ETH_BTC/sell/amount', 'poloniex/LTC_BTC/buy/max_price', 'poloniex/LTC_BTC/buy/amount', 'poloniex/LTC_BTC/sell/max_price', 'poloniex/LTC_BTC/sell/amount', 'poloniex/XRP_BTC/buy/max_price', 'poloniex/XRP_BTC/buy/amount', 'poloniex/XRP_BTC/sell/max_price', 'poloniex/XRP_BTC/sell/amount', 'quoine/BTC_JPY/buy/max_price', 'quoine/BTC_JPY/buy/amount', 'quoine/BTC_JPY/sell/max_price', 'quoine/BTC_JPY/sell/amount', 'quoine/BTC_USD/buy/max_price', 'quoine/BTC_USD/buy/amount', 'quoine/BTC_USD/sell/max_price', 'quoine/BTC_USD/sell/amount', 'quoine/ETH_BTC/buy/max_price', 'quoine/ETH_BTC/buy/amount', 'quoine/ETH_BTC/sell/max_price', 'quoine/ETH_BTC/sell/amount', 'quoine/QASH_BTC/buy/max_price', 'quoine/QASH_BTC/buy/amount', 'quoine/QASH_BTC/sell/max_price', 'quoine/QASH_BTC/sell/amount', 'wex/BCH_BTC/buy/max_price', 'wex/BCH_BTC/buy/amount', 'wex/BCH_BTC/sell/max_price', 'wex/BCH_BTC/sell/amount', 'wex/BTC_EUR/buy/max_price', 'wex/BTC_EUR/buy/amount', 'wex/BTC_EUR/sell/max_price', 'wex/BTC_EUR/sell/amount', 'wex/BTC_USD/buy/max_price', 'wex/BTC_USD/buy/amount', 'wex/BTC_USD/sell/max_price', 'wex/BTC_USD/sell/amount', 'wex/DASH_BTC/buy/max_price', 'wex/DASH_BTC/buy/amount', 'wex/DASH_BTC/sell/max_price', 'wex/DASH_BTC/sell/amount', 'wex/ETH_BTC/buy/max_price', 'wex/ETH_BTC/buy/amount', 'wex/ETH_BTC/sell/max_price', 'wex/ETH_BTC/sell/amount', 'wex/LTC_BTC/buy/max_price', 'wex/LTC_BTC/buy/amount', 'wex/LTC_BTC/sell/max_price', 'wex/LTC_BTC/sell/amount', 'wex/NMC_BTC/buy/max_price', 'wex/NMC_BTC/buy/amount', 'wex/NMC_BTC/sell/max_price', 'wex/NMC_BTC/sell/amount', 'wex/PPC_BTC/buy/max_price', 'wex/PPC_BTC/buy/amount', 'wex/PPC_BTC/sell/max_price', 'wex/PPC_BTC/sell/amount', 'wex/ZEC_BTC/buy/max_price', 'wex/ZEC_BTC/buy/amount', 'wex/ZEC_BTC/sell/max_price', 'wex/ZEC_BTC/sell/amount']

class Model:
    def __init__(self):
        """
        :param pairs:
        """
        self.pairs = PAIR_NAMES
        self.relevant_columns = RELEVANT_COLUMNS
        
        self.regr = []
        for pair in self.pairs:
            with open(pair + ".pkl", "rb+") as f:
                self.regr.append(pk.load(f))
    
    def __extract_features(self, test):
        vals = test[self.relevant_columns].values
        
        X_diff = vals[1:] - vals[:-1]
    
        X_rel = vals[1:] / vals[:-1]
        X_rel[np.isnan(X_rel)] = 1
        X_rel[np.isinf(X_rel)] = 1
        
        end_part = TICKS_IN_FIVE_MINUTES * (DIVIDER - 1) // DIVIDER
        
        pooling = tuple(pool_func(array, axis=0) for pool_func in (np.min, np.max, np.mean, np.sum)
                                                 for array in (X_diff,
                                                               X_rel,
                                                               X_diff[end_part:],
                                                               X_rel[end_part:]
                                                              ))
        
        simple = (X_diff[::DIVIDER].flatten(), 
                  X_rel[::DIVIDER].flatten(),
                  X_diff[end_part:].flatten(),
                  X_rel[end_part:].flatten()
                 )
        
        return np.hstack(simple + pooling)
    
    def __predict_pair(self, test, pair):
        """

        :param test: dataframe to predict on
        :return: predicted ratio
        """
        i = self.pairs.index(pair)
        if i is None:
            return 0.
        
        features = self.__extract_features(test)
        
        return self.regr[i].predict([features])[0]
    
    def predict(self, test):
        """
        :param test: dataframe to predict on
        :return: dict: pair -> predicted ratio
        """
        output = {}
    
        for pair in self.pairs:
            output[pair] = self.__predict_pair(test, pair)
        
        return output
