# 0x1F. Search Algorithm Visualizer

An animated, colorful terminal visualizer for the search algorithms taught in
[`0x1E-search_algorithms`](../0x1E-search_algorithms). It draws the sorted array
as a row of colored cells and animates, step by step, how each algorithm narrows
the search space, which element it probes, and what gets eliminated — while
counting comparisons in real time.

This is a small C project that ties together a lot of the concepts from this
repo: pointers and arrays, structs and typedefs, function pointers, dynamic
memory (`malloc`/`free`), variadic functions, `math.h`, and Makefiles.

## Algorithms

| Key      | Algorithm              | Time complexity     |
| -------- | ---------------------- | ------------------- |
| `linear` | Linear search          | `O(n)`              |
| `binary` | Binary search          | `O(log n)`          |
| `jump`   | Jump search            | `O(sqrt n)`         |
| `interp` | Interpolation search   | `O(log log n)` avg  |
| `exp`    | Exponential search     | `O(log n)`          |

## Build

```sh
make
```

Requires a C compiler and `libm` (linked automatically). Use any
ANSI-color-capable terminal.

## Usage

Interactive menu (pick an algorithm and a target, watch it run):

```sh
./search_viz
```

One-shot from the command line:

```sh
# ./search_viz [algorithm] [target] [size] [delay_ms]
./search_viz binary 53
./search_viz jump 53 32 200
./search_viz interp 999 24 120   # a value that is not present
```

- `algorithm` — one of the keys in the table above
- `target` — value to search for (defaults to the middle element)
- `size` — number of elements in the generated sorted array (default `16`)
- `delay_ms` — animation frame delay in milliseconds (default `350`)

Show help:

```sh
./search_viz --help
```

## Legend

- **Blue** cells: the current active search window
- **Bright yellow** cell: the element being compared this step
- **Dim grey** cells: eliminated from the search space
- **Green** cell: the matched value

## Files

| File            | Purpose                                                  |
| --------------- | -------------------------------------------------------- |
| `visualizer.h`  | Shared types and prototypes                              |
| `visualizer.c`  | Rendering/animation engine + array generation           |
| `algorithms.c`  | Instrumented search algorithms that drive the renderer   |
| `main.c`        | CLI parsing, algorithm dispatch, and interactive menu    |
| `Makefile`      | Build rules (`make`, `make run`, `make clean`, `make re`)|
