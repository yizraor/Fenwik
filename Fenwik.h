#pragma once

// ���������� ���������� ������ �������.
// ��������� �������� (� ��� �������������� �����������) ��������� ��� �������� ����
class Fenwik
{
protected:
	int size;   // ���-�� ��������� �������� ������
	int* data;  // ������ ������ �������
public:
	// ����������� ������ �������: �� ����� - �������� ������ ��������, �� �������� ����� ��������� ��������� �����
	Fenwik(int src_size, int* src_array);
	// �������������� ����������� ��� ����������� ������������� ������
	Fenwik(Fenwik & src);
	// ����������
	~Fenwik();

	// �������� ������ �������
	int get_size();

	// ��������� ���������� �����, �� ��������� �� 0 �� "idx" (������������)
	int sum_prefix(int idx);

	// ��������� ����� ��������� ��������� ������� � ��������� �� "idx_left" �� "idx_right" (������������)
	int sum_range(int idx_left, int idx_right);

	// ������ �������� �������� ��������� �������, �� ������ ������ ������
	int compute_value(int idx);

	// ���������� ���������-������� ����������� ������� ��� ��������� �� "idx_left" �� "idx_right" (������������)
	int* sum_prefixes(int idx_left, int idx_right, int* ret);

	// ���������� ������ � ����� � ���������� �������� ��������� �������
	void modify_value(int idx, int add_value);

	// ��������� ������ �������� ��������� ������� ������ � ��������� �� "idx_left" �� "idx_right' (������������)
	// ������, ���������� ����� �������������� ��� ���� ������� �������� (int* add_values) ��� �������������� ����������� ������� ��������
	void modify_values(int idx_left, int idx_right, int add_value);

	// ���������� ������������ �������� �������� ��������� �������
	void set_value(int idx, int new_value);

	// ���������� ����������� �������� ��������� ��������� ������� � ��������� �������� �� "idx_left" �� "idx_right" (������������)
	// ������, ���������� ����� �������������� ��� ���� ������� ������ ����������� �������� (int* new_values) ��� ��������������� ���������� ������� ��������
	void set_values(int idx_left, int idx_right, int new_value);
};
