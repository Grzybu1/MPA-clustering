import pandas as pd
import matplotlib.pyplot as plt
from sklearn import preprocessing
import sys

if __name__ == '__main__':
    df = pd.read_csv('../levy.csv',
                     index_col=0, names=['step_length'])
    # df.drop(labels='Celinski Harabasz', axis=1).plot()
    # df['Celinski Harabasz'].plot(secondary_y=True)
    print(df)
    df.plot()
    plt.show()
