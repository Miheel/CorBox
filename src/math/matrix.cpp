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
