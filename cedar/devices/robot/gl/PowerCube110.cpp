/*======================================================================================================================

    Copyright 2011, 2012 Institut fuer Neuroinformatik, Ruhr-Universitaet Bochum, Germany
 
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

    File:        PowerCube110

    Maintainer:  Hendrik Reimann
    Email:       hendrik.reimann@ini.rub.de
    Date:        2010 12 14

    Description: implementation for a class visualizing the PowerCube 110

    Credits:

======================================================================================================================*/

#define NOMINMAX // to avoid Windows issues
#define GL_GLEXT_PROTOTYPES // to avoid a problem with finding some GL stuff, apparently caused by Qt
#define BUFFER_OFFSET(i) ((char *)NULL + (i))

// CEDAR INCLUDES
#include "cedar/auxiliaries/systemFunctions.h"
#include "cedar/auxiliaries/gl/drawShapes.h"
#include "cedar/auxiliaries/gl/gl.h"
#include "cedar/auxiliaries/gl/glu.h"
#include "cedar/auxiliaries/math/constants.h"
#include "cedar/devices/robot/gl/namespace.h"
#include "cedar/devices/robot/gl/PowerCube110.h"
#include "cedar/devices/robot/KinematicChain.h"

// SYSTEM INCLUDES


//----------------------------------------------------------------------------------------------------------------------
// constructors and destructor
//----------------------------------------------------------------------------------------------------------------------

cedar::dev::robot::gl::PowerCube110::PowerCube110
(
  cedar::dev::robot::KinematicChainPtr kinematicChain
)
:
cedar::aux::gl::ObjectVisualization(kinematicChain->getRootCoordinateFrame()),
mpKinematicChain(kinematicChain)
{
  loadData();
}

cedar::dev::robot::gl::PowerCube110::~PowerCube110()
{

}

//----------------------------------------------------------------------------------------------------------------------
// methods
//----------------------------------------------------------------------------------------------------------------------

void cedar::dev::robot::gl::PowerCube110::initializeGl()
{
  //!@todo Implement this so that it works on windows
#ifndef CEDAR_OS_WINDOWS
  // proximal link
  glGenBuffers(1, &mProximalLinkVertexVboId);
  glBindBuffer(GL_ARRAY_BUFFER, mProximalLinkVertexVboId);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * mProximalLinkVertexNumber, NULL, GL_STATIC_DRAW);
  glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertex) * mProximalLinkVertexNumber, mProximalLinkVertex);
  glGenBuffers(1, &mProximalLinkIndexVboId);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mProximalLinkIndexVboId);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, mProximalLinkFacesNumber*3 * sizeof(GLushort), mProximalLinkIndex, GL_STATIC_DRAW);

  // distal link
  glGenBuffers(1, &mDistalLinkVertexVboId);
  glBindBuffer(GL_ARRAY_BUFFER, mDistalLinkVertexVboId);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * mDistalLinkVertexNumber, NULL, GL_STATIC_DRAW);
  glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertex) * mDistalLinkVertexNumber, mDistalLinkVertex);
  glGenBuffers(1, &mDistalLinkIndexVboId);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mDistalLinkIndexVboId);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, mDistalLinkFacesNumber*3 * sizeof(GLushort), mDistalLinkIndex, GL_STATIC_DRAW);

  // distal skin
  glGenBuffers(1, &mConnectorVertexVboId);
  glBindBuffer(GL_ARRAY_BUFFER, mConnectorVertexVboId);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * mConnectorVertexNumber, NULL, GL_STATIC_DRAW);
  glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertex) * mConnectorVertexNumber, mConnectorVertex);
  glGenBuffers(1, &mConnectorIndexVboId);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mConnectorIndexVboId);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, mConnectorFacesNumber*3 * sizeof(GLushort), mConnectorIndex, GL_STATIC_DRAW);
#endif // ndef CEDAR_OS_WINDOWS
}

void cedar::dev::robot::gl::PowerCube110::draw()
{
  setAxisLength(0);
  setResolution(20);
  setColor(.5, .5, .5);

  prepareDraw();
  if (mIsVisible)
  {
    cedar::aux::gl::setColor(mColorR, mColorG, mColorB);

    // save hand origin to stack
    glPushMatrix();

    // draw the base
    if (isDrawingLocalCoordinateFrame())
    {
      cedar::aux::gl::drawAxes(0.05);
      cedar::aux::gl::setColor(mColorR, mColorG, mColorB);
    }
    setMaterial(cedar::aux::gl::ObjectVisualization::CHROME);
    this->drawElement(mProximalLinkVertexVboId, mProximalLinkIndexVboId, mProximalLinkFacesNumber);
    this->drawElement(mConnectorVertexVboId, mConnectorIndexVboId, mConnectorFacesNumber);
    glRotated(mpKinematicChain->getJointAngle(0)*180.0/cedar::aux::math::pi, 0, -1, 0);
    this->drawElement(mDistalLinkVertexVboId, mDistalLinkIndexVboId, mDistalLinkFacesNumber);
    setMaterial(cedar::aux::gl::ObjectVisualization::NO_MATERIAL);
    /*

    // first finger root
    glTranslated(.019053, -0.033, .098);
    glRotated(mpPalm->getJointAngle(0)*180.0/cedar::aux::math::pi, 0, 0, -1);
    if (isDrawingLocalCoordinateFrame())
    {
      cedar::aux::gl::drawAxes(0.05);
      cedar::aux::gl::setColor(mColorR, mColorG, mColorB);
    }
    glTranslated(.0, 0.0, -.0175);
    glRotated(90, 1, 0, 0);
    glRotated(90, 0, 1, 0);
    setMaterial(cedar::aux::gl::ObjectVisualization::CHROME);
    this->drawElement(mKnuckleVertexVboId, mKnuckleIndexVboId, mKnuckleFacesNumber);
    setMaterial(cedar::aux::gl::ObjectVisualization::NO_MATERIAL);
    glRotated(-90, 0, 1, 0);
    glRotated(-90, 1, 0, 0);

    // first finger proximal link
    glTranslated(.0, 0.0, .0175);
    glRotated(mpFingerOne->getJointAngle(0)*180.0/cedar::aux::math::pi, 0, -1, 0);
    if (isDrawingLocalCoordinateFrame())
    {
      cedar::aux::gl::drawAxes(0.05);
      cedar::aux::gl::setColor(mColorR, mColorG, mColorB);
    }
    glTranslated(.0, 0.0, -.0175);
    glRotated(90, 1, 0, 0);
    glRotated(90, 0, 1, 0);
    setMaterial(cedar::aux::gl::ObjectVisualization::CHROME);
    this->drawElement(mProximalLinkVertexVboId, mProximalLinkIndexVboId, mProximalLinkFacesNumber);
    setMaterial(cedar::aux::gl::ObjectVisualization::BLACK);
    this->drawElement(mProximalSkinVertexVboId, mProximalSkinIndexVboId, mProximalSkinFacesNumber);
    setMaterial(cedar::aux::gl::ObjectVisualization::NO_MATERIAL);
    glRotated(-90, 0, 1, 0);
    glRotated(-90, 1, 0, 0);

    // first finger distal link
    glTranslated(0, 0, 0.104);
    glRotated(mpFingerOne->getJointAngle(1)*180.0/cedar::aux::math::pi, 0, -1, 0);
    if (isDrawingLocalCoordinateFrame())
    {
      cedar::aux::gl::drawAxes(0.05);
      cedar::aux::gl::setColor(mColorR, mColorG, mColorB);
    }
    glTranslated(.0, .0, -.104);
    glRotated(90, 1, 0, 0);
    glRotated(90, 0, 1, 0);
    setMaterial(cedar::aux::gl::ObjectVisualization::CHROME);
    this->drawElement(mDistalLinkVertexVboId, mDistalLinkIndexVboId, mDistalLinkFacesNumber);
    setMaterial(cedar::aux::gl::ObjectVisualization::BLACK);
    this->drawElement(mConnectorVertexVboId, mConnectorIndexVboId, mConnectorFacesNumber);
    setMaterial(cedar::aux::gl::ObjectVisualization::NO_MATERIAL);

    // move back to hand origin and re-save
    glPopMatrix();
    glPushMatrix();

    // second finger root
    glTranslated(-.038105, 0.0, .098);
    glRotated(180, 0, 0, 1);

    if (isDrawingLocalCoordinateFrame())
    {
      cedar::aux::gl::drawAxes(0.05);
      cedar::aux::gl::setColor(mColorR, mColorG, mColorB);
    }
    glTranslated(.0, 0.0, -.0175);
    glRotated(90, 1, 0, 0);
    glRotated(90, 0, 1, 0);
    setMaterial(cedar::aux::gl::ObjectVisualization::CHROME);
    this->drawElement(mKnuckleVertexVboId, mKnuckleIndexVboId, mKnuckleFacesNumber);
    setMaterial(cedar::aux::gl::ObjectVisualization::NO_MATERIAL);
    glRotated(-90, 0, 1, 0);
    glRotated(-90, 1, 0, 0);

    // second finger proximal link
    glTranslated(.0, 0.0, .0175);
    glRotated(mpFingerTwo->getJointAngle(0)*180.0/cedar::aux::math::pi, 0, -1, 0);
    if (isDrawingLocalCoordinateFrame())
    {
      cedar::aux::gl::drawAxes(0.05);
      cedar::aux::gl::setColor(mColorR, mColorG, mColorB);
    }
    glTranslated(.0, 0.0, -.0175);
    glRotated(90, 1, 0, 0);
    glRotated(90, 0, 1, 0);
    setMaterial(cedar::aux::gl::ObjectVisualization::CHROME);
    this->drawElement(mProximalLinkVertexVboId, mProximalLinkIndexVboId, mProximalLinkFacesNumber);
    setMaterial(cedar::aux::gl::ObjectVisualization::BLACK);
    this->drawElement(mProximalSkinVertexVboId, mProximalSkinIndexVboId, mProximalSkinFacesNumber);
    setMaterial(cedar::aux::gl::ObjectVisualization::NO_MATERIAL);
    glRotated(-90, 0, 1, 0);
    glRotated(-90, 1, 0, 0);

    // second finger distal link
    glTranslated(0, 0, 0.104);
    glRotated(mpFingerTwo->getJointAngle(1)*180.0/cedar::aux::math::pi, 0, -1, 0);
    if (isDrawingLocalCoordinateFrame())
    {
      cedar::aux::gl::drawAxes(0.05);
      cedar::aux::gl::setColor(mColorR, mColorG, mColorB);
    }
    glTranslated(.0, .0, -.104);
    glRotated(90, 1, 0, 0);
    glRotated(90, 0, 1, 0);
    setMaterial(cedar::aux::gl::ObjectVisualization::CHROME);
    this->drawElement(mDistalLinkVertexVboId, mDistalLinkIndexVboId, mDistalLinkFacesNumber);
    setMaterial(cedar::aux::gl::ObjectVisualization::BLACK);
    this->drawElement(mConnectorVertexVboId, mConnectorIndexVboId, mConnectorFacesNumber);
    setMaterial(cedar::aux::gl::ObjectVisualization::NO_MATERIAL);

    // move back to hand origin
    glPopMatrix();

    // third finger root
    glTranslated(.019053, 0.033, .098);
    glRotated(mpPalm->getJointAngle(0)*180.0/cedar::aux::math::pi, 0, 0, 1);
    if (isDrawingLocalCoordinateFrame())
    {
      cedar::aux::gl::drawAxes(0.05);
      cedar::aux::gl::setColor(mColorR, mColorG, mColorB);
    }
    glTranslated(.0, 0.0, -.0175);
    glRotated(90, 1, 0, 0);
    glRotated(90, 0, 1, 0);
    setMaterial(cedar::aux::gl::ObjectVisualization::CHROME);
    this->drawElement(mKnuckleVertexVboId, mKnuckleIndexVboId, mKnuckleFacesNumber);
    setMaterial(cedar::aux::gl::ObjectVisualization::NO_MATERIAL);
    glRotated(-90, 0, 1, 0);
    glRotated(-90, 1, 0, 0);

    // third finger proximal link
    glTranslated(.0, 0.0, .0175);
    glRotated(mpFingerThree->getJointAngle(0)*180.0/cedar::aux::math::pi, 0, -1, 0);
    if (isDrawingLocalCoordinateFrame())
    {
      cedar::aux::gl::drawAxes(0.05);
      cedar::aux::gl::setColor(mColorR, mColorG, mColorB);
    }
    glTranslated(.0, 0.0, -.0175);
    glRotated(90, 1, 0, 0);
    glRotated(90, 0, 1, 0);
    setMaterial(cedar::aux::gl::ObjectVisualization::CHROME);
    this->drawElement(mProximalLinkVertexVboId, mProximalLinkIndexVboId, mProximalLinkFacesNumber);
    setMaterial(cedar::aux::gl::ObjectVisualization::BLACK);
    this->drawElement(mProximalSkinVertexVboId, mProximalSkinIndexVboId, mProximalSkinFacesNumber);
    setMaterial(cedar::aux::gl::ObjectVisualization::NO_MATERIAL);
    glRotated(-90, 0, 1, 0);
    glRotated(-90, 1, 0, 0);

    // first finger distal link
    glTranslated(0, 0, 0.104);
    glRotated(mpFingerThree->getJointAngle(1)*180.0/cedar::aux::math::pi, 0, -1, 0);
    if (isDrawingLocalCoordinateFrame())
    {
      cedar::aux::gl::drawAxes(0.05);
      cedar::aux::gl::setColor(mColorR, mColorG, mColorB);
    }
    glTranslated(.0, .0, -.104);
    glRotated(90, 1, 0, 0);
    glRotated(90, 0, 1, 0);
    setMaterial(cedar::aux::gl::ObjectVisualization::CHROME);
    this->drawElement(mDistalLinkVertexVboId, mDistalLinkIndexVboId, mDistalLinkFacesNumber);
    setMaterial(cedar::aux::gl::ObjectVisualization::BLACK);
    this->drawElement(mConnectorVertexVboId, mConnectorIndexVboId, mConnectorFacesNumber);
    setMaterial(cedar::aux::gl::ObjectVisualization::NO_MATERIAL);

*/



  }








  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


}



void cedar::dev::robot::gl::PowerCube110::loadData()
{
  // proximal link
  QString proximal_link_vertex_data_file_name
    = QString(cedar::aux::locateResource("meshes/PowerCube110/proximal_link_vertex.txt").c_str());
  loadVertexData(proximal_link_vertex_data_file_name, mProximalLinkVertexNumber, mProximalLinkVertex);
  QString proximal_link_index_data_file_name
    = QString(cedar::aux::locateResource("meshes/PowerCube110/proximal_link_index.txt").c_str());
  loadIndexData(proximal_link_index_data_file_name, mProximalLinkFacesNumber, mProximalLinkIndex);

  // distal link
  QString distal_link_vertex_data_file_name
    = QString(cedar::aux::locateResource("meshes/PowerCube110/distal_link_vertex.txt").c_str());
  loadVertexData(distal_link_vertex_data_file_name, mDistalLinkVertexNumber, mDistalLinkVertex);
  QString distal_link_index_data_file_name
    = QString(cedar::aux::locateResource("meshes/PowerCube110/distal_link_index.txt").c_str());
  loadIndexData(distal_link_index_data_file_name, mDistalLinkFacesNumber, mDistalLinkIndex);

  // connector
  QString connector_vertex_data_file_name
    = QString(cedar::aux::locateResource("meshes/PowerCube110/connector_vertex.txt").c_str());
  loadVertexData(connector_vertex_data_file_name, mConnectorVertexNumber, mConnectorVertex);
  QString connector_index_data_file_name
    = QString(cedar::aux::locateResource("meshes/PowerCube110/connector_index.txt").c_str());
  loadIndexData(connector_index_data_file_name, mConnectorFacesNumber, mConnectorIndex);

}

