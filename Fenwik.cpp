#include <memory.h>
#include "Fenwik.h"

using namespace std;

// конструктор дерева Фенвика: на входе - исходный массив значений, по которому будут считаться частичные суммы
Fenwik::Fenwik(int src_size, int* src_array)
{
	// инициализация
	size = src_size;
	data = new int[size];
	memset(data, 0, size * sizeof(int));

	// формирование дерева (тривиальный вариант)
	for (int i = 0; i < size; ++i)
		modify_value(i, src_array[i]);
}

// дополнительный конструктор для копирования существующего дерева
Fenwik::Fenwik(Fenwik & src)
{
	size = src.size;
	data = new int[size];
	memcpy(data, src.data, size * sizeof(int));
}

// деструктор
Fenwik::~Fenwik()
{
	delete[] data;
}

// получить размер массива
int Fenwik::get_size()
{
	return size;
}

// обновление дерева в связи с изменением элемента исходного массива
void Fenwik::modify_value(int idx, int add_value)
{
	for (; idx < size; idx |= idx + 1)
		data[idx] += add_value;
}

// получение префиксной суммы, по элементам от 0 до "idx" (включительно)
int Fenwik::sum_prefix(int idx)
{
	int res = 0;

	for (; idx >= 0; idx = (idx & (idx + 1)) - 1)
		res += data[idx];

	return res;
}

// получение суммы элементов исходного массива с индексами от "idx_left" до "idx_right" (включительно)
__inline int Fenwik::sum_range(int idx_left, int idx_right)
{
	return (sum_prefix(idx_right) - sum_prefix(idx_left - 1));
}

// расчет значения элемента исходного массива, на основе данных дерева
__inline int Fenwik::compute_value(int idx)
{
	//return sum_range(idx, idx);
	return (idx % 2) ? sum_range(idx, idx) : data[idx];
}

// заполнение параметра-массива префиксными суммами для элементов от "idx_left" до "idx_right" (включительно)
int* Fenwik::sum_prefixes(int idx_left, int idx_right, int* ret)
{
	// инициализация результатов
	memset(ret, 0, (idx_right - idx_left + 1) * sizeof(int));

	// определение количества компонентных сумм для "idx_left"
	int temp[8 * sizeof(int) + 1];
	int i, idx, temp_idx, t;

	t = 1;
	idx = (idx_left & (idx_left + 1)) - 1;
		
	while (idx >= 0)
	{
		++t;
		idx = (idx & (idx + 1)) - 1;
	}

	// проход по компонентным суммам для "idx_left" спуском влево, заполнение массива "temp"
	temp[0] = 0;
	temp[t] = data[idx_left];
	temp_idx = t;

	for (idx = (idx_left & (idx_left + 1)) - 1; idx >= 0; idx = (idx & (idx + 1)) - 1)
		temp[--t] = data[idx];

	// преобразование массива "temp" в массив нарастающего итога по его исходным значениям
	for (i = 1; i <= temp_idx; ++i)
		temp[i] += temp[i - 1];

	// вычисление результатов
	ret[0] = temp[temp_idx];

	for (i = idx_left + 1; i <= idx_right; ++i)
	{
		// спуск по массиву "temp"
		// снимаем с массива "temp" (как со стека) столько значений, сколько единиц подряд идут в конце значения индекса "i"
		for (idx = i; (idx & 1); idx >>= 1)
			--temp_idx;

		// подъём в массиве "temp" и получение результата
		temp[temp_idx + 1] = temp[temp_idx] + data[i];
		ret[i - idx_left] = temp[++temp_idx];
	}

	return ret;
}

// изменение набора значений исходного массива данных с индексами от "idx_left" по "idx_right' (включительно)
// кстати, легко модифицируется под приём массива значений (int* add_values) для индивидуальной модификации каждого элемента
void Fenwik::modify_values(int idx_left, int idx_right, int add_value)
{
	int i, idx;
	int full_sum = (idx_right - idx_left + 1) * add_value;

	// обновление частичных сумм в диапазоне указанных индексов (и связанных с ними последующих)
	for (i = idx_left; i <= idx_right; ++i)
	{
		data[i] += add_value;
		idx = i | (i + 1);

		// проход по частичным суммам до окончания интервала idx_left..idx_right
		for (; idx <= idx_right; idx |= idx + 1)
			data[idx] += add_value;

		// проход по частичным суммам, пока не встретится на пути полная префиксная сумма
		for (; (idx < size) && (((idx & (idx + 1)) - 1) >= 0); idx |= idx + 1)
			data[idx] += add_value;
	}

	// обновление всех последующих полных префиксных сумм
	idx = idx_right | (idx_right + 1);

	// пропуск неполных частичных сумм после "idx_right" до момента, когда встретится на пути полная префиксная сумма
	while ((idx < size) && (((idx & (idx + 1)) - 1) >= 0))
		idx |= idx + 1;

	// проход по оставшимся полным префиксным суммам до конца
	for (; idx < size; idx |= idx + 1)
		data[idx] += full_sum;
}

// присвоение константного значения элементу исходного массива
void Fenwik::set_value(int idx, int new_value)
{
	modify_value(idx, new_value - compute_value(idx));
}

// присвоение константных значений элементам исходного массива в диапазоне индексов от "idx_left" до "idx_right" (включительно)
// кстати, легко модифицируется под приём массива разных константных значений (int* new_values) для индивидуального присвоения каждого элемента
void Fenwik::set_values(int idx_left, int idx_right, int new_value)
{
	int i, idx, sum_cur, cur_value, add_value;
	int full_sum = 0;
	int sum_prev = sum_prefix(idx_right);

	// обновление частичных сумм в диапазоне указанных индексов (и связанных с ними последующих)
	for (i = idx_right; i >= idx_left; --i)
	{
		sum_cur = sum_prev;
		sum_prev = sum_prefix(i - 1);

		cur_value = sum_cur - sum_prev;
		add_value = new_value - cur_value;

		full_sum += add_value;
		data[i] += add_value;

		idx = i | (i + 1);

		// проход по частичным суммам до окончания интервала idx_left..idx_right
		for (; idx <= idx_right; idx |= idx + 1)
			data[idx] += add_value;

		// проход по частичным суммам, пока не встретится на пути полная префиксная сумма
		for (; (idx < size) && ((idx & (idx + 1)) > 0); idx |= idx + 1)
			data[idx] += add_value;
	}

	// обновление всех последующих полных префиксных сумм
	idx = idx_right | (idx_right + 1);

	// пропуск неполных частичных сумм после "idx_right" до момента, когда встретится на пути полная префиксная сумма
	while ((idx < size) && ((idx & (idx + 1)) > 0))
		idx |= idx + 1;

	// проход по оставшимся полным префиксным суммам до конца
	for (; idx < size; idx |= idx + 1)
		data[idx] += full_sum;
}
