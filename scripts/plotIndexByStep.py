import pandas as pd
import matplotlib.pyplot as plt
from sklearn import preprocessing
import sys
datasets = [
    # 'divorce',
    'iris',
    # 'wdbc',
    # 'wine',
    # 'forests',
]
indexAsFitting = [
    'CHIndex',
    'DBIndex',
    'GIndex',
    'SIndex'
]
savedIndex = [
    'CelinskiHarabaszIndex',
    'GiniIndex',
    'SilhoutteIndex',
    'DaviesBouldinIndex',
    'RandIndex'
]

if __name__ == '__main__':
    for dataset in datasets:
        for index in indexAsFitting:
            result_df = pd.DataFrame()
            for saved in savedIndex:
                df = pd.read_csv('../outputFiles/hopefullyFinalData/indexesByStep/' + dataset + '_' + index + '0' + saved + '.csv',
                                 index_col=0, comment="#", names=('step', 'index'))
                result_df[saved] = df['index']
            ax = plt.subplots()
            ax = result_df.drop('CelinskiHarabaszIndex',
                                axis=1).plot(logy=True)
            ax.set_ylabel('Index value')

            ax2 = ax.twinx()
            ax2 = result_df['CelinskiHarabaszIndex'].plot(
                secondary_y=True, color='m', logy=True)
            ax2.set_ylabel('CHI value')

            lines = ax.get_lines() + ax2.get_lines()
            labels = [l.get_label() for l in lines]
            lab = []
            for l in labels:
                if l == 'CelinskiHarabaszIndex':
                    lab.append('CalinskiHarabaszIndex')
                else:
                    lab.append(l)
            ax.legend(lines, lab, loc='lower left')
            plt.axvline(x=400, color='black', linestyle='--')
            plt.axvline(x=800, color='black', linestyle='--')
            plt.show()
