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

# Read the first 36 lines of the files
data_unoptimized = pd.read_csv('../../logs/un-optimized/mean_unopt.txt', sep='\t', header=None, nrows=36)
data_optimized = pd.read_csv('../../logs/optimized/mean_optimized.txt', sep='\t', header=None, nrows=36)

# Define column names for easier access
data_unoptimized.columns = ['alpha', 'L', 'R', 'index_time', 'memory', 'qps', 'algorithm', 'size', 'recall']
data_optimized.columns = ['alpha', 'L', 'R', 'index_time', 'memory', 'qps', 'algorithm', 'size', 'recall']

# Create a figure with three subplots in a row
fig, (ax1, ax2, ax3) = plt.subplots(1, 3, figsize=(20, 6))

# Plot 1: Fixed L = 135, R = 64
subset_unoptimized = data_unoptimized[(data_unoptimized['L'] == 135) & (data_unoptimized['R'] == 64)]
subset_optimized = data_optimized[(data_optimized['L'] == 135) & (data_optimized['R'] == 64)]
ax1.plot(subset_unoptimized['alpha'], subset_unoptimized['index_time'], label='Random Medoid')
ax1.plot(subset_optimized['alpha'], subset_optimized['index_time'], label='True Medoid')
configure_plot(ax1, '(L=135, R=64)', 'Alpha', 'Index Time (s)', fontsize=18)
ax1.legend(fontsize=18, frameon=False)

# Plot 2: Fixed alpha = 1.1, L = 135
subset_unoptimized = data_unoptimized[(data_unoptimized['alpha'] == 1.1) & (data_unoptimized['L'] == 135)]
subset_optimized = data_optimized[(data_optimized['alpha'] == 1.1) & (data_optimized['L'] == 135)]
ax2.plot(subset_unoptimized['R'], subset_unoptimized['index_time'], label='Random Medoid')
ax2.plot(subset_optimized['R'], subset_optimized['index_time'], label='True Medoid')
configure_plot(ax2, '(Alpha=1.1, L=135)', 'R', 'Index Time (s)', fontsize=18)
ax2.legend(fontsize=18, frameon=False)

# Plot 3: Fixed alpha = 1.1, R = 64
subset_unoptimized = data_unoptimized[(data_unoptimized['alpha'] == 1.1) & (data_unoptimized['R'] == 64)]
subset_optimized = data_optimized[(data_optimized['alpha'] == 1.1) & (data_optimized['R'] == 64)]
ax3.plot(subset_unoptimized['L'], subset_unoptimized['index_time'], label='Random Medoid')
ax3.plot(subset_optimized['L'], subset_optimized['index_time'], label='True Medoid')
configure_plot(ax3, '(Alpha=1.1, R=64)', 'L', 'Index Time (s)', fontsize=18)
ax3.legend(fontsize=18, frameon=False)

plt.tight_layout()
plt.savefig('optimized_vamana.png')

