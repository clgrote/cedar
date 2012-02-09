/*======================================================================================================================

    Copyright 2011 Institut fuer Neuroinformatik, Ruhr-Universitaet Bochum, Germany

    This file is part of cedar.

    cedar is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License as published by the
    Free Software Foundation, either version 3 of the License, or (at your
    option) any later version.

    cedar is distributed in the hope that it will be useful, but WITHOUT ANY
    WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public
    License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with cedar. If not, see <http://www.gnu.org/licenses/>.

========================================================================================================================

    Institute:   Ruhr-Universitaet Bochum
                 Institut fuer Neuroinformatik

    File:        iiClosedFormInverseKinematics.cpp

    Maintainer:  Bjoern Weghenkel
    Email:       bjoern.weghenkel@ini.ruhr-uni-bochum.de
    Date:        2011 05 13

    Description: Inverse kinematics for the Cora arm.

    Credits: Ioannis Iossifidis

======================================================================================================================*/


#include <iostream>
#include "iiClosedFormInverseKinematics.hpp"


ClosedFormInverseKinematics::ClosedFormInverseKinematics()
{
  InitVariables();
}


ClosedFormInverseKinematics::~ClosedFormInverseKinematics()
{
	// TODO Auto-generated destructor stub
}


void ClosedFormInverseKinematics::InitVariables()
{
  mP_u = cv::Mat::zeros(3, 1, CV_64FC1);	// uperarm vector
  mP_f = cv::Mat::zeros(3, 1, CV_64FC1);	// forearm vector
  mP_h = cv::Mat::zeros(3, 1, CV_64FC1);	// hand vector
  mP_g = cv::Mat::zeros(3, 1, CV_64FC1);	// gripper vector
  mP_W = cv::Mat::zeros(3, 1, CV_64FC1);	// wrist vector
  mP_T = cv::Mat::zeros(3, 1, CV_64FC1);	// vector to the endeffector

  /* mJointAngle(0) denotes the trunk angle
   * and mJointAngle(1) to mJointAngle(8) the angles for the
   * remaining joints*/
  mJointAngle = cv::Mat::zeros(8, 1, CV_64FC1);

  /* This should replaced by an configFile in odrder to handle flexible different robotarm configurations*/
  mTrunk    = cv::Mat::zeros(3, 1, CV_64FC1);
  mShoulder = cv::Mat::zeros(3, 1, CV_64FC1);
  mUperArm  = cv::Mat::zeros(3, 1, CV_64FC1);
  mForeArm  = cv::Mat::zeros(3, 1, CV_64FC1);
  mEef      = cv::Mat::zeros(3, 1, CV_64FC1);

  mTrunk.at<double>(2, 0)    = 420.5;//400;
  mShoulder.at<double>(0, 0) = 225;
  mUperArm.at<double>(0, 0)	 = 325;
  mForeArm.at<double>(0, 0)	 = 310;
  mEef.at<double>(0, 0)	     = 342.2;//285;

  /*Setting taskcoordiantes for testing*/
  /* Task coordinates
   * Pos in 		[mm]
   * Angles in 	[deg]*/
  mTaskCoordinates.Pos = cv::Mat::zeros(3, 1, CV_64FC1);
  mTaskCoordinates.eefOrientationAngle = cv::Mat::zeros(3, 1, CV_64FC1);
}


void ClosedFormInverseKinematics::Rx(double rotAngle, cv::Mat* pRotMatX)
{

  (* pRotMatX ) = cv::Mat::zeros(3, 3, CV_64FC1);

  (* pRotMatX ).at<double>(0,0) = 1;
  (* pRotMatX ).at<double>(1,1) = cos(rotAngle);
  (* pRotMatX ).at<double>(1,2) = -sin(rotAngle);
  (* pRotMatX ).at<double>(2,1) = sin(rotAngle);
  (* pRotMatX ).at<double>(2,2) = cos(rotAngle);

  return;
}


void ClosedFormInverseKinematics::Ry(double rotAngle, cv::Mat* pRotMatY)
{

  (* pRotMatY ) = cv::Mat::zeros(3, 3, CV_64FC1);

  (* pRotMatY ).at<double>(0,0) = cos(rotAngle);
  (* pRotMatY ).at<double>(0,2) = sin(rotAngle);
  (* pRotMatY ).at<double>(1,1) = 1;
  (* pRotMatY ).at<double>(2,0) = -sin(rotAngle);
  (* pRotMatY ).at<double>(2,2) = cos(rotAngle);

  return;
}


void ClosedFormInverseKinematics::Rz(double rotAngle, cv::Mat* pRotMatZ)
{

  (*pRotMatZ) = cv::Mat::zeros(3, 3, CV_64FC1);


  (*pRotMatZ).at<double>(0,0) = cos(rotAngle);
  (*pRotMatZ).at<double>(0,1) = -sin(rotAngle);
  (*pRotMatZ).at<double>(1,0) = sin(rotAngle);
  (*pRotMatZ).at<double>(1,1) = cos(rotAngle);
  (*pRotMatZ).at<double>(2,2) = 1;

  return;
}


int ClosedFormInverseKinematics::TaskCoordinatesToArmGeometry()
{
	cv::Mat eefVector = cv::Mat::zeros(3,1,CV_64FC1);
	cv::Mat rotX = cv::Mat::zeros(3,3,CV_64FC1);
	cv::Mat rotX2 = cv::Mat::zeros(3,3,CV_64FC1);
	cv::Mat rotY = cv::Mat::zeros(3,3,CV_64FC1);
  cv::Mat rotY2 = cv::Mat::zeros(3,3,CV_64FC1);
	cv::Mat rotZ = cv::Mat::zeros(3,3,CV_64FC1);
	cv::Mat e_z_unit = cv::Mat::zeros(3,1,CV_64FC1);

  e_z_unit.at<double>(2, 0) = 1;

  double trunk_ang = CalcTrunkAng();

  /* assign mJointAngle(0) with the calculated trunk angle*/
  mJointAngle.at<double>(0, 0) = trunk_ang;

  double shoulder_gear = mTaskCoordinates.shoulderGear;

  /*calculate the endeffector position with respect to the trunk orientation
   * and the shouldergear.*/
  Rz(-trunk_ang,&rotZ);

  mCalculatedHandState.eefPos = rotZ * mTaskCoordinates.Pos;

  /* different offset
   * 1. offset generated by the shoulderGear
   * 2. offset generated by the trunk length*/
  Ry(-shoulder_gear,&rotY);
  // the following line prevents the next from crashing. i don't know why!
  mCalculatedHandState.eefPos = mCalculatedHandState.eefPos - (rotY * mShoulder) - mTrunk;

  Ry(shoulder_gear,&rotY);
  mCalculatedHandState.eefPos = rotY * mCalculatedHandState.eefPos;


  /* Endeffector  orientation
   * we do this due to our own convention of angles determine the
   * orientation of robots hand. It is not necessary for the overall solution*/
  //double phi_eef   = deg2rad(mTaskCoordinates.eefOrientationAngle.at<double>(0, 0)) - M_PI_2;
  //double theta_eef = deg2rad(mTaskCoordinates.eefOrientationAngle.at<double>(1, 0)) + M_PI;
  double phi_eef   = mTaskCoordinates.eefOrientationAngle.at<double>(0, 0) + M_PI;
  double theta_eef = mTaskCoordinates.eefOrientationAngle.at<double>(1, 0) + M_PI;

  /* From this section the calculation are related to the attached technical report*/

  // eef vector
  cv::Mat p_m = cv::Mat::zeros(3,1,CV_64FC1);// vector to middle of the redundancy circle
  //CvMAT p_w = CvMAT(3,1,CV_64FC1);// wrist vector

  mP_T = mCalculatedHandState.eefPos; // using the offset cleaned endeffctor postion

  /* calculate wrist position */
  Rz(phi_eef - trunk_ang,&rotZ);
  Ry(theta_eef,&rotY);
  Ry(shoulder_gear,&rotY2);

  /* wirst postion is calculated by subtracting the vector, defining the orinetation and length
   * of the hand */
  //mCalculatedHandState.wristPos = mCalculatedHandState.eefPos - rotY2 * rotZ * rotY * mEef;
  //mP_W =  mP_T - rotY2 * rotZ * rotY * mEef;
  mP_W =  mP_T -  rotZ * rotY * mEef;

  //printf(" mP_W.StdOutFormatted(); \n");
  //mP_W.StdOutFormatted();

  /* calculation the elbow position with respect to the redundancy angle*/
  double pw_norm_pow2 = pow(norm(mP_W, CV_L2), 2);//between shoulder and wrist
  double pu_norm_pow2	= pow(norm(mUperArm, CV_L2), 2);
  double pf_norm_pow2	= pow(norm(mForeArm, CV_L2), 2);

  //cout<<"mP_W Norm = " << mP_W.norm(CV_L2)<<endl;

  double auxiliary_variable =  (pu_norm_pow2 - pf_norm_pow2 + pw_norm_pow2)   / (2* pw_norm_pow2) ;
  double auxiliary_variable_2 =  (pu_norm_pow2 - pf_norm_pow2 + pw_norm_pow2)   / (2 * norm(mP_W, CV_L2)) ;

  p_m = mP_W * auxiliary_variable ;


  //double pm_norm_pow2 = pow(norm(p_m, CV_L2), 2);
  double redundancyCircleRadius 	= sqrt(  (pu_norm_pow2 - pow(auxiliary_variable_2 , 2)) );//page 16 IRINI

  //printf("Redundancy Radius = %f\n", redundancyCircleRadius);


  /* elbow position with respect to the redundancy angle alpha*/
  /* calculate the assosiated spherical angle of the wrist vector mP_W */
  double phi_W = atan2(mP_W.at<double>(2, 0), mP_W.at<double>(1, 0));
  double theta_W = acos(mP_W.at<double>(0, 0) / norm(mP_W, CV_L2));



  Rx(mTaskCoordinates.redundancyAng , &rotX); //first rotation
  Rz(theta_W , &rotY);	// secodn rotation
  Rx(phi_W   , &rotX2);	//third rotation


  mP_u =   (rotX2 * rotY * rotX * e_z_unit ) * redundancyCircleRadius + p_m;
  //printf("\n\nElbow = \n\n");
  //mP_u.StdOutFormatted();

  /* we calculate the gripper vector perpendicular to the
   * hand vector mP_h and the worlds z-achsis in order to
   * orient the gripper parallel to the table surface as default configuration*/
    mP_h = mP_T - mP_W;
  /* cross product didn't find now in the CvMAT*/

	mP_g.at<double>(0, 0) = mP_h.at<double>(1, 0) * e_z_unit.at<double>(2, 0) - mP_h.at<double>(2, 0) * e_z_unit.at<double>(1, 0);
	mP_g.at<double>(1, 0) = mP_h.at<double>(2, 0) * e_z_unit.at<double>(0, 0) - mP_h.at<double>(0, 0) * e_z_unit.at<double>(2, 0);
	mP_g.at<double>(2, 0) = mP_h.at<double>(0, 0) * e_z_unit.at<double>(1, 0) - mP_h.at<double>(1, 0) * e_z_unit.at<double>(0, 0);

	return(0);
}

double ClosedFormInverseKinematics::CalcTrunkAng()
{
	cv::Mat rotX = cv::Mat::zeros(3, 3, CV_64FC1);
	cv::Mat rotY = cv::Mat::zeros(3, 3, CV_64FC1);
	cv::Mat rotZ = cv::Mat::zeros(3, 3, CV_64FC1);

//	printf("CalcTrunkAng()\n\n");
	/* Endeffector  orientation
	 * we do this due to our own convention of angles determine the
	 * orientation of robots hand. It is not necessary for the overall solution*/
  //double phi_eef   = deg2rad(mTaskCoordinates.eefOrientationAngle.at<double>(0, 0)) - M_PI_2;
  //double theta_eef = deg2rad(mTaskCoordinates.eefOrientationAngle.at<double>(1, 0)) + M_PI;
  double phi_eef   = mTaskCoordinates.eefOrientationAngle.at<double>(0, 0) + M_PI;
  double theta_eef = mTaskCoordinates.eefOrientationAngle.at<double>(1, 0) + M_PI;

	Rz(phi_eef,&rotZ);
	Ry(theta_eef,&rotY);

	mP_W = mTaskCoordinates.Pos - rotZ * rotY * mEef;

	//double p_w_length = norm(mP_W, CV_L2);
	double phi_w = atan2(mP_W.at<double>(1, 0), mP_W.at<double>(0, 0));

	if( ( phi_w < deg2rad(-90 )) && (phi_w > deg2rad(-180) )  )
				phi_w = phi_w + deg2rad(360);

  //return(phi_w-deg2rad(90));
  return(phi_w);
}


void ClosedFormInverseKinematics::InverseKinematics()
{
	cv::Mat rotX = cv::Mat::zeros(3, 3, CV_64FC1);
	cv::Mat rotY = cv::Mat::zeros(3, 3, CV_64FC1);
	cv::Mat rotZ = cv::Mat::zeros(3, 3, CV_64FC1);

	TaskCoordinatesToArmGeometry();
	/*Calculate joint angle 1 and 2*/
	mJointAngle.at<double>(1, 0) = atan2(mP_u.at<double>(2, 0), mP_u.at<double>(1, 0));
	mJointAngle.at<double>(2, 0) = acos(mP_u.at<double>(0, 0) / mUperArm.at<double>(0, 0));

  //cout << "Angle 1 = " << mJointAngle.at<double>(1, 0) << endl;
  //cout << "Angle 2 = " << mJointAngle.at<double>(2, 0) << endl;

/* calculate wrist position */

	/* calculate forearm vector and joint angles 3 and 4*/
	mP_f  =  mP_W - mP_u;

	Rx(-mJointAngle.at<double>(1, 0), &rotX);
	Rz(-mJointAngle.at<double>(2, 0), &rotZ);

	cv::Mat TrafoElbowCS = rotZ * rotX;
	mP_f = TrafoElbowCS * mP_f;

	mJointAngle.at<double>(3, 0) = atan2(mP_f.at<double>(2, 0), mP_f.at<double>(1, 0));
	mJointAngle.at<double>(4, 0) = acos(mP_f.at<double>(0, 0) / mForeArm.at<double>(0, 0));

	//cout << "Angle 3 = " << mJointAngle.at<double>(3, 0) << endl;
	//cout << "Angle 4 = " << mJointAngle.at<double>(4, 0) << endl;

	/*calculation of the angles 5 and 6*/
	mP_h = mP_T - mP_W;


	Rx(-mJointAngle.at<double>(3, 0), &rotX);
	Rz(-mJointAngle.at<double>(4, 0), &rotZ);
	cv::Mat TrafoWristCS = rotZ * rotX;

	mP_h =  TrafoWristCS * TrafoElbowCS * mP_h;
	mJointAngle.at<double>(5, 0) = atan2(mP_h.at<double>(2, 0), mP_h.at<double>(1, 0));
	mJointAngle.at<double>(6, 0) = acos(mP_h.at<double>(0, 0) / mEef.at<double>(0, 0));

	//cout << "Angle 5 = " << mJointAngle.at<double>(5, 0) << endl;
	//cout << "Angle 6 = " << mJointAngle.at<double>(6, 0) << endl;

	/* calculation of the angle 7*/
	Rx(-mJointAngle.at<double>(5, 0), &rotX);
	Rz(-mJointAngle.at<double>(6, 0), &rotZ);
	cv::Mat TrafoEefCS = rotZ * rotX;

	mP_g =  TrafoEefCS * TrafoWristCS * TrafoElbowCS * mP_g;
	mJointAngle.at<double>(7, 0) = atan2(mP_g.at<double>(2, 0), mP_g.at<double>(1, 0));

	//cout << "Angle 7 = " << mJointAngle.at<double>(7, 0) << endl;
	return;
}


void ClosedFormInverseKinematics::printMatrix(cv::Mat& mat, const char* name)
{
  std::cout << name << ":" << std::endl;

  for(int i = 0; i < mat.size().height; ++i)
  {
    std::cout << "|\t";

    for(int j = 0; j < mat.size().width; ++j)
    {
      std::cout << mat.at<double>(i, j) << "\t";
    }

    std::cout << "|" << std::endl;
  }

  std::cout << std::endl;
  return;
}
