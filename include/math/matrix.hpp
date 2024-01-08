#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <iostream>
#include <initializer_list>
#include "static_array.hpp"
#include "type_traits.hpp"

namespace cor
{

	template <typename T, usize row, usize col>
	class MatX
	{

	public:
		MatX() = default;

		MatX(std::initializer_list<T> list)
		{
			data = list;
		}

		T &operator()(usize rowpos, usize colpos)
		{
			return data[rowpos * col + colpos];
		}
		const T &operator()(usize rowpos, usize colpos) const
		{
			return data[rowpos * col + colpos];
		}

		T &operator[](usize pos) { return data[pos]; }
		const T &operator[](usize pos) const { return data[pos]; }

		template <usize Lrow, usize Lcol>
		MatX operator+(MatX<T, Lrow, Lcol> &lhs)
		{
			assert((col == Lcol && row == Lrow) && "mat 1 and mat 2 must have equal # of col and row");
			for (usize i = 0; i < row * col; i++)
			{
				data[i] += lhs[i];
			}
			return *this;
		}

		template <usize Lrow, usize Lcol>
		MatX operator-(MatX<T, Lrow, Lcol> &lhs)
		{
			assert((col == Lcol && row == Lrow) && "mat 1 and mat 2 must have equal # of col and row");
			for (usize i = 0; i < row * col; i++)
			{
				data[i] -= lhs[i];
			}
			return *this;
		}

		MatX operator*(usize scalar)
		{
			for (usize i = 0; i < row * col; i++)
			{
				data[i] *= scalar;
			}
			return *this;
		}

		template <usize Lrow, usize Lcol>
		MatX<T, row, Lcol> operator*(MatX<T, Lrow, Lcol> &lhs)
		{
			assert((col == Lrow) && "mat 1 col must be the same a mat 2 row");
			MatX<T, row, Lcol> newmat;
			T sum = 0;
			for (usize _row = 0; _row < row; _row++)
			{
				for (usize _col = 0; _col < Lcol; _col++)
				{
					sum = 0;
					for (usize i = 0; i < col; i++)
					{
						sum += data[_row * col + i] * lhs[i * Lcol + _col];
					}
					newmat[_row * Lcol + _col] = sum;
				}
			}
			return newmat;
		}

		double det()
		{
			assert((col == row) && "determinant only works on quadratic matricies");
			if constexpr (row == 2)
			{
				return data[0] * data[3] - data[1] * data[2];
			}
			if constexpr (row == 3)
			{
				return (*this)(0, 0) * (*this)(1, 1) * (*this)(2, 2) +
					   (*this)(0, 1) * (*this)(1, 2) * (*this)(2, 0) +
					   (*this)(0, 2) * (*this)(1, 0) * (*this)(2, 1) -
					   (*this)(0, 2) * (*this)(1, 1) * (*this)(2, 0) -
					   (*this)(0, 1) * (*this)(1, 0) * (*this)(2, 2) -
					   (*this)(0, 0) * (*this)(1, 2) * (*this)(2, 1);
			}
			if constexpr (row > 3)
			{
				// do stuff
			}
		}

		MatX<T, col, row> Tanspose()
		{
			MatX<T, col, row> newmat;

			for (usize _row = 0; _row < row; _row++)
			{
				for (usize _col = 0; _col < col; _col++)
				{
					newmat[_col * row + _row] = (*this)(_row, _col);
				}
			}
			return newmat;
		}

		void print()
		{

			for (usize _row = 0; _row < row; _row++)
			{
				for (usize _col = 0; _col < col; _col++)
				{
					std::cout << data[_row * col + _col] << " ";
				}
				std::cout << "\n";
			}
		}

	private:
		cor::Static_Array<T, row * col> data;
	};

	using Mat2 = MatX<double, 2, 2>;
	using Mat3 = MatX<double, 3, 3>;
	template <typename T, usize size>
	using RowVec = MatX<T, 1, size>;
	template <typename T, usize size>
	using Vector = MatX<T, size, 1>;

	// template<class>
	// class Array;

	// using Array2D = cor::Array<cor::Array<float>>;

	// Array2D identityMatrix(usize N);

	// Array2D matrixAdd2D(Array2D& first, Array2D& second);

	// Array2D matrixSub2D(Array2D& first, Array2D& second);

	// Array2D matrixScalar2D(Array2D& first, int scalar);

	// Array2D matrixMultip2D(Array2D& first, Array2D& second);

	// Array2D matrixTransp2D(Array2D& first);

	// Array2D matrixinvers2D(Array2D& first);

	// float Det2x2(Array2D& first);
	// float Det3x3(Array2D& first);

	// Array2D matrixDet2D(Array2D& first);

}

#endif // !MATRIX_HPP
