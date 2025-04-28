import time
import numpy as np

start = time.perf_counter()

test = np.arange(1, 100_000_001)
test = test.reshape((100, 100, 100, 100))
sall = np.sum(test, 3)

end = time.perf_counter()

print(f"CPU time used(reshape|arange|sum): {end - start:.6f} seconds\n")
