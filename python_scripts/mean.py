import numpy as np

def read_file(filename):
    """Reads the file and splits lines into lists of values."""
    with open(filename, 'r') as f:
        lines = [line.strip().split() for line in f]
    return lines

def is_number(s):
    """Checks if a string can be converted to a float."""
    try:
        float(s)
        return True
    except ValueError:
        return False

def compute_means(files):
    """Computes the mean across multiple files for numeric columns."""
    # Read all files into a list of lines
    data = [read_file(file) for file in files]
    
    # Initialize the output
    result = []
    
    # Iterate over rows (assumes all files have the same structure)
    for rows in zip(*data):
        # Transpose the row to separate columns
        columns = list(zip(*rows))
        
        # Process each column
        mean_row = []
        for i, col in enumerate(columns):
            if(i < 3):
                mean_row.append(col[0])
                continue
            
            if is_number(col[0]):  # Check if column contains numeric data
                mean_row.append(f"{np.mean([float(x) for x in col]):.6f}")
            else:
                # Assume non-numeric columns are the same across files
                mean_row.append(col[0])
        result.append(mean_row)
    
    return result

def write_output(filename, data):
    """Writes the output to a file."""
    with open(filename, 'w') as f:
        for row in data:
            f.write('\t'.join(row) + '\n')

import os
# File paths (replace with your actual file paths)
log_folder = '../logs/un-optimized'
files = os.listdir(log_folder)  # Replace with actual file paths
for i, file in enumerate(files):
    files[i] = os.path.join(log_folder, file)

print(files)
output_file = 'mean_results.txt'
output_file = os.path.join(log_folder, output_file)

# Compute the mean and write to output
mean_data = compute_means(files)
write_output(output_file, mean_data)

print(f"Mean results written to {output_file}")