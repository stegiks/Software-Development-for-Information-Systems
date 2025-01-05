import subprocess
import os

# Define the ranges for num_points and num_processes
alpha_values = [1.0, 2.0, 3.0, 4.0, 5.0]
L_values = [150, 200, 250, 300, 350]
R_values = [10, 20, 30, 40, 50]
executable_name = "main"
log_file_list = [ "../logs/unfiltered_log" + str(i) + ".txt" for i in range(1, 6)]
executable_dir = "../bin"


for log_file in log_file_list:
    for alpha in alpha_values:
        for L in L_values:
            for R in R_values:    
                try:
                    
                    # Example of arguments -b ./data/siftsmall_base.fvecs -q ./data/siftsmall_query.fvecs -f fvecs -gt ./data/siftsmall_groundtruth.ivecs -a 1 -R 50 -L 150 -log ./log_unfiletered.txt
                    args_list = [
                        os.path.join(executable_dir, executable_name),
                        "-b",
                        "../data/siftsmall_base.fvecs",
                        "-q",
                        "../data/siftsmall_query.fvecs",
                        "-f",
                        "fvecs",
                        "-gt",
                        "../data/siftsmall_groundtruth.ivecs",
                        "-a",
                        str(alpha),
                        "-R",
                        str(R),
                        "-L",
                        str(L),
                        "-log",
                        log_file]
                    
                    # print(f"Running: {args_list} in {os.path.abspath(executable_dir)}")
                    result = subprocess.run(
                        args_list,
                        cwd=executable_dir,  # Set the working directory explicitly
                        capture_output=True,
                        text=True,
                        check=True,
                    )
                    print(f"Execution completed for alpha={alpha}, L={L}, R={R}")
                    # print(result.stdout)
                except subprocess.CalledProcessError as e:
                    print(f"Command: {' '.join(e.cmd)}")  # Full command that failed
                    print(f"Return code: {e.returncode}")  # Return code of the process
                    print(f"Standard output:\n{e.stdout}")  # Captured standard output
                    print(f"Standard error:\n{e.stderr}")  # Captured standard error
                except Exception as ex:
                    print(f"Unexpected error occurred: {str(ex)}")  # Catch any other unexpected errors
