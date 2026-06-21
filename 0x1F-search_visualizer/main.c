#include "visualizer.h"

/**
 * struct algo_s - Maps a menu/CLI key to a search implementation
 * @key: Short identifier accepted on the command line
 * @label: Human readable label shown in the menu
 * @run: Function pointer to the instrumented algorithm
 */
typedef struct algo_s
{
	const char *key;
	const char *label;
	int (*run)(viz_t *);
} algo_t;

static const algo_t g_algos[] = {
	{"linear", "Linear Search          O(n)", viz_linear_search},
	{"binary", "Binary Search          O(log n)", viz_binary_search},
	{"jump", "Jump Search            O(sqrt n)", viz_jump_search},
	{"interp", "Interpolation Search   O(log log n)", viz_interpolation_search},
	{"exp", "Exponential Search     O(log n)", viz_exponential_search},
	{NULL, NULL, NULL}
};

/**
 * find_algo - Look up an algorithm by its CLI key
 * @key: Identifier to match
 *
 * Return: Pointer to the matching algo_t, or NULL
 */
static const algo_t *find_algo(const char *key)
{
	size_t i;

	for (i = 0; g_algos[i].key; i++)
		if (strcmp(g_algos[i].key, key) == 0)
			return (&g_algos[i]);
	return (NULL);
}

/**
 * print_array - Print the array on one line (used by the menu)
 * @array: Array to print
 * @size: Number of elements
 */
static void print_array(int *array, size_t size)
{
	size_t i;

	printf("  Array: ");
	for (i = 0; i < size; i++)
		printf("%d%s", array[i], i + 1 < size ? ", " : "\n");
}

/**
 * usage - Print command line usage and the available algorithm keys
 * @prog: argv[0]
 */
static void usage(const char *prog)
{
	size_t i;

	printf("Usage: %s [algorithm] [target] [size] [delay_ms]\n", prog);
	printf("       %s            (interactive menu)\n\n", prog);
	printf("Algorithms:\n");
	for (i = 0; g_algos[i].key; i++)
		printf("  %-8s %s\n", g_algos[i].key, g_algos[i].label);
}

/**
 * run_once - Run a single animated search and print the result
 * @algo: Algorithm to run
 * @array: Sorted array to search
 * @size: Number of elements
 * @target: Value to look for
 * @delay_us: Frame delay in microseconds
 */
static void run_once(const algo_t *algo, int *array, size_t size,
					 int target, useconds_t delay_us)
{
	viz_t *v;
	int idx;

	v = viz_create(array, size, target, delay_us);
	if (v == NULL)
		return;
	idx = algo->run(v);
	viz_result(v, idx);
	viz_free(v);
}

/**
 * read_line - Read a trimmed line from stdin into a buffer
 * @buf: Destination buffer
 * @n: Size of @buf
 *
 * Return: 1 on success, 0 on EOF
 */
static int read_line(char *buf, size_t n)
{
	if (fgets(buf, n, stdin) == NULL)
		return (0);
	buf[strcspn(buf, "\n")] = '\0';
	return (1);
}

/**
 * interactive - Run the menu-driven loop
 * @array: Sorted array to search
 * @size: Number of elements
 *
 * Return: 0 on normal exit
 */
static int interactive(int *array, size_t size)
{
	char line[64];
	size_t i;
	int target;
	long choice;

	while (1)
	{
		printf("\n\033[1;96m=== ALX Search Visualizer ===\033[0m\n");
		print_array(array, size);
		printf("\nChoose an algorithm:\n");
		for (i = 0; g_algos[i].key; i++)
			printf("  %zu) %s\n", i + 1, g_algos[i].label);
		printf("  0) Quit\n> ");
		fflush(stdout);
		if (!read_line(line, sizeof(line)))
			break;
		choice = strtol(line, NULL, 10);
		if (choice == 0)
			break;
		if (choice < 1 || (size_t)choice > i)
		{
			printf("Invalid choice.\n");
			continue;
		}
		printf("Target value to search for: ");
		fflush(stdout);
		if (!read_line(line, sizeof(line)))
			break;
		target = (int)strtol(line, NULL, 10);
		run_once(&g_algos[choice - 1], array, size, target, 350000);
		printf("\nPress Enter to continue...");
		fflush(stdout);
		read_line(line, sizeof(line));
	}
	printf("Bye!\n");
	return (0);
}

/**
 * main - Entry point. Either runs a one-shot CLI search or the menu.
 * @argc: Argument count
 * @argv: Argument vector: [algorithm] [target] [size] [delay_ms]
 *
 * Return: 0 on success, 1 on usage error
 */
int main(int argc, char **argv)
{
	int *array;
	size_t size = 16;
	int target;
	useconds_t delay_us = 350000;
	const algo_t *algo;

	if (argc >= 2 && (strcmp(argv[1], "-h") == 0 ||
		strcmp(argv[1], "--help") == 0))
	{
		usage(argv[0]);
		return (0);
	}

	if (argc == 1)
	{
		array = make_sorted_array(size, 42);
		if (array == NULL)
			return (1);
		interactive(array, size);
		free(array);
		return (0);
	}

	algo = find_algo(argv[1]);
	if (algo == NULL)
	{
		printf("Unknown algorithm: %s\n\n", argv[1]);
		usage(argv[0]);
		return (1);
	}
	if (argc >= 4)
		size = (size_t)strtoul(argv[3], NULL, 10);
	if (size == 0)
		size = 16;
	if (argc >= 5)
		delay_us = (useconds_t)(strtoul(argv[4], NULL, 10) * 1000);

	array = make_sorted_array(size, 42);
	if (array == NULL)
		return (1);
	target = argc >= 3 ? (int)strtol(argv[2], NULL, 10)
					   : array[size / 2];
	run_once(algo, array, size, target, delay_us);
	free(array);
	return (0);
}
