/*======================================================================================================================

    Copyright 2011, 2012, 2013, 2014, 2015, 2016, 2017 Institut fuer Neuroinformatik, Ruhr-Universitaet Bochum, Germany
 
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

    File:        CoraArm.h

    Maintainer:  Hendrik Reimann
    Email:       hendrik.reimann@ini.rub.de
    Date:        2010 12 14

    Description: header for a class visualizing the arm of CoRA

    Credits:

======================================================================================================================*/

#ifndef CEDAR_DEV_GL_CORA_ARM_H
#define CEDAR_DEV_GL_CORA_ARM_H

// CEDAR INCLUDES
#include "cedar/devices/gl/namespace.h"
#include "cedar/devices/gl/AmtecChain.h"

// SYSTEM INCLUDES

/*!@brief Visualization of the CoRA arm
 *
 * This class provides a simple OpenGL visualization of the CoRA arm. It has to be provided with a pointer to an
 * instance of KinematicChain of the CoRA arm, used to get the transformations to the joint coordinate frames.
 * To actually display the arm, add an instance of this class to a scene (cedar::aux::gl::Scene) and create a viewer
 * for that scene (cedar::aux::gl::Viewer).
 */
class cedar::dev::gl::CoraArm : public cedar::dev::gl::AmtecChain
{
  //--------------------------------------------------------------------------------------------------------------------
  // constructors and destructor
  //--------------------------------------------------------------------------------------------------------------------
public:
  //!@brief constructor
  CoraArm(cedar::dev::KinematicChainPtr pKinematicChain);
  //!@brief destructor
  ~CoraArm();

  //--------------------------------------------------------------------------------------------------------------------
  // public methods
  //--------------------------------------------------------------------------------------------------------------------
public:
  // none yet

  //--------------------------------------------------------------------------------------------------------------------
  // protected methods
  //--------------------------------------------------------------------------------------------------------------------
protected:
  //!@brief draws the base
  void drawBase();

  //!@brief draws the segment attached to the specified joint
  void drawSegment(unsigned int index);

  //!@brief draws the end-effector
  void drawEndEffector();

  //--------------------------------------------------------------------------------------------------------------------
  // private methods
  //--------------------------------------------------------------------------------------------------------------------
private:
  void drawLinkedModule(double size);

  void drawTriangleLink(int link);

  //--------------------------------------------------------------------------------------------------------------------
  // members
  //--------------------------------------------------------------------------------------------------------------------
protected:
  // none yet
private:
  // none yet
}; // class cedar::dev::gl::CoraArm
#endif // CEDAR_DEV_GL_CORA_ARM_H

