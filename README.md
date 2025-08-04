# NumC

A **NumPy-like** numerical computing library for C. Built to provide an
intuitive and powerful API for N-dimensional arrays, NumC brings the
expressiveness of array-oriented programming to C, complete with broadcasting,
slicing, and reduction operations.

## Table of Contents

* [In Action](#in-action)
* [Build and Installation](#build-and-installation)
* [Quick Start](#quick-start)
* [Features](#features)
* [Core Concepts](#core-concepts)
* [API Reference](#api-reference)
* [Advanced Topics](#advanced-topics)

***

## In Action

NumC allows you to write expressive numerical code, and the `nc_display` utility gives you a clear view of your arrays.

```c
// See the full code in the Quick Start section
ndarray_t *A = nc_reshape(nc_arange(0, 6, 1, nc_double), SND_INLINE(2, 3), true);
ndarray_t *B = nc_reshape(nc_arange(0, 3, 1, nc_double), SND_INLINE(1, 3), true);
ndarray_t *C = nc_add(A, B); // A broadcasts to B's shape

// Let's see what we made!
nc_display(A, true);
nc_display(B, true);
nc_display(C, true);

nc_free(&A);
nc_free(&B);
nc_free(&C);
```

**Output:**

```
ndarray_t:
  ndim: 2
  shape: [2, 3]
  strides: [24, 8]
  dtype: 2
  total_size: 6
  item_size: 8
  data:([
   [0.000000, 1.000000, 2.000000],
   [3.000000, 4.000000, 5.000000]
  ])
ndarray_t:
  ndim: 2
  shape: [1, 3]
  strides: [24, 8]
  dtype: 2
  total_size: 3
  item_size: 8
  data:([
   [0.000000, 1.000000, 2.000000]
  ])
ndarray_t:
  ndim: 2
  shape: [2, 3]
  strides: [24, 8]
  dtype: 2
  total_size: 6
  item_size: 8
  data:([
   [0.000000, 2.000000, 4.000000],
   [3.000000, 5.000000, 7.000000]
  ])
```
---

## Build and Installation

A standard C compiler (GCC or Clang), `make`, and `cmake` are required.

### Development (Building from Source)

The `Makefile` is provided for development on NumC itself.

```bash
# Build the static library
make

# Run the test suite
make test

# Clean build artifacts
make clean
```

### Installation (System-wide for Consumers)

For using NumC as a dependency in another project, a CMake-based installation is the standard method. This installs the library and headers to a system-standard location.

```bash
# Create an out-of-source build directory
mkdir build && cd build

# Configure and generate the build system
cmake ..

# Compile the library
make

# Install the library and headers (may require sudo)
sudo make install
```
---

## Quick Start

1.  **Get the code:** Clone the repository and navigate into it.

    ```bash
    git clone https://github.com/ShashwatAgrawal20/numc.git
    cd numc
    ```

2.  **Write some code:** Create a test file (e.g., `main.c`).

    ```c
    #include <numc/numc.h>

    int main() {
        // Create a 2x3 array with values [0, 1, 2, 3, 4, 5]
        ndarray_t *A = nc_reshape(nc_arange(0, 6, 1, nc_double), SND_INLINE(2, 3), true);

        // Create a 1x3 array with values [0, 1, 2]
        ndarray_t *B = nc_reshape(nc_arange(0, 3, 1, nc_double), SND_INLINE(1, 3), true);

        // Add them. B will be broadcast to the shape of A.
        printf("Adding two arrays with broadcasting...\n");
        ndarray_t *C = nc_add(A, B);

        // Display results
        nc_display(A, true);
        nc_display(B, true);
        nc_display(C, true);

        // Clean up
        nc_free(&A); // A was reshaped inline, so it now points to the same data as nc_arange
        // B and C were created by functions, so they own their data and must be freed
        nc_free(&B);
        nc_free(&C);

        return 0;
    }
    ```

3.  **Compile and Run:** NumC is a multi-file library. The easiest way to compile is to build all `numc` source files along with your own.

    ```bash
    # This assumes your numc source files are in a 'src' directory
    gcc main.c src/*.c -Iinclude -o main && ./main
    ```

## Features

* **N-Dimensional Arrays**: The `ndarray_t` is the heart of the library, capable of handling any number of dimensions.
* **NumPy-like API**: Familiar function names and behavior (`arange`, `reshape`, `sum`, etc.).
* **Broadcasting**: Automatically applies element-wise operations to arrays of compatible but different shapes.
* **Type-Generic**: Supports `int`, `float`, and `double` data types.
* **Powerful Reductions**: `nc_sum` can operate over the entire array or along a specific axis.
* **Zero-Copy Slicing**: `nc_slice` creates efficient *views* into array data without copying.
* **Convenience Macros**: `SND_INLINE` and `NC_..._OPTS` macros make writing array code clean and easy.

---

## Core Concepts

### The `ndarray_t`

The `ndarray_t` struct contains not just the data, but also metadata about the array's shape, dimensions, and data type.

### Array Creation

Create arrays using `nc_arange`, `nc_create`, or by reshaping an existing array.

```c
// Create a 1D array from 0 to 9
ndarray_t *a = nc_arange(0, 10, 1, nc_int);

// Create an uninitialized 4x5 array of floats
size_t shape[] = {4, 5};
ndarray_t *b = nc_create(SND(shape), nc_float);

// Reshape 'a' into a 2x5 array. The 'true' makes it an inline operation.
ndarray_t *c = nc_reshape(a, SND_INLINE(2, 5), true);
```

> \[!NOTE\]
> The `SND_INLINE(...)` macro is a powerful variadic helper that lets you define a shape on-the-fly without creating a separate array.

### Memory Management

Always free arrays with `nc_free()`. It requires a pointer to your array variable (`ndarray_t**`) to safely nullify it after freeing.

```c
ndarray_t *my_array = nc_arange(0, 10, 1, nc_int);
// ... do work ...
nc_free(&my_array); // my_array is now NULL
```

---

## API Reference

### Array Creation & Manipulation

| Function Signature                                                             | Description                                                                                    |
| ------------------------------------------------------------------------------ | ---------------------------------------------------------------------------------------------- |
| `ndarray_t* nc_create(size_t* shape, int ndim, dtype_t dtype)`                  | Creates an uninitialized array with the given shape and data type.                             |
| `ndarray_t* nc_arange(double start, double stop, double step, dtype_t dtype)`   | Creates a 1D array containing a sequence of numbers.                                           |
| `ndarray_t* nc_reshape(ndarray_t* array, size_t* shape, int ndim, bool is_inline)` | Returns a new array (or modifies `array` in-place if `is_inline` is true) with a new shape. |
| `void nc_free(ndarray_t** array)`                                               | Frees all memory associated with an array and sets the pointer to `NULL`.                      |
| `void nc_display(ndarray_t* array, bool print_data)`                            | Prints the metadata and (optionally) the contents of an array.                                 |

### Element Access

| Function Signature                                          | Description                                                        |
| ----------------------------------------------------------- | ------------------------------------------------------------------ |
| `void nc_set(ndarray_t* array, size_t* indices, void* value)` | Sets the value at the specified N-dimensional index.               |
| `void* nc_get(ndarray_t* array, size_t* indices)`            | Returns a pointer to the value at the specified N-dimensional index. |

### Slicing

Slicing creates a **view**, not a copy. Changes to the view affect the original array.

| Function Signature                                                      | Description                                                              |
| ----------------------------------------------------------------------- | ------------------------------------------------------------------------ |
| `ndarray_t* nc_slice(ndarray_t* array, slice_t* slices, int num_slices)` | Creates a view into the array based on an array of `slice_t` structs. |

A `slice_t` defines how to slice one dimension: `{ .start=0, .end=0, .step=1 }`.

### Element-wise Operations

These operations support broadcasting between compatible arrays.

| Function       | Description                   |
| -------------- | ----------------------------- |
| `nc_add(a, b)` | Element-wise addition.        |
| `nc_sub(a, b)` | Element-wise subtraction.     |
| `nc_mul(a, b)` | Element-wise multiplication.  |
| `nc_div(a, b)` | Element-wise division.        |

### Reduction Operations

| Function Signature                                                   | Description                                                              |
| -------------------------------------------------------------------- | ------------------------------------------------------------------------ |
| `ndarray_t* nc_sum(const ndarray_t* array, const nc_sum_opts* opts)` | Computes the sum of array elements over a given axis or the entire array. |

Use the `NC_SUM_DEFAULT_OPTS(...)` macro to easily specify options:

```c
// Sum along axis 1
ndarray_t* result = nc_sum(my_array, NC_SUM_DEFAULT_OPTS(.axis = 1));
```

---

## Advanced Topics

### Broadcasting

Broadcasting is the mechanism that allows NumC to perform arithmetic on arrays of different shapes. The rules are:

1.  The arrays are aligned from their trailing dimensions.
2.  When comparing dimensions, they must either be equal, or one of them must be 1.
3.  Missing dimensions are treated as 1.

Example: A `(2, 3)` array and a `(3)` array are compatible. The `(3)` array is promoted to `(1, 3)` and then broadcast across the first dimension to match `(2, 3)`.

### Views vs. Copies

* **Views** (`nc_slice`, `nc_reshape` with `is_inline=true`): These operations are fast because they don't copy data. They create a new `ndarray_t` struct that points to the *original* data. Modifying a view modifies the source array.
* **Copies** (`nc_add`, `nc_sub`, `nc_reshape` with `is_inline=false`): These operations allocate new memory and create a completely independent array.

> \[!CAUTION\]
> Be mindful of data ownership. If you create a view from an array and then free the original array, the view's data pointer becomes invalid.
