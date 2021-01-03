import pandas as pd
import matplotlib.pyplot as plt
from sklearn import preprocessing
import sys

if __name__ == '__main__':
    df = pd.read_csv('../brownian.csv',
                     index_col=0, names=['brownian'])
    # df.drop(labels='Celinski Harabasz', axis=1).plot()
    # df['Celinski Harabasz'].plot(secondary_y=True)
    print(df)
    df.plot.hist(by=1, bins=100)
    plt.show()
