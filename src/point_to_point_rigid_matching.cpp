#include "point_to_point_rigid_matching.h"
#include <igl/polar_svd.h>
#include "closest_rotation.h"
#include <Eigen/Dense>
#include <iostream>
void point_to_point_rigid_matching(
  const Eigen::MatrixXd & X,
  const Eigen::MatrixXd & P,
  Eigen::Matrix3d & R,
  Eigen::RowVector3d & t)
{
  // Replace with your code
  int k = X.rows();
  //R = Eigen::Matrix3d::Identity();
  ///t = Eigen::RowVector3d::Zero();
  Eigen::MatrixXd A = Eigen::MatrixXd::Zero(3 * k, 6);
  
  A.block(k,0, k,1) = -X.col(2);
  A.block(2*k,0, k,1) = X.col(1);
  A.block(0,1, k,1) = X.col(2);
  A.block(2*k,1, k,1) = -X.col(0);
  A.block(0,2, k,1) = -X.col(1);
  A.block(k,2, k,1) = X.col(0);
  A.block(0,3, k,1) = Eigen::MatrixXd::Ones(k,1);
  A.block(k,4, k,1) = Eigen::MatrixXd::Ones(k,1);
  A.block(k*2,5, k,1) = Eigen::MatrixXd::Ones(k,1);
  Eigen::MatrixXd B = Eigen::MatrixXd::Zero(3 * k, 1);
  B.block(0,0, k,1) = X.col(0) - P.col(0);
  B.block(k,0, k,1) = X.col(1) - P.col(1);
  B.block(2*k,0, k,1) = X.col(2) - P.col(2);
 //  std::cout << "X - P" << X.col(0) - P.col(0) << std::endl;
 //  std::cout << "X - P" << X.col(1) - P.col(1) << std::endl;
 // std::cout << "X - P" << X.col(2) - P.col(2) << std::endl;
  Eigen::Matrix<double, 6, 1> u;
  u = (A.transpose() * A).inverse() * (- A.transpose() * B);
  Eigen::Matrix3d M;
  M << 1 , -u(2,0), u(1,0),
       u(2,0), 1, -u(0,0),
       -u(1,0), u(0,0), 1;
  closest_rotation(M, R);
  t << u(3,0), u(4,0), u(5,0); 
}

