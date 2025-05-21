import sys
import csv
from pulp import LpMaximize, LpProblem, LpVariable, lpSum, LpBinary


def read_truck(file_path):
    with open(file_path, newline='') as csvfile:
        reader = csv.DictReader(csvfile)
        for row in reader:
            return int(row['Capacity'])


def read_pallets(file_path):
    weights = []
    profits = []
    with open(file_path, newline='') as csvfile:
        reader = csv.DictReader(csvfile)
        for row in reader:
            weights.append(int(row['Weight']))
            profits.append(float(row['Profit']))
    return weights, profits


def main():
    if len(sys.argv) != 3:
        print("Usage: python3 ilp_solver.py <truck.csv> <pallets.csv>")
        return

    truck_file = sys.argv[1]
    pallets_file = sys.argv[2]

    capacity = read_truck(truck_file)
    weights, profits = read_pallets(pallets_file)
    num_items = len(weights)

    # Define the ILP problem
    model = LpProblem(name="knapsack-ilp", sense=LpMaximize)

    x = [LpVariable(f"x_{i}", cat=LpBinary) for i in range(num_items)]

    model += lpSum(x[i] * profits[i] for i in range(num_items)), "TotalProfit"
    model += lpSum(x[i] * weights[i] for i in range(num_items)) <= capacity, "WeightLimit"

    model.solve()

    total_profit = sum(profits[i] for i in range(num_items) if x[i].value() == 1)
    selected_indices = [i for i in range(num_items) if x[i].value() == 1]

    # Print the result in the format C++ expects
    print(total_profit)
    print(" ".join(str(i) for i in selected_indices))


if __name__ == "__main__":
    main()
