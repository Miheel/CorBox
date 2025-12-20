#include "matrix.hpp"

// cor::Array2D cor::matrixinvers2D(Array2D & matrix)
//{
//
// cor::Array2D newMatrix;
//	//needs to be general for all matricies
//	if (Det2x2(matrix) != 0)
//	{
//		newMatrix.resize(matrix.size());
//		for (usize i = 0; i < matrix.size(); i++)
//		{
//			newMatrix[i] = matrix[i];
//		}
//
//		for (usize col = 0; col < newMatrix.size(); col++)
//		{
//			usize pivot_row = col;
//			for (usize j = col + 1; j < newMatrix.size(); j++)
//			{
//				auto val1 = newMatrix[j][col], val2 = newMatrix[pivot_row][col];
//				if (val1 > val2)
//				{
//					pivot_row = j;
//				}
//			}
//
//
//			if (pivot_row != col)
//			{
//				newMatrix[col].swap(newMatrix[pivot_row]);
//			}
//			auto pivot = newMatrix[col][col];
//			for (usize i = col; i < newMatrix.size(); i++)
//			{
//				newMatrix[col][i] /= pivot;
//			}
//
//			for (usize row = col + 1; row < newMatrix.size(); row++)
//			{
//				auto ratio = newMatrix[row][col];
//				for (usize k = col; k < newMatrix.size(); k++)
//				{
//					newMatrix[row][k] -= newMatrix[col][k] * ratio;
//				}
//			}
//		}
//
//	}
//	return newMatrix;
// }

cor::Mat4 cor::translationMat4(double dx, double dy, double dz)
{
    return cor::MatX<double, 4, 4>{1, 0, 0, dx,
                                   0, 1, 0, dy,
                                   0, 0, 1, dz,
                                   0, 0, 0, 1};
}

cor::Mat4 cor::scalingMat4(double sx, double sy, double sz)
{
    return cor::Mat4{sx, 0, 0, 0,
                     0, sy, 0, 0,
                     0, 0, sz, 0,
                     0, 0, 0, 1};
}

/*** Rodrigues' rotation formula
 * Given an axis defined by the unit vector (x, y, z) and an angle θ in radians,
 */
cor::Mat4 cor::rotationMat4(double radians, double x, double y, double z)
{
    cor::Mat4 rotMat;
    if (x == 0 && y == 0 && z == 0)
    {
        rotMat.identity();
        return rotMat;
    }

    double c = std::cos(radians);
    double s = std::sin(radians);
    double t = 1 - c;

    // rodrigues' rotation formula for rotating a matrix around an arbitrary axis
    // given by the unit vector(x, y, z)
    return cor::Mat4{t * x * x + c, t * x * y - s * z, t * x * z + s * y, 0,
                     t * x * y + s * z, t * y * y + c, t * y * z - s * x, 0,
                     t * x * z - s * y, t * y * z + s * x, t * z * z + c, 0,
                     0, 0, 0, 1};
}