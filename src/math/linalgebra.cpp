#include "linalgebra.hpp"

cor::Mat4 cor::translationMat4(double dx, double dy, double dz)
{
    return cor::Mat4{1, 0, 0, dx,
                     0, 1, 0, dy,
                     0, 0, 1, dz,
                     0, 0, 0, 1};
}

cor::Mat4 cor::translationMat4(const cor::Vector<double, 3> &vec)
{
    return cor::translationMat4(vec[0], vec[1], vec[2]);
}

cor::Mat4 cor::scalingMat4(double sx, double sy, double sz)
{
    return cor::Mat4{sx, 0, 0, 0,
                     0, sy, 0, 0,
                     0, 0, sz, 0,
                     0, 0, 0, 1};
}

cor::Mat4 cor::scalingMat4(const cor::Vector<double, 3> &vec)
{
    return cor::scalingMat4(vec[0], vec[1], vec[2]);
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

cor::Mat4 cor::rotationMat4(double radians, const cor::Vector<double, 3> &vec)
{
    return cor::rotationMat4(radians, vec[0], vec[1], vec[2]);
}

cor::Mat4 cor::shearMat4(cor::pair shx, cor::pair shy, cor::pair shz)
{
    return cor::Mat4{1, shx.first, shx.second, 0,
                     shy.first, 1, shy.second, 0,
                     shz.first, shz.second, 1, 0,
                     0, 0, 0, 1};
}

cor::Mat4 cor::inverseMat4(const cor::Mat4 &mat)
{

    return cor::Mat4();
}
