#ifndef LINALGEBRA_HPP
#define LINALGEBRA_HPP

#include "matrix.hpp"

namespace cor
{

    struct pair
    {
        double first;
        double second;
    };

    template <typename T, cor::usize R>
    struct EigenResult
    {
        double eigenvalue;
        cor::Vector<T, R> eigenvector;
    };

    cor::Mat4 translationMat4(double dx, double dy, double dz);
    cor::Mat4 translationMat4(const cor::Vector<double, 3> &vec);
    cor::Mat4 scalingMat4(double sx, double sy, double sz);
    cor::Mat4 scalingMat4(const cor::Vector<double, 3> &vec);
    cor::Mat4 rotationMat4(double radians, double x, double y, double z);
    cor::Mat4 rotationMat4(double radians, const cor::Vector<double, 3> &vec);
    cor::Mat4 shearMat4(pair shx, pair shy, pair shz);
    cor::Mat4 inverseMat4(const cor::Mat4 &mat);
    // transformationmatrix * vector4x1

    struct Transform
    {
        cor::Mat4 transformMat;

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

        Transform &shear(cor::pair shx, cor::pair shy, cor::pair shz)
        {
            transformMat = cor::shearMat4(shx, shy, shz) * transformMat;
            return *this;
        }

        cor::Mat4 getTransformMat() const
        {
            return transformMat;
        }
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

    template <typename T, cor::usize R, cor::usize C>
    T det(const MatX<T, R, C> &mat)
    {
        assert((C == R) && "determinant only works on square matricies");
        return mat.det();
    }

    template <typename T, cor::usize R, cor::usize C>
    cor::MatX<T, R, C> adjugate(const cor::MatX<T, R, C> &mat)
    {
        if constexpr (R == 1)
        {
            return MatX<T, R, C>({1});
        }
        if constexpr (R == 2)
        {
            return cor::MatX<T, R, C>({mat[3], -mat[1], -mat[2], mat[0]});
        }
        else
        {
            cor::MatX<T, R, C> adjugateMat;
            for (cor::usize row = 0; row < R; row++)
            {
                for (cor::usize col = 0; col < C; col++)
                {
                    auto cofactor = mat.CofactorExp(row, col);
                    int sign = (row + col) % 2 == 0 ? 1 : -1;
                    adjugateMat(col, row) = sign * cofactor.det();
                }
            }
            return adjugateMat;
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
    EigenResult<T, R> eigen(const MatX<T, R, C> &mat)
    {
        static_assert(R == C, "Eigen decomposition only works on square matricies");
        EigenResult<T, R> result;
        Vector<T, R> eigenvec(T{5});                        // initial guess
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
        return lhs == rhs;
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

#endif // !LINALGEBRA_HPP
