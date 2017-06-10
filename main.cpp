#include <iostream>
#include <Windows.h>
#include "Fenwik.h"

using namespace std;

// тестирование производительности метода, получающего массив префиксных сумм
void test_sum_prefixes(Fenwik & obj, int* src, int idx_left, int idx_right, int passes)
{
	int* triv_ret = new int[idx_right - idx_left + 1];
	int* opt_ret = new int[idx_right - idx_left + 1];

	LARGE_INTEGER freq;
	LARGE_INTEGER time_1, time_2, time_3;
	LONGLONG diff_1, diff_2;

	QueryPerformanceFrequency(&freq);
	QueryPerformanceCounter(&time_1);

	for (int k = 0; k < passes; ++k)
		for (int i = idx_left; i <= idx_right; ++i)
			triv_ret[i - idx_left] = obj.sum_prefix(i);

	QueryPerformanceCounter(&time_2);

	for (int k = 0; k < passes; ++k)
		obj.sum_prefixes(idx_left, idx_right, opt_ret);

	QueryPerformanceCounter(&time_3);
	diff_1 = (time_2.QuadPart - time_1.QuadPart) * 1000 / freq.QuadPart;
	diff_2 = (time_3.QuadPart - time_2.QuadPart) * 1000 / freq.QuadPart;

	cout << "TEST_SUM_PREFIXES:  trivial = " << diff_1 << " ms;  optimized = " << diff_2 << " ms" << endl;

	int* check = new int[idx_right - idx_left + 1];
	memset(check, 0, (idx_right - idx_left + 1) * sizeof(int));

	for (int i = idx_left; i <= idx_right; ++i)
		for (int j = 0; j <= i; ++j)
			check[i - idx_left] += src[j];

	if (0 != memcmp(check, triv_ret, (idx_right - idx_left + 1) * sizeof(int)))
		throw "TEST_SUM_PREFIXES: validation error (1)";

	if (0 != memcmp(check, opt_ret, (idx_right - idx_left + 1) * sizeof(int)))
		throw "TEST_SUM_PREFIXES: validation error (2)";

	delete[] triv_ret;
	delete[] opt_ret;
	delete[] check;
}

// тестирование производительности метода, модифицирующего значения исходных данных (src[i] += add_value)
void test_modify_values(Fenwik & obj, int* src, int idx_left, int idx_right, int add_value, int passes)
{
	int* triv_ret = new int[obj.get_size()];
	int* opt_ret = new int[obj.get_size()];

	Fenwik triv_obj(obj);
	Fenwik opt_obj(obj);

	for (int i = idx_left; i <= idx_right; ++i)
		triv_obj.modify_value(i, add_value);
	opt_obj.modify_values(idx_left, idx_right, add_value);

	triv_obj.sum_prefixes(0, obj.get_size() - 1, triv_ret);
	opt_obj.sum_prefixes(0, obj.get_size() - 1, opt_ret);

	LARGE_INTEGER freq;
	LARGE_INTEGER time_1, time_2, time_3;
	LONGLONG diff_1, diff_2;

	QueryPerformanceFrequency(&freq);
	QueryPerformanceCounter(&time_1);

	for (int k = 0; k < passes; ++k)
		for (int i = idx_left; i <= idx_right; ++i)
			triv_obj.modify_value(i, add_value);

	QueryPerformanceCounter(&time_2);

	for (int k = 0; k < passes; ++k)
		opt_obj.modify_values(idx_left, idx_right, add_value);

	QueryPerformanceCounter(&time_3);
	diff_1 = (time_2.QuadPart - time_1.QuadPart) * 1000 / freq.QuadPart;
	diff_2 = (time_3.QuadPart - time_2.QuadPart) * 1000 / freq.QuadPart;

	cout << "TEST_MODIFY_VALUES:  trivial = " << diff_1 << " ms;  optimized = " << diff_2 << " ms" << endl;

	int* check_src = new int[obj.get_size()];
	int* check = new int[obj.get_size()];

	memcpy(check_src, src, obj.get_size() * sizeof(int));
	memset(check, 0, obj.get_size() * sizeof(int));

	for (int i = idx_left; i <= idx_right; ++i)
		check_src[i] += add_value;

	for (int g_sum = 0, i = 0; i < obj.get_size(); ++i)
		check[i] = (g_sum += check_src[i]);

	if (0 != memcmp(check, triv_ret, obj.get_size() * sizeof(int)))
		throw "TEST_MODIFY_VALUES:  validation error (1)";

	if (0 != memcmp(check, opt_ret, obj.get_size() * sizeof(int)))
		throw "TEST_MODIFY_VALUES:  validation error (2)";

	delete[] triv_ret;
	delete[] opt_ret;
	delete[] check_src;
	delete[] check;
}

// тестирование производительности метода, устанавливающего значения исходных данных (src[i] = add_value)
void test_set_values(Fenwik & obj, int* src, int idx_left, int idx_right, int new_value, int passes)
{
	int* triv_ret = new int[obj.get_size()];
	int* opt_ret = new int[obj.get_size()];

	Fenwik triv_obj(obj);
	Fenwik opt_obj(obj);

	for (int i = idx_left; i <= idx_right; ++i)
		triv_obj.set_value(i, new_value);
	opt_obj.set_values(idx_left, idx_right, new_value);

	triv_obj.sum_prefixes(0, obj.get_size() - 1, triv_ret);
	opt_obj.sum_prefixes(0, obj.get_size() - 1, opt_ret);

	LARGE_INTEGER freq;
	LARGE_INTEGER time_1, time_2, time_3;
	LONGLONG diff_1, diff_2;

	QueryPerformanceFrequency(&freq);
	QueryPerformanceCounter(&time_1);

	for (int k = 0; k < passes; ++k)
		for (int i = idx_left; i <= idx_right; ++i)
			triv_obj.set_value(i, new_value);

	QueryPerformanceCounter(&time_2);

	for (int k = 0; k < passes; ++k)
		opt_obj.set_values(idx_left, idx_right, new_value);

	QueryPerformanceCounter(&time_3);
	diff_1 = (time_2.QuadPart - time_1.QuadPart) * 1000 / freq.QuadPart;
	diff_2 = (time_3.QuadPart - time_2.QuadPart) * 1000 / freq.QuadPart;

	cout << "TEST_SET_VALUES:  trivial = " << diff_1 << " ms;  optimized = " << diff_2 << " ms" << endl;

	int* check_src = new int[obj.get_size()];
	int* check = new int[obj.get_size()];

	memcpy(check_src, src, obj.get_size() * sizeof(int));
	memset(check, 0, obj.get_size() * sizeof(int));

	for (int i = idx_left; i <= idx_right; ++i)
		check_src[i] = new_value;

	for (int g_sum = 0, i = 0; i < obj.get_size(); ++i)
		check[i] = (g_sum += check_src[i]);

	if (0 != memcmp(check, triv_ret, obj.get_size() * sizeof(int)))
		throw "TEST_SET_VALUES:  validation error (1)";

	if (0 != memcmp(check, opt_ret, obj.get_size() * sizeof(int)))
		throw "TEST_SET_VALUES:  validation error (2)";

	delete[] triv_ret;
	delete[] opt_ret;
	delete[] check_src;
	delete[] check;
}


// главная функция: генерация случайных данных, выполнение тестов
void main(void)
{
	srand(GetTickCount());

	int size = 127870;
	int* src = new int[size];

	for (int i = 0; i < size; ++i)
		src[i] = rand() % 67;

	Fenwik obj(size, src);

#ifdef _DEBUG
#define ITERS_MULTIPLIER 1
#else
#define ITERS_MULTIPLIER 10
#endif

	try
	{
		test_sum_prefixes(obj, src, 0, 5, 2000000 * ITERS_MULTIPLIER);
		test_sum_prefixes(obj, src, 7, 57, 200000 * ITERS_MULTIPLIER);
		test_sum_prefixes(obj, src, 120000, 120050, 200000 * ITERS_MULTIPLIER);
		test_sum_prefixes(obj, src, 14, 514, 20000 * ITERS_MULTIPLIER);
		test_sum_prefixes(obj, src, 7500, 10000, 4000 * ITERS_MULTIPLIER);
		//test_sum_prefixes(obj, src, 0, size - 1, 100 * ITERS_MULTIPLIER);

		cout << endl;

		test_modify_values(obj, src, 0, 5, 11, 500000 * ITERS_MULTIPLIER);
		test_modify_values(obj, src, 7, 57, 12, 100000 * ITERS_MULTIPLIER);
		test_modify_values(obj, src, 120000, 120050, 13, 100000 * ITERS_MULTIPLIER);
		test_modify_values(obj, src, 14, 514, 14, 10000 * ITERS_MULTIPLIER);
		test_modify_values(obj, src, 7500, 10000, 15, 2500 * ITERS_MULTIPLIER);
		//test_modify_values(obj, src, 0, size - 1, 1, 100 * ITERS_MULTIPLIER);

		cout << endl;

		test_set_values(obj, src, 0, 5, 11, 400000 * ITERS_MULTIPLIER);
		test_set_values(obj, src, 7, 57, 12, 50000 * ITERS_MULTIPLIER);
		test_set_values(obj, src, 120000, 120050, 13, 50000 * ITERS_MULTIPLIER);
		test_set_values(obj, src, 14, 514, 14, 5000 * ITERS_MULTIPLIER);
		test_set_values(obj, src, 7500, 10000, 15, 1000 * ITERS_MULTIPLIER);
		//test_set_values(obj, src, 0, size - 1, 1, 100 * ITERS_MULTIPLIER);
	}
	catch (char* e)
	{
		cout << endl << "TEST FAILED!" << endl;
		cout << "details: " << e << endl;
	}

	delete[] src;
	cout << endl << "(press any key)" << endl;
	getchar();
}
