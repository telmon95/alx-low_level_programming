#include "visualizer.h"

/**
 * viz_linear_search - Animated linear search
 * @v: Visualization context
 *
 * Return: Index of the value, or -1 if not found
 */
int viz_linear_search(viz_t *v)
{
	size_t i;

	v->algo_name = "Linear Search";
	v->complexity = "O(n)";

	for (i = 0; i < v->size; i++)
	{
		viz_reset_states(v);
		viz_mark_eaten_outside(v, i, v->size - 1);
		v->comparisons++;
		viz_set_msg(v, "Comparing array[%zu] = %d with target %d",
					i, v->array[i], v->value);
		if (v->array[i] == v->value)
		{
			viz_draw(v, (int)i, CELL_FOUND);
			return ((int)i);
		}
		viz_draw(v, (int)i, CELL_PROBE);
	}
	viz_set_msg(v, "Reached the end of the array without a match");
	viz_draw(v, -1, CELL_IDLE);
	return (-1);
}

/**
 * viz_binary_search - Animated binary search
 * @v: Visualization context
 *
 * Return: Index of the value, or -1 if not found
 */
int viz_binary_search(viz_t *v)
{
	size_t lo = 0, hi = v->size - 1, mid;

	v->algo_name = "Binary Search";
	v->complexity = "O(log n)";

	while (hi >= lo)
	{
		mid = lo + (hi - lo) / 2;
		viz_reset_states(v);
		viz_mark_window(v, lo, hi);
		viz_mark_eaten_outside(v, lo, hi);
		v->comparisons++;
		viz_set_msg(v, "Window [%zu..%zu]  mid=%zu  array[%zu]=%d",
					lo, hi, mid, mid, v->array[mid]);
		if (v->array[mid] == v->value)
		{
			viz_draw(v, (int)mid, CELL_FOUND);
			return ((int)mid);
		}
		viz_draw(v, (int)mid, CELL_PROBE);
		if (v->array[mid] > v->value)
		{
			if (mid == 0)
				break;
			hi = mid - 1;
		}
		else
			lo = mid + 1;
	}
	viz_reset_states(v);
	viz_mark_eaten_outside(v, 1, 0);
	viz_set_msg(v, "Window collapsed without a match");
	viz_draw(v, -1, CELL_IDLE);
	return (-1);
}

/**
 * viz_jump_search - Animated jump search
 * @v: Visualization context
 *
 * Return: Index of the value, or -1 if not found
 */
int viz_jump_search(viz_t *v)
{
	size_t step, prev = 0, curr = 0;

	v->algo_name = "Jump Search";
	v->complexity = "O(sqrt n)";
	step = (size_t)sqrt((double)v->size);
	if (step == 0)
		step = 1;

	while (curr < v->size && v->array[curr] < v->value)
	{
		viz_reset_states(v);
		viz_mark_window(v, prev, curr);
		v->comparisons++;
		viz_set_msg(v, "Jumping by %zu: array[%zu]=%d < %d, leap ahead",
					step, curr, v->array[curr], v->value);
		viz_draw(v, (int)curr, CELL_PROBE);
		prev = curr;
		curr += step;
	}

	if (curr >= v->size)
		curr = v->size - 1;
	for (; prev <= curr; prev++)
	{
		viz_reset_states(v);
		viz_mark_window(v, prev, curr);
		v->comparisons++;
		viz_set_msg(v, "Linear scan of block: array[%zu]=%d",
					prev, v->array[prev]);
		if (v->array[prev] == v->value)
		{
			viz_draw(v, (int)prev, CELL_FOUND);
			return ((int)prev);
		}
		viz_draw(v, (int)prev, CELL_PROBE);
		if (v->array[prev] > v->value)
			break;
	}
	viz_set_msg(v, "Value not present in the scanned block");
	viz_draw(v, -1, CELL_IDLE);
	return (-1);
}

/**
 * viz_interpolation_search - Animated interpolation search
 * @v: Visualization context
 *
 * Return: Index of the value, or -1 if not found
 */
int viz_interpolation_search(viz_t *v)
{
	size_t lo = 0, hi = v->size - 1, pos;

	v->algo_name = "Interpolation Search";
	v->complexity = "O(log log n) avg";

	while (hi >= lo && v->value >= v->array[lo] && v->value <= v->array[hi])
	{
		if (v->array[hi] == v->array[lo])
			pos = lo;
		else
			pos = lo + (size_t)(((double)(hi - lo) /
				(v->array[hi] - v->array[lo])) * (v->value - v->array[lo]));
		if (pos >= v->size)
			break;
		viz_reset_states(v);
		viz_mark_window(v, lo, hi);
		viz_mark_eaten_outside(v, lo, hi);
		v->comparisons++;
		viz_set_msg(v, "Estimating pos=%zu in [%zu..%zu]  array[%zu]=%d",
					pos, lo, hi, pos, v->array[pos]);
		if (v->array[pos] == v->value)
		{
			viz_draw(v, (int)pos, CELL_FOUND);
			return ((int)pos);
		}
		viz_draw(v, (int)pos, CELL_PROBE);
		if (v->array[pos] < v->value)
			lo = pos + 1;
		else
		{
			if (pos == 0)
				break;
			hi = pos - 1;
		}
	}
	viz_set_msg(v, "Value outside the interpolated range");
	viz_draw(v, -1, CELL_IDLE);
	return (-1);
}

/**
 * viz_exponential_search - Animated exponential search
 * @v: Visualization context
 *
 * Return: Index of the value, or -1 if not found
 */
int viz_exponential_search(viz_t *v)
{
	size_t bound = 1, lo, hi, mid;

	v->algo_name = "Exponential Search";
	v->complexity = "O(log n)";

	while (bound < v->size && v->array[bound] < v->value)
	{
		viz_reset_states(v);
		viz_mark_window(v, 0, bound);
		v->comparisons++;
		viz_set_msg(v, "Doubling bound to %zu: array[%zu]=%d < %d",
					bound, bound, v->array[bound], v->value);
		viz_draw(v, (int)bound, CELL_PROBE);
		bound *= 2;
	}

	lo = bound / 2;
	hi = bound < v->size ? bound : v->size - 1;
	viz_set_msg(v, "Binary searching the window [%zu..%zu]", lo, hi);
	while (hi >= lo)
	{
		mid = lo + (hi - lo) / 2;
		viz_reset_states(v);
		viz_mark_window(v, lo, hi);
		viz_mark_eaten_outside(v, lo, hi);
		v->comparisons++;
		viz_set_msg(v, "Window [%zu..%zu]  mid=%zu  array[%zu]=%d",
					lo, hi, mid, mid, v->array[mid]);
		if (v->array[mid] == v->value)
		{
			viz_draw(v, (int)mid, CELL_FOUND);
			return ((int)mid);
		}
		viz_draw(v, (int)mid, CELL_PROBE);
		if (v->array[mid] > v->value)
		{
			if (mid == 0)
				break;
			hi = mid - 1;
		}
		else
			lo = mid + 1;
	}
	viz_set_msg(v, "Window collapsed without a match");
	viz_draw(v, -1, CELL_IDLE);
	return (-1);
}
