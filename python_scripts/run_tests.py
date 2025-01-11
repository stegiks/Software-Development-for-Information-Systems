import subprocess
import os

def run_process(args_list, cwd):
    result = subprocess.run(
        args_list,
        cwd=cwd,
        capture_output=True,
        text=True,
        check=True,
    )
    return result.stdout, result.stderr

def compile_with_flags(flags):
    root_dir = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
    
    # Clean build
    subprocess.run(["make", "clean"], check=True, cwd=root_dir)
    
    # Construct make command with flags
    make_cmd = ["make", "OPTIMIZED=1", "PARALLEL0=1"]
    for flag_name, flag_value in flags.items():
        make_cmd.append(f"{flag_name}={flag_value}")
    
    # Run make with flags
    subprocess.run(make_cmd, check=True, cwd=root_dir)

def main():
    executable_dir = "../bin"
    executable_name = "main"
    num_runs = 3

    alpha_values = [1.0, 1.1, 1.2]
    L_values = [100, 135, 170]
    R_values = [32, 64, 96, 128]

    # Define optimization flags
    optimization_flags = [
        {"PARALLEL1": 1},
    ]

    # Define algorithms
    algorithms = ["regular", "filter", "stitch"]

    for flags in optimization_flags:
        compile_with_flags(flags)
        for algo in algorithms:
            for alpha in alpha_values:
                for L in L_values:
                    for R in R_values:

                        for run in range(1, num_runs + 1):
                            if algo == "regular":
                                # Regular Vamana
                                args_list = [
                                    os.path.join(executable_dir, executable_name),
                                    "-b", "../data/siftsmall_base.fvecs",
                                    "-q", "../data/siftsmall_query.fvecs",
                                    "-f", "fvecs",
                                    "-gt", "../data/siftsmall_groundtruth.ivecs",
                                    "-a", str(alpha),
                                    "-R", str(R),
                                    "-L", str(L),
                                    "-log", f"../logs/log_PARALLEL1_flag{flags['PARALLEL1']}_{run}.txt"
                                ]
                            else:
                                # Filtered or Stitched Vamana
                                args_list = [
                                    os.path.join(executable_dir, executable_name),
                                    "-b", "../data/dummy-data.bin",
                                    "-q", "../data/dummy-queries.bin",
                                    "-f", "bin",
                                    "-gt", "../data/dummy-groundtruth.bin",
                                    "-a", str(alpha),
                                    "-R", str(R),
                                    "-L", str(L),
                                    "-algo", algo,
                                    "-log", f"../logs/log_PARALLEL1_flag{flags['PARALLEL1']}_{run}.txt"
                                ]
                            try:
                                output, error = run_process(args_list, executable_dir)
                                print(output)  # Print the output for debugging
                                if error:
                                    print(f"Error: {error}")
                            except subprocess.CalledProcessError as e:
                                print(f"Command: {' '.join(e.cmd)}")
                                print(f"Return code: {e.returncode}")
                                print(f"Standard output:\n{e.stdout}")
                                print(f"Standard error:\n{e.stderr}")

if __name__ == "__main__":
    main()