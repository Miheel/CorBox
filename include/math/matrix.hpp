#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <cmath>
#include <iostream>
#include <initializer_list>
#include "math.hpp"
#include "static_array.hpp"
#include "type_traits.hpp"

namespace cor
{
	template <typename T, cor::usize R, cor::usize C>
	class MatX;

	using Mat2 = MatX<double, 2, 2>;
	using Mat3 = MatX<double, 3, 3>;
	using Mat4 = MatX<double, 4, 4>;
	template <typename T, cor::usize C>
	using RowVec = MatX<T, 1, C>;
	template <typename T, cor::usize R>
	using Vector = MatX<T, R, 1>;

	template <typename T, cor::usize R, cor::usize C, cor::usize otherR, cor::usize otherC>
	void CofactorExp(cor::usize colpos, MatX<T, R, C> &origmat, MatX<T, otherR, otherC> &newmat);

	template <typename T, cor::usize R, cor::usize C>
	T det(MatX<T, R, C> &mat);

	template <typename T, cor::usize R, cor::usize C>
	class MatX
	{

	public:
		MatX()
		{
			data.fill(0);
		}

		MatX(std::initializer_list<T> list)
		{
			data = list;
		}

		MatX(const MatX &mat) : data(mat.data)
		{
		}

		MatX(MatX &&mat) noexcept
		{
			this->data.swap(mat.data);
		}

		T &operator()(cor::usize rowpos, cor::usize colpos)
		{
			return data[rowpos * C + colpos];
		}
		const T &operator()(cor::usize rowpos, cor::usize colpos) const
		{
			return data[rowpos * C + colpos];
		}

		T &operator[](cor::usize pos) { return data[pos]; }
		const T &operator[](cor::usize pos) const { return data[pos]; }

		constexpr cor::usize rows() const { return R; }
		constexpr cor::usize cols() const { return C; }

		MatX &operator=(const MatX &rhs)
		{
			MatX temp(rhs);
			this->data.swap(temp.data);
			return *this;
		}
		MatX &operator=(MatX &&rhs) noexcept
		{
			this->data.swap(rhs.data);
			return *this;
		}

		template <cor::usize otherR, cor::usize otherC>
		MatX operator+(const MatX<T, otherR, otherC> &rhs) const
		{
			assert((C == otherC && R == otherR) && "mat 1 and mat 2 must have equal # of col and row");
			MatX result;
			for (cor::usize i = 0; i < R * C; i++)
			{
				result[i] = data[i] + rhs[i];
			}
			return result;
		}

		template <cor::usize otherR, cor::usize otherC>
		MatX operator-(const MatX<T, otherR, otherC> &rhs) const
		{
			assert((C == otherC && R == otherR) && "mat 1 and mat 2 must have equal # of col and row");

			MatX result;
			for (cor::usize i = 0; i < R * C; i++)
			{
				result[i] = data[i] - rhs[i];
			}
			return result;
		}

		MatX operator*(T scalar) const
		{
			MatX result;
			for (cor::usize i = 0; i < R * C; i++)
			{
				result[i] = data[i] * scalar;
			}
			return result;
		}

		template <typename U, cor::usize otherR, cor::usize otherC>
		MatX<T, R, otherC> operator*(const MatX<U, otherR, otherC> &rhs) const
		{
			assert((C == otherR) && "mat1 col must be the same a mat2 row");
			MatX<T, R, otherC> newmat;
			T sum = 0;
			for (cor::usize _row = 0; _row < R; _row++)
			{
				for (cor::usize _col = 0; _col < otherC; _col++)
				{
					sum = 0;
					for (cor::usize i = 0; i < C; i++)
					{
						// sum += data[_row * C + i] * rhs[i * otherC + _col];
						sum += this->operator()(_row, i) * rhs(i, _col);
					}
					// newmat[_row * otherC + _col] = sum;
					newmat(_row, _col) = sum;
				}
			}
			return newmat;
		}

		// determinant only works on square matricies where row == col
		template <cor::usize origR = R, cor::usize origC = C, typename = cor::EnableIf_T<(origR == origC)>>
		T det()
		{
			assert((C == R) && "determinant only works on square matricies");
			return cor::det(*this);
		}

		MatX<T, C, R> transpose() const
		{
			MatX<T, C, R> newmat;

			for (cor::usize _row = 0; _row < R; _row++)
			{
				for (cor::usize _col = 0; _col < C; _col++)
				{
					newmat(_col, _row) = this->operator()(_row, _col);
				}
			}
			return newmat;
		}

		void zero()
		{
			data.fill(0);
		}

		void print()
		{

			for (cor::usize _row = 0; _row < R; _row++)
			{
				for (cor::usize _col = 0; _col < C; _col++)
				{
					std::cout << this->operator()(_row, _col) << " ";
				}
				std::cout << "\n";
			}
		}

	private:
		cor::Static_Array<T, R * C> data;
	};

	template <typename T, cor::usize R, cor::usize C, cor::usize otherR, cor::usize otherC>
	void CofactorExp(cor::usize colpos, MatX<T, R, C> &origmat, MatX<T, otherR, otherC> &newmat)
	{
		int j = 0;
		for (cor::usize row = 1; row < R; row++)
		{
			for (cor::usize col = 0; col < C; col++)
			{
				if (col != colpos)
				{
					newmat[j++] = origmat(row, col);
				}
			}
		}
	}

	template <typename T, cor::usize R, cor::usize C>
	T det(MatX<T, R, C> &mat)
	{
		assert((C == R) && "determinant only works on square matricies");
		if constexpr (R == 2)
		{
			return mat[0] * mat[3] - mat[1] * mat[2];
		}
		if constexpr (R == 3)
		{
			return mat(0, 0) * mat(1, 1) * mat(2, 2) +
				   mat(0, 1) * mat(1, 2) * mat(2, 0) +
				   mat(0, 2) * mat(1, 0) * mat(2, 1) -
				   mat(0, 2) * mat(1, 1) * mat(2, 0) -
				   mat(0, 1) * mat(1, 0) * mat(2, 2) -
				   mat(0, 0) * mat(1, 2) * mat(2, 1);
		}
		if constexpr (R > 3)
		{
			MatX<T, R - 1, C - 1> temp;
			T determinant = 0;
			int sign = 1;
			for (cor::usize i = 0; i < R; i++)
			{
				if (R >= 4)
				{
					auto elem = mat(0, i);
					CofactorExp(i, mat, temp);
					determinant += sign * elem * det(temp);
					sign = -sign;
				}
			}
			return determinant;
		}
	}

	template <typename T, cor::usize R, cor::usize C>
	T EuclidVecNorm(const MatX<T, R, C> &vec)
	{
		T sum = 0;
		for (cor::usize i = 0; i < R * C; i++)
		{
			sum += vec[i] * vec[i];
		}
		return std::sqrt(sum);
	}

	template <typename T, cor::usize R, cor::usize C>
	bool operator==(const MatX<T, R, C> &lhs, const MatX<T, R, C> &rhs)
	{
		for (cor::usize i = 0; i < R * C; i++)
		{
			if (lhs[i] != rhs[i])
			{
				return false;
			}
		}
		return true;
	}

	template <typename T, cor::usize C>
	RowVec<T, C> operator/(const RowVec<T, C> &vec, double mag)
	{
		RowVec<T, C> normvec;
		for (cor::usize i = 0; i < C; i++)
		{
			normvec[i] = vec[i] / mag;
		}
		return normvec;
	}

	template <typename T, cor::usize C>
	RowVec<T, C> operator/(double mag, const RowVec<T, C> &vec)
	{
		return vec / mag;
	}

	template <typename T, cor::usize R>
	Vector<T, R> operator/(const Vector<T, R> &vec, double mag)
	{
		Vector<T, R> normvec;
		for (cor::usize i = 0; i < R; i++)
		{
			normvec[i] = vec[i] / mag;
		}
		return normvec;
	}

	template <typename T, cor::usize R>
	Vector<T, R> operator/(double mag, const Vector<T, R> &vec)
	{
		return vec / mag;
	}

	// TOODO: implement eigenvalues and eigenvectors
	//  hint: power iteration method
	//  template <typename T, cor::usize R, cor::usize C>
	//  MatX<double, R, C> eigen(MatX<double, R, C> &vec)
	//  {
	//  	MatX<double, R, C> eigenvec;
	//  	eigenvec.zero();

	// 	eigenvec = vec * eigenvec;
	// }
}

#endif // !MATRIX_HPP
