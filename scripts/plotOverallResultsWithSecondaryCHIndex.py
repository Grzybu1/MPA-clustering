import pandas as pd
import matplotlib.pyplot as plt
from sklearn import preprocessing
import sys

if __name__ == '__main__':
    df = pd.read_csv('../outputFiles/' + sys.argv[1],
                     index_col=0, names=('Celinski Harabasz', 'Gini Index',
                                         'Silhoutte Index', 'Davis Bouldin Index', 'Rand Index'))
    df.drop(labels='Celinski Harabasz', axis=1).plot()
    df['Celinski Harabasz'].plot(secondary_y=True)
    plt.show()

    # x = df.values.astype(float)
    # min_max_scaler = preprocessing.MinMaxScaler()
    # x_scaled = min_max_scaler.fit_transform(x)
    # df_normalized = pd.DataFrame(x_scaled)
    # df_normalized.columns = ('Celinski Harabasz', 'Gini Index',
    #                          'Silhoutte Index', 'Davis Bouldin Index', 'Rand Index')
    # # df_normalized.rows = range(0, 9)
    # print(df_normalized)
    # df_normalized.plot()
    # plt.show()
