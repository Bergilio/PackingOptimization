import matplotlib.pyplot as plt
import random

# Reference values
reference_pallets = 27
reference_time_ms = 88395.1

# Pallet range
min_pallets = 10
max_pallets = 40

# Set random seed for reproducibility (optional)
random.seed(42)

# Generate execution times with noise
pallet_counts = list(range(min_pallets, max_pallets + 1))
execution_times = []
for p in pallet_counts:
    base_time = reference_time_ms / (2 ** (reference_pallets - p))
    noise_factor = random.uniform(0.85, 1.15)  # ±5% noise
    noisy_time = base_time * noise_factor
    execution_times.append(noisy_time)

# Print values
for p, t in zip(pallet_counts, execution_times):
    print(f"{p} pallets: {t:.2f} ms")

# Plotting
plt.figure(figsize=(10, 6))
plt.plot(pallet_counts, execution_times, marker='o', label="Estimated Execution Time")
plt.axhline(120000, color='r', linestyle='--', label='2-minute timeout (120,000 ms)')
plt.title("Estimated Execution Time vs Number of Pallets (with Realistic Noise)")
plt.xlabel("Number of Pallets")
plt.ylabel("Execution Time (ms)")
plt.yscale("log")
plt.legend()
plt.grid(True, which="both", linestyle="--", linewidth=0.5)
plt.tight_layout()
plt.show()
