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

	template <typename T, cor::usize R>
	struct EigenResult
	{
		double eigenvalue;
		cor::Vector<T, R> eigenvector;
	};

	struct pair
	{
		double first;
		double second;
	};

	struct Transform
	{
		Mat4 transformMat;

		Transform()
		{
			transformMat.identity();
		}

		Transform &translate(double dx, double dy, double dz)
		{
			transformMat = cor::translationMat4(dx, dy, dz) * transformMat;
			return *this;
		}

		Transform &scale(double sx, double sy, double sz)
		{
			transformMat = cor::scalingMat4(sx, sy, sz) * transformMat;
			return *this;
		}

		Transform &rotate(double radians, double x, double y, double z)
		{
			transformMat = cor::rotationMat4(radians, x, y, z) * transformMat;
			return *this;
		}

		Transform &shear(pair shx, pair shy, pair shz)
		{
			transformMat = cor::shearMat4(shx, shy, shz) * transformMat;
			return *this;
		}

		cor::Mat4 getTransformMat() const
		{
			return transformMat;
		}
	};

	Mat4 translationMat4(double dx, double dy, double dz);
	Mat4 translationMat4(const cor::Vector<double, 3> &vec);
	Mat4 scalingMat4(double sx, double sy, double sz);
	Mat4 scalingMat4(const cor::Vector<double, 3> &vec);
	Mat4 rotationMat4(double radians, double x, double y, double z);
	Mat4 rotationMat4(double radians, const cor::Vector<double, 3> &vec);
	Mat4 shearMat4(pair shx, pair shy, pair shz);
	Mat4 inverseMat4(const Mat4 &mat);
	// transformationmatrix * vector4x1

	template <typename T, cor::usize R, cor::usize C, cor::usize otherR, cor::usize otherC>
	void CofactorExp(cor::usize colpos, const MatX<T, R, C> &origmat, MatX<T, otherR, otherC> &newmat);

	template <typename T, cor::usize R>
	bool residual(const cor::Vector<T, R> &vec);

	template <typename T, cor::usize R, cor::usize C>
	T det(const MatX<T, R, C> &mat);

	template <typename T, cor::usize R, cor::usize C>
	double EuclidVecNorm(const MatX<T, R, C> &vec);

	template <typename T, cor::usize R, cor::usize C>
	EigenResult<T, R> eigen(const MatX<T, R, C> &mat);

	template <typename T, cor::usize R, cor::usize C>
	MatX<T, R, C> operator*(const T scalar, const MatX<T, R, C> &mat);

	template <typename T, cor::usize R, cor::usize C>
	bool operator==(const MatX<T, R, C> &lhs, const MatX<T, R, C> &rhs);

	template <typename T, cor::usize R, cor::usize C>
	bool operator!=(const MatX<T, R, C> &lhs, const MatX<T, R, C> &rhs);

	template <typename T, cor::usize C>
	RowVec<T, C> operator/(const RowVec<T, C> &vec, double scalar);

	template <typename T, cor::usize R>
	Vector<T, R> operator/(const Vector<T, R> &vec, double scalar);

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

		// determinant only works on square matricies where row == col
		template <cor::usize origR = R, cor::usize origC = C, typename = cor::EnableIf_T<(origR == origC)>>
		T det() const
		{
			static_assert((C == R) && "determinant only works on square matricies");
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

	template <typename T, cor::usize R, cor::usize C, cor::usize otherR, cor::usize otherC>
	void CofactorExp(cor::usize colpos, const MatX<T, R, C> &origmat, MatX<T, otherR, otherC> &newmat)
	{
		cor::usize j = 0;
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

	template <typename T, cor::usize R>
	bool residual(const cor::Vector<T, R> &vec)
	{
		double epsilon = 1e-9; // Define a small threshold for floating-point comparison
		double magnitude = cor::EuclidVecNorm(vec);
		if (magnitude < epsilon)
			return true;
		return false;
	}

	template <typename T, cor::usize R, cor::usize C>
	T det(const MatX<T, R, C> &mat)
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
				auto elem = mat(0, i);
				CofactorExp(i, mat, temp);
				determinant += sign * elem * det(temp);
				sign = -sign;
			}
			return determinant;
		}
	}

	template <typename T, cor::usize R, cor::usize C>
	double EuclidVecNorm(const MatX<T, R, C> &vec)
	{
		double sum = 0;
		for (cor::usize i = 0; i < R * C; i++)
		{
			sum += vec[i] * vec[i];
		}
		return std::sqrt(sum);
	}

	template <typename T, cor::usize R, cor::usize C>
	EigenResult<T, R> eigen(const MatX<T, R, C> &mat)
	{
		static_assert(R == C, "Eigen decomposition only works on square matricies");
		EigenResult<T, R> result;
		Vector<T, R> eigenvec(T{5});						// initial guess
		eigenvec = eigenvec / cor::EuclidVecNorm(eigenvec); // normalize
		double eigenval = 0, norm = 0, convergence = 0;
		double tol = 1e-9;
		cor::usize maxIterations = 1000;

		for (cor::usize i = 0; i < maxIterations; i++)
		{
			auto newvec = mat * eigenvec;
			norm = cor::EuclidVecNorm(newvec);
			// normalize the new vector to get the next eigenvector approximation
			eigenvec = newvec / norm;

			// Rayleigh quotient
			// since eigenvec is normalized, denominator is 1
			//			  eigenvec_T * mat * eigenvec
			// eigenval = -----------------------------
			//  			eigenvec_T * eigenvec <-- this is 1
			auto recomputed = mat * eigenvec;
			eigenval = static_cast<double>((eigenvec.transpose() * recomputed)[0]);

			// calculate convergence
			// instead of checking the eigenvalue difference abs(eigenval - prev_eigenval)
			// check the norm of (A*v - λ*v)
			convergence = cor::EuclidVecNorm(recomputed - (eigenval * eigenvec));
			if (convergence < tol)
			{
				break;
			}

			std::cout << "Iteration " << i << ": Eigenvalue = " << eigenval << "\n";
		}

		// return the eigenvalue and eigenvector(eigenvector is normalized)
		result.eigenvector = eigenvec;
		result.eigenvalue = eigenval;
		return result;
	}

	template <typename T, cor::usize R, cor::usize C>
	MatX<T, R, C> operator*(const T scalar, const MatX<T, R, C> &mat)
	{
		return mat * scalar;
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

	template <typename T, cor::usize R, cor::usize C>
	bool operator!=(const MatX<T, R, C> &lhs, const MatX<T, R, C> &rhs)
	{
		return !(lhs == rhs);
	}

	template <typename T, cor::usize C>
	RowVec<T, C> operator/(const RowVec<T, C> &vec, double scalar)
	{
		RowVec<T, C> normvec;
		for (cor::usize i = 0; i < C; i++)
		{
			normvec[i] = vec[i] / scalar;
		}
		return normvec;
	}

	template <typename T, cor::usize R>
	Vector<T, R> operator/(const Vector<T, R> &vec, double scalar)
	{
		Vector<T, R> normvec;
		for (cor::usize i = 0; i < R; i++)
		{
			normvec[i] = vec[i] / scalar;
		}
		return normvec;
	}

} // namespace cor

#endif // !MATRIX_HPP
