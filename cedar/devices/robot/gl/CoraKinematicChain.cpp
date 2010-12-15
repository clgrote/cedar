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

    File:        CoraKinematicChain.cpp

    Maintainer:  Hendrik Reimann
    Email:       hendrik.reimann@ini.rub.de
    Date:        2010 12 15

    Description: implementation for a class visualizing the arm of CoRA

    Credits:

======================================================================================================================*/

// LOCAL INCLUDES
#include "CoraKinematicChain.h"
#include "devices/robot/gl/namespace.h"

// PROJECT INCLUDES

// SYSTEM INCLUDES
#include <OpenGL/gl.h>

using namespace cedar::dev::robot;
using namespace cedar::dev::robot::gl;
//using namespace cedar::aux::gl;
//using namespace std;
//using namespace cv;

//----------------------------------------------------------------------------------------------------------------------
// constructors and destructor
//----------------------------------------------------------------------------------------------------------------------

gl::CoraKinematicChain::CoraKinematicChain(cedar::dev::robot::KinematicChainModelPtr& rpKinematicChainModel)
:
gl::KinematicChain(rpKinematicChainModel)
{

}

gl::CoraKinematicChain::~CoraKinematicChain()
{

}

//----------------------------------------------------------------------------------------------------------------------
// methods
//----------------------------------------------------------------------------------------------------------------------

void gl::CoraKinematicChain::setMaterial(int material)
{
  switch (material)
  {
  case NO_MATERIAL:
    glMaterialfv(GL_FRONT, GL_SPECULAR, mNoSpecular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mNoSpecular);
    glEnable(GL_COLOR_MATERIAL);
    break;
  case LIGHT_BLUE_METAL:
    glDisable(GL_COLOR_MATERIAL);
    glMaterialfv(GL_FRONT, GL_AMBIENT, mLightBlueMetal_Ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mLightBlueMetal_Diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mLightBlueMetal_Specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mLightBlueMetal_Shininess);
    break;
  case DARK_BLUE_METAL:
    glDisable(GL_COLOR_MATERIAL);
    glMaterialfv(GL_FRONT, GL_AMBIENT, mDarkBlueMetal_Ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mDarkBlueMetal_Diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mDarkBlueMetal_Specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mDarkBlueMetal_Shininess);
    break;
  case BRASS:
    glDisable(GL_COLOR_MATERIAL);
    glMaterialfv(GL_FRONT, GL_AMBIENT, mBrass_Ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mBrass_Diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mBrass_Specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mBrass_Shininess);
    break;
  case ARTIFICIAL_SKIN:
    glDisable(GL_COLOR_MATERIAL);
    glMaterialfv(GL_FRONT, GL_AMBIENT, mArtificialSkin_Ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mArtificialSkin_Diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mArtificialSkin_Specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mArtificialSkin_Shininess);
    break;
  case WHITE_PLASTIC:
    glDisable(GL_COLOR_MATERIAL);
    glMaterialfv(GL_FRONT, GL_AMBIENT, mWhitePlastic_Ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mWhitePlastic_Diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mWhitePlastic_Specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mWhitePlastic_Shininess);
    break;
  case BLACK_METAL:
    glDisable(GL_COLOR_MATERIAL);
    glMaterialfv(GL_FRONT, GL_AMBIENT, mBlackMetal_Ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mBlackMetal_Diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mBlackMetal_Specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mBlackMetal_Shininess);
    break;
  case CHROME:
    glDisable(GL_COLOR_MATERIAL);
    glMaterialfv(GL_FRONT, GL_AMBIENT, mChrome_Ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mChrome_Diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mChrome_Specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mChrome_Shininess);
    break;
  }
}







