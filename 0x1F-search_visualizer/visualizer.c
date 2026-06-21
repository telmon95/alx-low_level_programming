#include "visualizer.h"
#include <stdarg.h>

/* ANSI escape helpers */
#define ESC "\033["
#define RESET ESC "0m"
#define CLEAR ESC "2J" ESC "H"
#define HIDE_CURSOR ESC "?25l"
#define SHOW_CURSOR ESC "?25h"

/**
 * cell_open - Emit the opening ANSI color sequence for a cell state
 * @s: The visual state of the cell
 */
static void cell_open(cell_state_t s)
{
	switch (s)
	{
	case CELL_FOUND:
		fputs(ESC "1;30;42m", stdout);   /* bold black on green */
		break;
	case CELL_PROBE:
		fputs(ESC "1;30;103m", stdout);  /* bold black on bright yellow */
		break;
	case CELL_WINDOW:
		fputs(ESC "97;44m", stdout);     /* white on blue */
		break;
	case CELL_EATEN:
		fputs(ESC "2;90m", stdout);      /* dim grey */
		break;
	case CELL_IDLE:
	default:
		fputs(ESC "37m", stdout);        /* plain */
		break;
	}
}

/**
 * viz_create - Allocate a visualization context for an array
 * @array: Sorted array to be searched
 * @size: Number of elements in @array
 * @value: Target value to search for
 * @delay_us: Animation frame delay in microseconds
 *
 * Return: Pointer to a new context, or NULL on failure
 */
viz_t *viz_create(int *array, size_t size, int value, useconds_t delay_us)
{
	viz_t *v;

	v = malloc(sizeof(*v));
	if (v == NULL)
		return (NULL);
	v->state = calloc(size, sizeof(*v->state));
	if (v->state == NULL)
	{
		free(v);
		return (NULL);
	}
	v->array = array;
	v->size = size;
	v->value = value;
	v->algo_name = "";
	v->complexity = "";
	v->comparisons = 0;
	v->step = 0;
	v->delay_us = delay_us;
	v->msg[0] = '\0';
	return (v);
}

/**
 * viz_free - Release a visualization context (not the underlying array)
 * @v: Context to free
 */
void viz_free(viz_t *v)
{
	if (v == NULL)
		return;
	free(v->state);
	free(v);
}

/**
 * viz_reset_states - Reset every cell back to the idle state
 * @v: Visualization context
 */
void viz_reset_states(viz_t *v)
{
	size_t i;

	for (i = 0; i < v->size; i++)
		v->state[i] = CELL_IDLE;
}

/**
 * viz_mark_window - Mark the inclusive range [lo, hi] as the active window
 * @v: Visualization context
 * @lo: Lower bound index
 * @hi: Upper bound index
 */
void viz_mark_window(viz_t *v, size_t lo, size_t hi)
{
	size_t i;

	for (i = lo; i <= hi && i < v->size; i++)
		v->state[i] = CELL_WINDOW;
}

/**
 * viz_mark_eaten_outside - Mark every cell outside [lo, hi] as eliminated
 * @v: Visualization context
 * @lo: Lower bound index of the surviving window
 * @hi: Upper bound index of the surviving window
 */
void viz_mark_eaten_outside(viz_t *v, size_t lo, size_t hi)
{
	size_t i;

	for (i = 0; i < v->size; i++)
		if (i < lo || i > hi)
			v->state[i] = CELL_EATEN;
}

/**
 * viz_set_msg - Set the one-line status message (printf-style)
 * @v: Visualization context
 * @fmt: Format string
 */
void viz_set_msg(viz_t *v, const char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	vsnprintf(v->msg, sizeof(v->msg), fmt, ap);
	va_end(ap);
}

/**
 * viz_banner - Print the static header for the current run
 * @v: Visualization context
 */
void viz_banner(viz_t *v)
{
	fputs(CLEAR HIDE_CURSOR, stdout);
	printf(ESC "1;96m" "  ALX Search Visualizer  " RESET "\n");
	printf("  Algorithm : " ESC "1;93m" "%s" RESET "  (" ESC "95m" "%s"
		   RESET ")\n", v->algo_name, v->complexity);
	printf("  Target    : " ESC "1;92m" "%d" RESET "\n", v->value);
}

/**
 * viz_draw - Render one animation frame and pause
 * @v: Visualization context
 * @probe_index: Index currently being compared (-1 for none)
 * @probe_state: Visual state to apply to @probe_index
 */
void viz_draw(viz_t *v, int probe_index, cell_state_t probe_state)
{
	size_t i;
	char buf[16];

	if (probe_index >= 0 && (size_t)probe_index < v->size)
		v->state[probe_index] = probe_state;
	v->step++;

	viz_banner(v);
	printf("\n  ");
	for (i = 0; i < v->size; i++)
	{
		snprintf(buf, sizeof(buf), "%4d", v->array[i]);
		cell_open(v->state[i]);
		printf(" %s ", buf);
		fputs(RESET, stdout);
		putchar(' ');
	}
	printf("\n  ");
	for (i = 0; i < v->size; i++)
		printf(" [%2zu] ", i);
	printf("\n\n");

	printf("  Step: " ESC "1m" "%-4zu" RESET "  Comparisons: " ESC "1m"
		   "%-4zu" RESET "\n", v->step, v->comparisons);
	if (v->msg[0] != '\0')
		printf("  " ESC "96m" "%s" RESET "\n", v->msg);

	fflush(stdout);
	if (v->delay_us)
		usleep(v->delay_us);
}

/**
 * viz_result - Render the final result frame and restore the cursor
 * @v: Visualization context
 * @found_index: Index where the value was found, or -1
 */
void viz_result(viz_t *v, int found_index)
{
	printf("\n");
	if (found_index >= 0)
		printf("  " ESC "1;42;30m" " FOUND " RESET " value " ESC "1;92m" "%d"
			   RESET " at index " ESC "1;92m" "%d" RESET "\n",
			   v->value, found_index);
	else
		printf("  " ESC "1;41;97m" " NOT FOUND " RESET " value " ESC "1;91m"
			   "%d" RESET " is not in the array\n", v->value);
	printf("  Total comparisons: " ESC "1m" "%zu" RESET " over " ESC "1m"
		   "%zu" RESET " frames\n", v->comparisons, v->step);
	fputs(SHOW_CURSOR, stdout);
	fflush(stdout);
}

/**
 * make_sorted_array - Build a sorted array of pseudo-random distinct-ish ints
 * @size: Number of elements to generate
 * @seed: Seed for reproducible output
 *
 * Return: Malloc'd sorted array, or NULL on failure
 */
int *make_sorted_array(size_t size, unsigned int seed)
{
	int *array;
	size_t i;
	int acc = 0;

	if (size == 0)
		return (NULL);
	array = malloc(size * sizeof(*array));
	if (array == NULL)
		return (NULL);
	srand(seed);
	for (i = 0; i < size; i++)
	{
		acc += 1 + (rand() % 7);
		array[i] = acc;
	}
	return (array);
}
