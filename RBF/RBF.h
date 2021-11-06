#ifndef RBF_H
#define RBF_H
#include <eigen3/Eigen/Core>
#include <eigen3/Eigen/Dense>
using namespace Eigen;

#define INPUT_NODE 6
#define HIDDEN_NODE 11
#define OUTPUT_NODE 9

#define DATASET_NUM 110

Matrix<double, DATASET_NUM, OUTPUT_NODE+1> Dataset;

class RBF
{
public:
    RBF();
    void KMeans();

    Matrix<double, DATASET_NUM, OUTPUT_NODE+1> Dataset;

private:
    Matrix<double, 1, INPUT_NODE> Input;
    Matrix<double, INPUT_NODE, HIDDEN_NODE> Center;
    Matrix<double, 1, INPUT_NODE> Norm_sqar;
    Matrix<double, INPUT_NODE, HIDDEN_NODE> Delta;
    Matrix<double, HIDDEN_NODE, OUTPUT_NODE> Weight;
};
#endif
// RBF_H