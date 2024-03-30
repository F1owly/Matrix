#pragma once
#include <initializer_list>
#include <iomanip>
#include <stdexcept>

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
				std::runtime_error("Rows at least zero!");
			}
			m_ptr = new double[rows];
			m_rows = rows;
			m_columns = 1;

		}

		Matrix(int rows, int cols) {
			if (rows < 0) {
				std::runtime_error("Rows at least zero!");
			}
			if (cols < 0) {
				std::runtime_error("Cols at least zero!");
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
				throw "Can`t create matrix! Empty initializer_list!";
			}

			int i = 0;
			for (auto el_list : list) {
				for (auto el : el_list) {
					m_ptr[i++] = el;
				}
			}
		}

		int rows() const{
			return m_rows;
		}

		int columns() const{
			return m_columns;
		}

		bool empty() const{
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

		double& operator()(int i, int j){
			if(0 <= i && i <m_rows && 0 <= j && j < m_columns){
				return m_ptr[i * m_rows + j];
			}
			else {
				delete[]m_ptr;
				throw "double& operator()(int i, int j); //Index out of range.";
			}
		}
		
		const double& operator()(int i, int j) const {
			if (0 <= i && i < m_rows && 0 <= j && j < m_columns) {
				return m_ptr[i * m_rows + j];
			}
			else {
				delete[]m_ptr;
				throw "const double& operator()(int i, int j) const; //Index out of range.";
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
			for (int i = 0; i < m.m_columns*m.m_rows;i++) {
				if (i % m.m_columns == 0) {
					os << "|";
				}
				os << std::setw(maxl) << std::setprecision(3) << m.m_ptr[i];
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

		Matrix operator+(const Matrix& m) const{
			if (m_rows != m.m_rows || m_columns != m.m_columns) {
				std::runtime_error("The matrices must be of the same size!");
			}

			Matrix ans(m_rows, m_columns);
			for (int i = 0; i < m_columns * m_rows; i++) {
				ans.m_ptr[i] = m_ptr[i] + m.m_ptr[i];
			}
			return ans;
		}

		Matrix& operator+=(const Matrix& m) {
			if (m_rows != m.m_rows || m_columns != m.m_columns) {
				std::runtime_error("The matrices must be of the same size!");
			}

			for (int i = 0; i < m_columns * m_rows; i++) {
				m_ptr[i] += m.m_ptr[i];
			}
			return *this;
		}

		Matrix operator-(const Matrix& m) const {
			if (m_rows != m.m_rows || m_columns != m.m_columns) {
				std::runtime_error("The matrices must be of the same size!");
			}

			Matrix ans(m_rows, m_columns);
			for (int i = 0; i < m_columns * m_rows; i++) {
				ans.m_ptr[i] = m_ptr[i] - m.m_ptr[i];
			}
			return ans;
		}

		Matrix& operator-=(const Matrix& m) {
			if (m_rows != m.m_rows || m_columns != m.m_columns) {
				std::runtime_error("The matrices must be of the same size!");
			}

			for (int i = 0; i < m_columns * m_rows; i++) {
				m_ptr[i] -= m.m_ptr[i];
			}
			return *this;
		}

		Matrix operator*(const Matrix& m) const{
			if (m_columns != m.m_rows) {
				std::runtime_error("The matrices are not compatible");
			}

			Matrix ans(m_rows, m.m_columns);
			int s = 0;
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
				std::cout << i << " = " << m.m_ptr[i] << " * " << x << " = " << m.m_ptr[i] * x << std::endl;

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
	};
	
}