#ifndef VISUALIZER_H
#define VISUALIZER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>

/**
 * enum cell_state_e - Visual state of a single array cell while drawing a frame
 *
 * @CELL_IDLE: Element is still part of the search space but inactive this step
 * @CELL_WINDOW: Element is inside the current active search window [lo, hi]
 * @CELL_PROBE: Element is the one being compared on this step
 * @CELL_EATEN: Element was eliminated from the search space
 * @CELL_FOUND: Element matched the target value
 */
typedef enum cell_state_e
{
	CELL_IDLE = 0,
	CELL_WINDOW,
	CELL_PROBE,
	CELL_EATEN,
	CELL_FOUND
} cell_state_t;

/**
 * struct viz_s - Shared visualization context passed to every algorithm
 *
 * @array: The sorted array being searched
 * @size: Number of elements in @array
 * @value: The target value being searched for
 * @algo_name: Human readable name of the running algorithm
 * @complexity: Big-O label for the running algorithm
 * @comparisons: Running count of element comparisons performed
 * @step: Running count of rendered animation frames
 * @delay_us: Microseconds to pause between frames (animation speed)
 * @state: Per-cell visual state, parallel to @array (length @size)
 * @msg: One-line status message shown under the array
 */
typedef struct viz_s
{
	int *array;
	size_t size;
	int value;
	const char *algo_name;
	const char *complexity;
	size_t comparisons;
	size_t step;
	useconds_t delay_us;
	cell_state_t *state;
	char msg[256];
} viz_t;

/* Rendering / animation engine (visualizer.c) */
viz_t *viz_create(int *array, size_t size, int value, useconds_t delay_us);
void viz_free(viz_t *v);
void viz_reset_states(viz_t *v);
void viz_mark_window(viz_t *v, size_t lo, size_t hi);
void viz_mark_eaten_outside(viz_t *v, size_t lo, size_t hi);
void viz_set_msg(viz_t *v, const char *fmt, ...);
void viz_draw(viz_t *v, int probe_index, cell_state_t probe_state);
void viz_banner(viz_t *v);
void viz_result(viz_t *v, int found_index);

/* Instrumented search algorithms (algorithms.c) */
int viz_linear_search(viz_t *v);
int viz_binary_search(viz_t *v);
int viz_jump_search(viz_t *v);
int viz_interpolation_search(viz_t *v);
int viz_exponential_search(viz_t *v);

/* Helpers (visualizer.c) */
int *make_sorted_array(size_t size, unsigned int seed);

#endif /* VISUALIZER_H */
