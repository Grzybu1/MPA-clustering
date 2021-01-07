import pandas as pd
import matplotlib.pyplot as plt
from sklearn import preprocessing
import sys
# ('Celinski Harabasz', 'Gini Index', 'Silhoutte Index', 'Davis Bouldin Index', 'Rand Index')
datasets = ['cevical_cancer', 'divorce', 'glass', 'iris', 'wdbc', 'wine', 'fertility_Diagnosis', 'forests', 'parkinsons']
if __name__ == '__main__':
    df = pd.read_csv('../outputFiles/' + sys.argv[1],
                     index_col=None, comment="#", names=('MPA', 'Kmeans', 'Cmeans'))
    print(f'Średnia:\n{df.mean()}\nodchylenie standardowe:\n{df.std()}\n')
