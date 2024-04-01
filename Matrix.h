#pragma once
#include <initializer_list>
#include <iomanip>
#include <math.h>
#include <stdexcept>
#define EPS 0.00000001
namespace linalg {
	class Matrix {
	private:
		double* m_ptr;
		int m_rows;
		int m_columns;
	public:
		Matrix() {
			m_rows = 0;
			m_columns = 0;
			m_ptr = nullptr;
		}

		Matrix(int rows) {
			if (rows < 0) {
				throw std::runtime_error("Rows at least zero!");
			}
			m_ptr = new double[rows];
			m_rows = rows;
			m_columns = 1;

		}

		Matrix(int rows, int cols) {
			if (rows < 0) {
				throw std::runtime_error("Rows at least zero!");
			}
			if (cols < 0) {
				throw std::runtime_error("Cols at least zero!");
			}
			m_ptr = new double[static_cast<long long>(rows) * cols];
			m_rows = rows;
			m_columns = cols;
		}

		Matrix(const Matrix& m1) {
			m_ptr = m1.m_ptr;
			m_rows = m1.m_rows;
			m_columns = m1.m_columns;
		}

		Matrix(Matrix&& moved) noexcept {
			m_ptr = moved.m_ptr;
			m_rows = moved.m_rows;
			m_columns = moved.m_columns;
		}

		Matrix(const std::initializer_list<double>& list) : Matrix(list.size()) {
			int i = 0;
			for (auto el : list) {
				m_ptr[i++] = el;
			}
		}

		Matrix(const std::initializer_list<const std::initializer_list<double>>& list) {
			m_rows = list.size();
			m_columns = 1;
			if (list.begin() != list.end()) {
				m_columns = (*list.begin()).size();
				m_ptr = new double[static_cast<long long>(m_rows) * m_columns];
			}
			else {
				delete[] m_ptr;
				throw std::runtime_error("Can`t create matrix! Empty initializer_list!");
			}

			int i = 0;
			int cols = 0;
			for (auto el_list : list) {
				for (auto el : el_list) {
					if (cols >= m_columns)
						throw std::runtime_error("Can`t create matrix! Invalid initializer_list!");
					m_ptr[i++] = el;
					cols++;
				}
				cols = 0;
			}
		}

		int rows() const {
			return m_rows;
		}

		int columns() const {
			return m_columns;
		}

		bool empty() const {
			if (m_rows == 0 || m_columns == 0) {
				return true;
			}
			return false;
		}

		void reshape(int rows, int cols) {
			if (rows * cols == m_rows * m_columns) {
				m_rows = rows;
				m_columns = cols;
			}
			else {
				throw "Can`t reshape matrix!";
			}
		}

		Matrix& operator=(const Matrix& m1) {
			if (m_rows * m_columns > 0) {
				delete[] m_ptr;
			}

			m_rows = m1.m_rows;
			m_columns = m1.m_columns;
			m_ptr = new double[static_cast<long long>(m_rows) * m_columns];

			for (int i = 0; i < m_rows * m_columns; i++) {
				m_ptr[i] = m1.m_ptr[i];
			}

			return *this;
		}

		Matrix& operator=(Matrix&& m1) noexcept {
			if (m_rows * m_columns > 0) {
				delete[] m_ptr;
			}

			m_rows = m1.m_rows;
			m_columns = m1.m_columns;
			m_ptr = m1.m_ptr;

			return *this;
		}

		double& operator()(int i, int j) {
			if (0 <= i && i < m_rows && 0 <= j && j < m_columns) {
				return m_ptr[i * m_columns + j];
			}
			else {
				delete[]m_ptr;
				throw std::runtime_error("double& operator()(int i, int j); //Index out of range.");
			}
		}

		const double& operator()(int i, int j) const {
			if (0 <= i && i < m_rows && 0 <= j && j < m_columns) {
				return m_ptr[i * m_columns + j];
			}
			else {
				delete[]m_ptr;
				throw std::runtime_error("const double& operator()(int i, int j) const; //Index out of range.");
			}
		}

		friend std::ostream& operator << (std::ostream& os, const Matrix& m)
		{
			double maxd = 0;
			for (int i = 0; i < m.m_columns * m.m_rows; i++) {
				if (maxd < m.m_ptr[i]) {
					maxd = m.m_ptr[i];
				}
			}
			int maxl = 0;
			int maxi = static_cast<int>(maxd);
			while (maxi > 0) {
				maxi /= 10;
				maxl++;
			}
			for (int i = 0; i < m.m_columns * m.m_rows; i++) {
				if (i % m.m_columns == 0) {
					os << "|";
				}
				if (abs(m.m_ptr[i]) > EPS) {
					os << std::setw(maxl + 4) << std::setprecision(3) << m.m_ptr[i];
				}
				else {
					os << std::setw(maxl + 4) << std::setprecision(3) << 0;

				}
				if ((i + 1) % m.m_columns == 0) {
					os << "|";
					if (i + 1 < m.m_columns * m.m_rows) {
						os << '\n';
					}
				}
				else os << " ";

			}
			return os;
		}

		Matrix operator+(const Matrix& m) const {
			if (m_rows != m.m_rows || m_columns != m.m_columns) {
				throw std::runtime_error("The matrices must be of the same size!");
			}

			Matrix ans(m_rows, m_columns);
			for (int i = 0; i < m_columns * m_rows; i++) {
				ans.m_ptr[i] = m_ptr[i] + m.m_ptr[i];
			}
			return ans;
		}

		Matrix& operator+=(const Matrix& m) {
			if (m_rows != m.m_rows || m_columns != m.m_columns) {
				throw std::runtime_error("The matrices must be of the same size!");
			}

			for (int i = 0; i < m_columns * m_rows; i++) {
				m_ptr[i] += m.m_ptr[i];
			}
			return *this;
		}

		Matrix operator-(const Matrix& m) const {
			if (m_rows != m.m_rows || m_columns != m.m_columns) {
				throw std::runtime_error("The matrices must be of the same size!");
			}

			Matrix ans(m_rows, m_columns);
			for (int i = 0; i < m_columns * m_rows; i++) {
				ans.m_ptr[i] = m_ptr[i] - m.m_ptr[i];
			}
			return ans;
		}

		Matrix& operator-=(const Matrix& m) {
			if (m_rows != m.m_rows || m_columns != m.m_columns) {
				throw std::runtime_error("The matrices must be of the same size!");
			}

			for (int i = 0; i < m_columns * m_rows; i++) {
				m_ptr[i] -= m.m_ptr[i];
			}
			return *this;
		}

		Matrix operator*(const Matrix& m) const {
			if (m_columns != m.m_rows) {
				throw std::runtime_error("The matrices are not compatible");
			}

			Matrix ans(m_rows, m.m_columns);
			for (int i = 0; i < m_rows; i++) {
				for (int j = 0; j < m.m_columns; j++) {
					ans(i, j) = 0;
					for (int k = 0; k < m_columns; k++) {
						ans(i, j) += m(k, j) * m_ptr[i * m_rows + k];

					}
				}
			}
			return ans;
		}

		friend Matrix& operator*(double x, const Matrix& m) {
			Matrix ans(m.m_rows, m.m_columns);
			for (int i = 0; i < m.m_columns * m.m_rows; i++) {
				ans.m_ptr[i] = m.m_ptr[i] * x;
			}

			return ans;
		}

		friend Matrix& operator*(const Matrix& m, double x) {
			Matrix ans(m.m_rows, m.m_columns);
			for (int i = 0; i < m.m_columns * m.m_rows; i++) {
				ans.m_ptr[i] = m.m_ptr[i] * x;
			}

			return ans;
		}

		Matrix& operator*=(const Matrix& m) {
			*this = Matrix(*this * m);
			return *this;
		}

		Matrix& operator*=(double x) {
			for (int i = 0; i < m_columns * m_rows; i++) {
				m_ptr[i] *= x;
			}
			return *this;
		}

		double trace() const {
			if (m_rows != m_columns) {
				throw std::runtime_error("Matrix is not square!");
			}

			int trace = 0;
			for (int i = 0; i < m_rows; i++) {
				trace += (*this)(i, i);
			}
			return trace;
		}

		double det() const {
			if (m_rows != m_columns) {
				throw std::runtime_error("Matrix is not square!");
			}

			Matrix ans(m_rows, m_columns);
			for (int i = 0; i < m_rows * m_columns; i++) {
				ans.m_ptr[i] = m_ptr[i];
			}
			double det = 1;
			//(i, i_) - ��� ���������� �������������� �������� ��������
			for (int i = 0, i_ = 0; i < m_rows && i_ < m_columns; i++, i_++) {
				int j = i; // j - ������, � ����������� ��������� � ������� i_
				for (int k = i + 1; k < m_rows; k++) {
					if (abs(ans(k, i_)) > abs(ans(j, i_))) {
						j = k;
					}
				}

				//���� ��� �������� � ������� i_ �������, ��������� �� ��������� �������
				if (abs(ans(j, i_)) < EPS) {
					i--;
					continue;
				}

				if (j != i) {
					det *= -1;
				}
				//������ ������ � ������������ ��������� �� ����� ������ i
				for (int k = 0; k < m_columns; k++) {
					double swapper = ans(i, k);
					ans(i, k) = ans(j, k);
					ans(j, k) = swapper;
				}

				//�������� �� ���� �����, ��� ���� ������ i, ��� ������, ���������� �� ��������� ���, ����� ��� �������� i_��� ������� c���� ����� 0
				for (int h = i + 1; h < m_rows; h++) {
					double coef = ans(h, i_) / ans(i, i_);
					for (int k = 0; k < m_columns; k++) {
						ans(h, k) -= ans(i, k) * coef;
					}
				}
			}


			for (int i = 0; i < m_rows; i++) {
				det *= ans(i, i);
			}

			return det;
		}

		Matrix& gauss_forward() {
			Matrix ans(std::move(*this));
			//(i, i_) - ��� ���������� �������������� �������� ��������
			for (int i = 0, i_ = 0; i < m_rows && i_ < m_columns; i++, i_++) {
				int j = i; // j - ������, � ����������� ��������� � ������� i_
				for (int k = i + 1; k < m_rows; k++) {
					if (abs(ans(k, i_)) > abs(ans(j, i_))) {
						j = k;
					}
				}
				//���� ��� �������� � ������� i_ �������, ��������� �� ��������� �������
				if (abs(ans(j, i_)) < EPS) {
					i--;
					continue;
				}
				//������ ������ � ������������ ��������� �� ����� ������ i
				for (int k = 0; k < m_columns; k++) {
					double swapper = ans(i, k);
					ans(i, k) = ans(j, k);
					ans(j, k) = swapper;
				}
				//�������� �� ���� �����, ��� ���� ������ i, ��� ������, ���������� �� ��������� ���, ����� ��� �������� i_��� ������� c���� ����� 0
				for (int h = i + 1; h < m_rows; h++) {
					double coef = ans(h, i_) / ans(i, i_);
					for (int k = 0; k < m_columns; k++) {
						ans(h, k) -= ans(i, k) * coef;
					}
				}
			}
			return *this;
		}

		Matrix& gauss_back() {
			Matrix ans(std::move(*this));
			{
				//�������� �� �� ��� ������� �����������
				int j_based = -1;
				bool zeros_row = true;

				for (int i = 0; i < m_rows; i++) {
					for (int j = 0; j < m_columns; j++) {
						if (abs(ans(i, j)) > EPS) {
							if (j_based > j) {
								throw std::runtime_error("Matrix is not step view!");
							}
							j_based = j;
							zeros_row = false;
							break;
						}
					}

					if (zeros_row) {
						continue;
					}

					for (int k = i + 1; k < m_rows; k++) {
						if (abs(ans(k, j_based)) > EPS) {
							throw std::runtime_error("Matrix is not step view!");
						}
					}

					zeros_row = true;
				}
			}
			int n = -1;
			for (int i = m_rows - 1; i >= 0; i--) {
				for (int j = 0; j < m_columns; j++) {
					if (abs(ans(i, j)) > EPS) {
						n = i;
						i = -1;
						break;
					}
				}
			}

			if (n == -1) {
				return *this;
			}

			for (int i = n; i >= 0; i--) {
				int j_based = -1;
				for (int j = 0; j < m_columns; j++) {
					if (abs(ans(i, j)) > EPS) {
						j_based = j;
						break;
					}
				}
				if (j_based == -1) {
					continue;
				}

				for (int k = 0; k < i; k++) {
					double coef = ans(k, j_based) / ans(i, j_based);
					for (int h = 0; h < m_columns; h++) {
						ans(k, h) -= coef * ans(i, h);
					}
				}
			}
			return *this;
		}

		int rank() const {
			Matrix ans = *this;

			ans.gauss_forward();
			ans.gauss_back();

			int rank = 0;
			for (int i = 0; i < m_rows; i++) {
				for (int j = 0; j < m_columns; j++) {
					if (abs(ans(i, j)) > EPS) {
						rank++;
						break;
					}
				}
			}

			return rank;
		}

		double norm() const {
			int sum2 = 0;
			for (int i = 0; i < static_cast<long long>(m_rows * m_columns); i++) {
				sum2 += m_ptr[i] * m_ptr[i];
			}
			return std::sqrt(sum2);
		}


	};
}

	linalg::Matrix concatenate(const linalg::Matrix& m1, const linalg::Matrix& m2) {//c�������� �� �����������
		if (m1.rows() != m2.rows()) {
			throw std::runtime_error("The matrices have a different number of rows!");
		}

		linalg::Matrix ans(m1.rows(), m1.columns() + m2.columns());

		for (int i = 0; i < m1.rows(); i++) {
			for (int j = 0; j < m1.columns(); j++) {
				ans(i, j) = m1(i, j);
			}
			for (int j = 0; j < m2.columns(); j++) {
				ans(i, m1.columns() + j) = m2(i, j);
			}
		}
		return ans;
	}
