#include <memory.h>
#include "Fenwik.h"

using namespace std;

// ����������� ������ �������: �� ����� - �������� ������ ��������, �� �������� ����� ��������� ��������� �����
Fenwik::Fenwik(int src_size, int* src_array)
{
	// �������������
	size = src_size;
	data = new int[size];
	memset(data, 0, size * sizeof(int));

	// ������������ ������ (����������� �������)
	for (int i = 0; i < size; ++i)
		modify_value(i, src_array[i]);
}

// �������������� ����������� ��� ����������� ������������� ������
Fenwik::Fenwik(Fenwik & src)
{
	size = src.size;
	data = new int[size];
	memcpy(data, src.data, size * sizeof(int));
}

// ����������
Fenwik::~Fenwik()
{
	delete[] data;
}

// �������� ������ �������
int Fenwik::get_size()
{
	return size;
}

// ���������� ������ � ����� � ���������� �������� ��������� �������
void Fenwik::modify_value(int idx, int add_value)
{
	for (; idx < size; idx |= idx + 1)
		data[idx] += add_value;
}

// ��������� ���������� �����, �� ��������� �� 0 �� "idx" (������������)
int Fenwik::sum_prefix(int idx)
{
	int res = 0;

	for (; idx >= 0; idx = (idx & (idx + 1)) - 1)
		res += data[idx];

	return res;
}

// ��������� ����� ��������� ��������� ������� � ��������� �� "idx_left" �� "idx_right" (������������)
__inline int Fenwik::sum_range(int idx_left, int idx_right)
{
	return (sum_prefix(idx_right) - sum_prefix(idx_left - 1));
}

// ������ �������� �������� ��������� �������, �� ������ ������ ������
__inline int Fenwik::compute_value(int idx)
{
	return sum_range(idx, idx);
}

// ���������� ���������-������� ����������� ������� ��� ��������� �� "idx_left" �� "idx_right" (������������)
int* Fenwik::sum_prefixes(int idx_left, int idx_right, int* ret)
{
	// ������������� �����������
	memset(ret, 0, (idx_right - idx_left + 1) * sizeof(int));

	// ����������� ���������� ������������ ���� ��� "idx_left"
	int temp[8 * sizeof(int) + 1];
	int i, idx, temp_idx, t;

	t = 1;
	idx = (idx_left & (idx_left + 1)) - 1;
		
	while (idx >= 0)
	{
		++t;
		idx = (idx & (idx + 1)) - 1;
	}

	// ������ �� ������������ ������ ��� "idx_left" ������� �����, ���������� ������� "temp"
	temp[0] = 0;
	temp[t] = data[idx_left];
	temp_idx = t;

	for (idx = (idx_left & (idx_left + 1)) - 1; idx >= 0; idx = (idx & (idx + 1)) - 1)
		temp[--t] = data[idx];

	// �������������� ������� "temp" � ������ ������������ ����� �� ��� �������� ���������
	for (i = 1; i <= temp_idx; ++i)
		temp[i] += temp[i - 1];

	// ���������� �����������
	ret[0] = temp[temp_idx];

	for (i = idx_left + 1; i <= idx_right; ++i)
	{
		// ����� �� ������� "temp"
		// ������� � ������� "temp" (��� �� �����) ������� ��������, ������� ������ ������ ���� � ����� �������� ������� "i"
		for (idx = i; (idx & 1); idx >>= 1)
			--temp_idx;

		// ������ � ������� "temp" � ��������� ����������
		temp[temp_idx + 1] = temp[temp_idx] + data[i];
		ret[i - idx_left] = temp[++temp_idx];
	}

	return ret;
}

// ��������� ������ �������� ��������� ������� ������ � ��������� �� "idx_left" �� "idx_right' (������������)
// ������, ����� �������������� ��� ���� ������� �������� (int* add_values) ��� �������������� ����������� ������� ��������
void Fenwik::modify_values(int idx_left, int idx_right, int add_value)
{
	int i, idx;
	int full_sum = (idx_right - idx_left + 1) * add_value;

	// ���������� ��������� ���� � ��������� ��������� �������� (� ��������� � ���� �����������)
	for (i = idx_left; i <= idx_right; ++i)
	{
		data[i] += add_value;
		idx = i | (i + 1);

		// ������ �� ��������� ������ �� ��������� ��������� idx_left..idx_right
		for (; idx <= idx_right; idx |= idx + 1)
			data[idx] += add_value;

		// ������ �� ��������� ������, ���� �� ���������� �� ���� ������ ���������� �����
		for (; (idx < size) && (((idx & (idx + 1)) - 1) >= 0); idx |= idx + 1)
			data[idx] += add_value;
	}

	// ���������� ���� ����������� ������ ���������� ����
	idx = idx_right | (idx_right + 1);

	// ������� �������� ��������� ���� ����� "idx_right" �� �������, ����� ���������� �� ���� ������ ���������� �����
	while ((idx < size) && (((idx & (idx + 1)) - 1) >= 0))
		idx |= idx + 1;

	// ������ �� ���������� ������ ���������� ������ �� �����
	for (; idx < size; idx |= idx + 1)
		data[idx] += full_sum;
}

// ���������� ������������ �������� �������� ��������� �������
void Fenwik::set_value(int idx, int new_value)
{
	modify_value(idx, new_value - compute_value(idx));
}

// ���������� ����������� �������� ��������� ��������� ������� � ��������� �������� �� "idx_left" �� "idx_right" (������������)
// ������, ����� �������������� ��� ���� ������� ������ ����������� �������� (int* new_values) ��� ��������������� ���������� ������� ��������
void Fenwik::set_values(int idx_left, int idx_right, int new_value)
{
	int i, idx, sum_cur, cur_value, add_value;
	int full_sum = 0;
	int sum_prev = sum_prefix(idx_right);

	// ���������� ��������� ���� � ��������� ��������� �������� (� ��������� � ���� �����������)
	for (i = idx_right; i >= idx_left; --i)
	{
		sum_cur = sum_prev;
		sum_prev = sum_prefix(i - 1);

		cur_value = sum_cur - sum_prev;
		add_value = new_value - cur_value;

		full_sum += add_value;
		data[i] += add_value;

		idx = i | (i + 1);

		// ������ �� ��������� ������ �� ��������� ��������� idx_left..idx_right
		for (; idx <= idx_right; idx |= idx + 1)
			data[idx] += add_value;

		// ������ �� ��������� ������, ���� �� ���������� �� ���� ������ ���������� �����
		for (; (idx < size) && ((idx & (idx + 1)) > 0); idx |= idx + 1)
			data[idx] += add_value;
	}

	// ���������� ���� ����������� ������ ���������� ����
	idx = idx_right | (idx_right + 1);

	// ������� �������� ��������� ���� ����� "idx_right" �� �������, ����� ���������� �� ���� ������ ���������� �����
	while ((idx < size) && ((idx & (idx + 1)) > 0))
		idx |= idx + 1;

	// ������ �� ���������� ������ ���������� ������ �� �����
	for (; idx < size; idx |= idx + 1)
		data[idx] += full_sum;
}
