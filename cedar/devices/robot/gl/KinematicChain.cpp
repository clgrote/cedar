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

 ----- Institute:   Ruhr-Universitaet Bochum
                    Institut fuer Neuroinformatik
 
 ----- File:        KinematicChain.cpp
 
 ----- Maintainer:  Hendrik Reimann
 ----- Email:       hendrik.reimann@ini.rub.de
 ----- Date:        2010 11 06
 
 ----- Description: Implementation of the \em cedar::dev::robot::gl::KinematicChain class.
 
 ----- Credits:
 ---------------------------------------------------------------------------------------------------------------------*/

#define NOMINMAX // avoids Windows issues with cedar's min/max functions

// CEDAR INCLUDES
#include "cedar/devices/robot/gl/KinematicChain.h"
#include "cedar/auxiliaries/gl/drawShapes.h"
#include "cedar/auxiliaries/math/tools.h"
#include "cedar/auxiliaries/gl/gl.h"

// SYSTEM INCLUDES

//----------------------------------------------------------------------------------------------------------------------
// constructors and destructor
//----------------------------------------------------------------------------------------------------------------------

cedar::dev::robot::gl::KinematicChain::KinematicChain(cedar::dev::robot::KinematicChainModelPtr& rpKinematicChainModel)
:
cedar::aux::gl::Object(rpKinematicChainModel),
mpKinematicChainModel(rpKinematicChainModel)
{

}

cedar::dev::robot::gl::KinematicChain::~KinematicChain()
{
  
}

//----------------------------------------------------------------------------------------------------------------------
// methods
//----------------------------------------------------------------------------------------------------------------------

void cedar::dev::robot::gl::KinematicChain::draw()
{
  prepareDraw();

  if (mIsVisible)
  {
    drawBase();
    for (unsigned int j = 0; j < mpKinematicChainModel->getNumberOfJoints(); j++)
    {
      drawSegment(j);
    }
    drawEndEffector();
    drawEndEffectorVelocity();
    drawEndEffectorAcceleration();

    glPopMatrix(); //!\todo check if this is needed
  }
}

void cedar::dev::robot::gl::KinematicChain::drawBase()
{
  // move to origin
  glPopMatrix();
  glPushMatrix();

  // draw the link to the first joint
  glColor4d(mColorR/2, mColorG/2, mColorB/2, 0);
  cv::Mat proximal = mpKinematicChainModel->getTransformation()(cv::Rect(3, 0, 1, 3)).clone();
  cv::Mat distal = mpKinematicChainModel->getJointTransformation(0)(cv::Rect(3, 0, 1, 3)).clone();
  cedar::aux::gl::drawCone<double>(proximal, distal, .035, .035, mResolution, mIsDrawnAsWireFrame);

  // move to object coordinates
  mTransformationTranspose = mpKinematicChainModel->getTransformation().t();
  glMultMatrixd((GLdouble*)mTransformationTranspose.data);

  // draw the base
  glColor4d(mColorR, mColorG, mColorB, 0);
  cedar::aux::gl::drawTorus(0.1, 0.015, mResolution, mResolution, mIsDrawnAsWireFrame);
  glColor4d(mColorR/2, mColorG/2, mColorB/2, 0);
  glTranslatef(0.0f, 0.0f, 0.005f);
  cedar::aux::gl::drawDisk(0.0, 0.1, mResolution, mResolution, false, mIsDrawnAsWireFrame);
  glTranslatef(0.0f, 0.0f, -0.01f);
  cedar::aux::gl::drawDisk(0.0, 0.1, mResolution, mResolution, true, mIsDrawnAsWireFrame);
}

void cedar::dev::robot::gl::KinematicChain::drawSegment(unsigned int index)
{
  // move to origin transformation and resave it to the stack
  glPopMatrix();
  glPushMatrix();
  
  // move to object coordinates
  mTransformationTranspose = mpKinematicChainModel->getJointTransformation(index).t();
  glMultMatrixd((GLdouble*)mTransformationTranspose.data);
  
  // draw the joint
  glColor4d(mColorR, mColorG, mColorB, 0);
  cedar::aux::gl::drawSphere(.05, mResolution, mResolution, mIsDrawnAsWireFrame);
  
  // move to origin transformation and re-save it to the stack
  glPopMatrix();
  glPushMatrix();

  // draw the link
  glColor4d(mColorR/2, mColorG/2, mColorB/2, 0);
  cv::Mat proximal = mpKinematicChainModel->getJointTransformation(index)(cv::Rect(3, 0, 1, 3)).clone();
  cv::Mat distal;
  if (index+1 < mpKinematicChainModel->getNumberOfJoints())
  {
    distal = mpKinematicChainModel->getJointTransformation(index+1)(cv::Rect(3, 0, 1, 3)).clone();
  }
  else
  {
    distal = mpKinematicChainModel->calculateEndEffectorTransformation()(cv::Rect(3, 0, 1, 3)).clone();
  }
  cedar::aux::gl::drawCone<double>(proximal, distal, .035, .035, mResolution, mIsDrawnAsWireFrame);
}

void cedar::dev::robot::gl::KinematicChain::drawEndEffector()
{
  // move to origin
  glPopMatrix();
  glPushMatrix();

  // move to object coordinates
  mTransformationTranspose = mpKinematicChainModel->calculateEndEffectorTransformation().t();
  glMultMatrixd((GLdouble*)mTransformationTranspose.data);
  
	// draw the joint
  glColor4d(mColorR, mColorG, mColorB, 0);
  cedar::aux::gl::drawSphere(.05, mResolution, mResolution, mIsDrawnAsWireFrame);
}

void cedar::dev::robot::gl::KinematicChain::drawEndEffectorVelocity()
{
  // move to origin
  glPopMatrix();
  glPushMatrix();
  glColor4d(mColorR/2, mColorG/2, mColorB/2, 0);
  cv::Mat from = mpKinematicChainModel->calculateEndEffectorPosition();
  cv::Mat to = from + mpKinematicChainModel->calculateEndEffectorVelocity();
  cedar::aux::gl::drawArrow<double>(from, to, 0.005, 0.015, 0.05, mResolution);
}

void cedar::dev::robot::gl::KinematicChain::drawEndEffectorAcceleration()
{
  // move to origin
  glPopMatrix();
  glPushMatrix();
  glColor4d(mColorR, mColorG, mColorB, 0);
  cv::Mat from
    = mpKinematicChainModel->calculateEndEffectorPosition() + mpKinematicChainModel->calculateEndEffectorVelocity();
  cv::Mat to = from + mpKinematicChainModel->calculateEndEffectorAcceleration();
  cedar::aux::gl::drawArrow<double>(from, to, 0.005, 0.015, 0.05, mResolution);
}
