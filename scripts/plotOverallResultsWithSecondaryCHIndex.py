import pandas as pd
import matplotlib.pyplot as plt
from sklearn import preprocessing
import sys

if __name__ == '__main__':
    df = pd.read_csv('../outputFiles/' + sys.argv[1],
                     index_col=0, comment="#", names=('Celinski Harabasz', 'Gini Index',
                                                      'Silhoutte Index', 'Davis Bouldin Index', 'Rand Index'))
    df.drop(labels='Celinski Harabasz', axis=1).plot()
    df['Celinski Harabasz'].plot(secondary_y=True)
    plt.show()
