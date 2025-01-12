import pandas as pd
import matplotlib.pyplot as plt

# Configure plot function
def configure_plot(ax, title, xlabel, ylabel, fontsize, spine="left", lw=None):
    d = {'size': fontsize, 'weight': 'bold'}
    pad = 30
    if lw is None:
        lw = 3

    if spine:
        ax.spines[spine].set_linewidth(lw)
        ax.spines['bottom'].set_linewidth(lw)

    ax.set_title(title, fontdict=d, y=1.1)
    ax.set_xlabel(xlabel, fontdict=d, labelpad=pad)
    ax.set_ylabel(ylabel, fontdict=d, labelpad=pad)
    plt.setp(ax.get_yticklabels(), fontsize=fontsize - 2, fontweight="bold")
    plt.setp(ax.get_xticklabels(), fontsize=fontsize - 2, fontweight="bold")
    ax.yaxis.set_major_locator(plt.MaxNLocator(6))
    ax.xaxis.set_major_locator(plt.MaxNLocator(6))
    ax.tick_params(which='major', axis='x', direction='out', length=4, width=lw, pad=15)
    ax.tick_params(which='major', axis='y', direction='out', length=4, width=lw, pad=15)
    if spine:
        ax.spines['top'].set_visible(False)
        ax.spines["right" if spine == "left" else "left"].set_visible(False)

# Read lines 37 to 72 from the files
data_unoptimized = pd.read_csv('../../logs/un-optimized/mean_unopt.txt', sep='\t', header=None, skiprows=36, nrows=36)
data_optimized = pd.read_csv('../../logs/optimized/mean_optimized.txt', sep='\t', header=None, skiprows=36, nrows=36)

# Define column names for easier access
data_unoptimized.columns = ['alpha', 'L', 'R', 'index_time', 'memory', 'qps_f', 'qps_u', 'algorithm', 'size', 'recall_f', 'recall_u']
data_optimized.columns = ['alpha', 'L', 'R', 'index_time', 'memory', 'qps_f', 'qps_u', 'algorithm', 'size', 'recall_f', 'recall_u']

# Create a figure with three subplots in a row
fig, (ax1, ax2, ax3) = plt.subplots(1, 3, figsize=(20, 6))

# Plot 1: Fixed alpha = 1.0, L = 100
subset_unoptimized = data_unoptimized[(data_unoptimized['alpha'] == 1.0) & (data_unoptimized['L'] == 100)]
subset_optimized = data_optimized[(data_optimized['alpha'] == 1.0) & (data_optimized['L'] == 100)]
ax1.plot(subset_unoptimized['R'], subset_unoptimized['index_time'], label='Initial Empty Graph')
ax1.plot(subset_optimized['R'], subset_optimized['index_time'], label='R Regular')
configure_plot(ax1, '(Alpha=1.0, L=100)', 'R', 'Index Time (s)', fontsize=18)
ax1.legend(fontsize=18, frameon=False, loc='upper left')

# Plot 2: Fixed alpha = 1.1, L = 135
subset_unoptimized = data_unoptimized[(data_unoptimized['alpha'] == 1.1) & (data_unoptimized['L'] == 135)]
subset_optimized = data_optimized[(data_optimized['alpha'] == 1.1) & (data_optimized['L'] == 135)]
ax2.plot(subset_unoptimized['R'], subset_unoptimized['index_time'], label='Initial Empty Graph')
ax2.plot(subset_optimized['R'], subset_optimized['index_time'], label='R Regular')
configure_plot(ax2, '(Alpha=1.1, L=135)', 'R', 'Index Time (s)', fontsize=18)
ax2.legend(fontsize=18, frameon=False, loc='upper left')

# Plot 3: Fixed alpha = 1.2, L = 170
subset_unoptimized = data_unoptimized[(data_unoptimized['alpha'] == 1.2) & (data_unoptimized['L'] == 170)]
subset_optimized = data_optimized[(data_optimized['alpha'] == 1.2) & (data_optimized['L'] == 170)]
ax3.plot(subset_unoptimized['R'], subset_unoptimized['index_time'], label='Initial Empty Graph')
ax3.plot(subset_optimized['R'], subset_optimized['index_time'], label='R Regular')
configure_plot(ax3, '(Alpha=1.2, L=170)', 'R', 'Index Time (s)', fontsize=18)
ax3.legend(fontsize=18, frameon=False, loc='upper left')

plt.tight_layout()
plt.savefig('optimized_filter.png')