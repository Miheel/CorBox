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
	class MatX
	{

	public:
		MatX()
		{
			data.fill(T{0});
		}

		MatX(T initVal)
		{
			data.fill(initVal);
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
		constexpr cor::usize size() const { return data.size(); }
		constexpr bool empty() const { return data.empty(); }
		constexpr bool isSquare() const { return R == C; }

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
			static_assert((C == otherC && R == otherR) && "mat 1 and mat 2 must have equal # of col and row");
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
			static_assert((C == otherC && R == otherR) && "mat 1 and mat 2 must have equal # of col and row");

			MatX result;
			for (cor::usize i = 0; i < R * C; i++)
			{
				result[i] = data[i] - rhs[i];
			}
			return result;
		}

		template <typename U, cor::usize otherR, cor::usize otherC>
		MatX<T, R, otherC> operator*(const MatX<U, otherR, otherC> &rhs) const
		{
			static_assert((C == otherR) && "mat1 col # must be the same a mat2 row #");
			MatX<T, R, otherC> result;

			for (cor::usize _row = 0; _row < R; _row++)
			{
				for (cor::usize _col = 0; _col < otherC; _col++)
				{
					T sum{};
					for (cor::usize i = 0; i < C; i++)
					{
						// sum += data[_row * C + i] * rhs[i * otherC + _col];
						sum += this->operator()(_row, i) * rhs(i, _col);
					}
					// result[_row * otherC + _col] = sum;
					result(_row, _col) = sum;
				}
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

		MatX operator/(T scalar) const
		{
			MatX result;
			for (cor::usize i = 0; i < R * C; i++)
			{
				result[i] = data[i] / scalar;
			}
			return result;
		}

		bool operator==(const MatX &rhs) const
		{
			for (cor::usize i = 0; i < R * C; i++)
			{
				if (data[i] != rhs.data[i])
				{
					return false;
				}
			}
			return true;
		}

		bool operator!=(const MatX &rhs) const
		{
			return !(*this == rhs);
		}

		MatX<T, R - 1, C - 1> CofactorExp(cor::usize rowpos, cor::usize colpos) const
		{
			MatX<T, R - 1, C - 1> newmat;
			cor::usize j = 0;
			for (cor::usize row = 0; row < R; row++)
			{
				for (cor::usize col = 0; col < C; col++)
				{
					if (col != colpos && row != rowpos)
					{
						newmat[j++] = (*this)(row, col);
					}
				}
			}
			return newmat;
		}

		// determinant only works on square matricies where row == col
		template <cor::usize origR = R, cor::usize origC = C, typename = cor::EnableIf_T<(origR == origC)>>
		T det() const
		{
			static_assert((C == R) && "determinant only works on square matricies");
			if constexpr (R == 1)
			{
				return data[0];
			}
			if constexpr (R == 2)
			{
				return data[0] * data[3] - data[1] * data[2];
			}
			if constexpr (R == 3)
			{
				return (*this)(0, 0) * (*this)(1, 1) * (*this)(2, 2) +
					   (*this)(0, 1) * (*this)(1, 2) * (*this)(2, 0) +
					   (*this)(0, 2) * (*this)(1, 0) * (*this)(2, 1) -
					   (*this)(0, 2) * (*this)(1, 1) * (*this)(2, 0) -
					   (*this)(0, 1) * (*this)(1, 0) * (*this)(2, 2) -
					   (*this)(0, 0) * (*this)(1, 2) * (*this)(2, 1);
			}
			if constexpr (R > 3)
			{
				MatX<T, R - 1, C - 1> temp;
				T determinant = 0;
				int sign = 1;
				for (cor::usize i = 0; i < R; i++)
				{
					auto elem = (*this)(0, i);
					temp = CofactorExp(0, i);
					determinant += sign * elem * temp.det();
					sign = -sign;
				}
				return determinant;
			}
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

		bool isSymetric() const
		{
			return (this->transpose() == *this);
		}

		T trace() const
		{
			static_assert((C == R) && "trace only works on square matricies");
			T sum;
			for (cor::usize i = 0; i < R && i < C; i++)
			{
				sum += (*this)(i, i);
			}
			return sum;
		}

		void identity()
		{
			data.fill(T{0});
			for (cor::usize i = 0; i < R && i < C; i++)
			{
				this->operator()(i, i) = T{1};
			}
		}

		void zero()
		{
			data.fill(T{0});
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

	using Mat2 = MatX<double, 2, 2>;
	using Mat3 = MatX<double, 3, 3>;
	using Mat4 = MatX<double, 4, 4>;
	template <typename T, cor::usize C>
	using RowVec = MatX<T, 1, C>;
	template <typename T, cor::usize R>
	using Vector = MatX<T, R, 1>;

} // namespace cor

#endif // !MATRIX_HPP
