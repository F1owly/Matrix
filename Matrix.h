#pragma once
#include <initializer_list>

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
			m_ptr = new double[rows];
			m_rows = rows;
			m_columns = 1;
		}

		Matrix(int rows, int cols) {
			m_ptr = new double[static_cast<long long>(rows) * cols];
			m_rows = rows;
			m_columns = cols;
		}

		Matrix(const Matrix& m1) {
			m_ptr = m1.m_ptr;
			m_rows = m1.m_rows;
			m_columns = m1.m_columns;
		}

		Matrix(Matrix&& moved) {
			m_ptr = moved.m_ptr;
			m_rows = moved.m_rows;
			m_columns = moved.m_columns;
		}

		Matrix(const std::initializer_list<double>& list) : Matrix(list.size()) {
			int i = 0;
			for (auto el : list) {
				m_ptr[i] = el;
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


	};
}