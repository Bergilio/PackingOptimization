import os
import re
import pandas as pd
import matplotlib.pyplot as plt

def main():
    output_dir = "../output"
    pattern = re.compile(r"Solution_(\d+)_([1-4])\.txt")

    data = []

    for filename in os.listdir(output_dir):
        match = pattern.match(filename)
        if match:
            dataset_num = int(match.group(1))
            approach_num = int(match.group(2))
            filepath = os.path.join(output_dir, filename)

            with open(filepath, 'r') as f:
                for line in f:
                    if "Execution time:" in line:
                        time_ms = float(re.search(r"Execution time: ([\d.]+) ms", line).group(1))
                        data.append({
                            "Dataset": dataset_num,
                            "Approach": approach_num,
                            "ExecutionTimeMs": time_ms
                        })
                        break

    df = pd.DataFrame(data)

    approach_names = {
        1: "Brute Force",
        2: "Dynamic Programming",
        3: "Greedy",
        4: "ILP"
    }
    df["ApproachName"] = df["Approach"].map(approach_names)
    df.sort_values(["Dataset", "Approach"], inplace=True)
    df.to_csv("execution_times.csv", index=False)
    print("Execution times saved to execution_times.csv")

    # Plot each approach separately
    for approach, group in df.groupby("ApproachName"):
        plt.figure(figsize=(10, 5))
        plt.plot(group["Dataset"], group["ExecutionTimeMs"], marker='o')
        plt.title(f"Execution Time per Dataset - {approach}")
        plt.xlabel("Dataset Number")
        plt.ylabel("Execution Time (ms)")
        plt.grid(True)
        plt.tight_layout()
        plt.savefig(f"execution_time_{approach.replace(' ', '_').lower()}.png")
        plt.show()

if __name__ == "__main__":
    main()
