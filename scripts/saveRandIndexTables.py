import pandas as pd

datasets = [
    # 'cevical_cancer',
    'divorce',
    # 'glass',
    'iris',
    'wdbc',
    'wine',
    # 'fertility_Diagnosis',
    'forests',
    # 'parkinsons'
]
indexAsFitting = ['CHIndex', 'DBIndex', 'GIndex', 'SIndex']
methods = ['MPA', 'Kmeans', 'Cmeans']
if __name__ == '__main__':
    for index in indexAsFitting:
        resultTable = []
        for dataset in datasets:
            row = []
            df = pd.read_csv('../outputFiles/hopefullyFinalData/overallResults/' + dataset + "_" + index + 'RandIndex' + '.csv',
                             index_col=None, comment="#",
                             names=('MPA', 'Kmeans', 'Cmeans'))
            for method in methods:
                row.append(round(df[method].mean(), 5))
                row.append(round(df[method].std(), 5))

            hierarchical_df = pd.read_csv(
                '../outputFiles/hopefullyFinalData/hierarchicalResults/' + dataset + '.csv',
                index_col=None, comment='#',
                names=['rand_index'])
            row.append(round(hierarchical_df['rand_index'].mean(), 5))

            dbscan_df = pd.read_csv(
                '../outputFiles/hopefullyFinalData/dbscanResults/' + dataset + '_' + index + '.csv',
                index_col=None, comment='#',
                names=['clusters', 'noises', 'rand_index'])
            row.append(round(dbscan_df['rand_index'].mean(), 5))
            row.append(round(dbscan_df['rand_index'].std(), 5))

            resultTable.append(row)
        resultDf = pd.DataFrame(data=resultTable, index=datasets,
                                columns=(
                                    'MPA_mean',
                                    'MPA_std',
                                    'Kmeans_mean',
                                    'Kmeans_std',
                                    'Cmeans_mean',
                                    'Cmeans_std',
                                    'Hierarchical_mean',
                                    'Dbscan_mean',
                                    'Dbscan_std'
                                )
                                )
        resultDf.to_csv('../aggregatedData/' + index + "_results" + '.csv')
    # print(f'Średnia:\n{df.mean()}\nodchylenie standardowe:\n{df.std()}\n')
