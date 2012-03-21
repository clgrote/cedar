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

    File:        ObjectVisualization.h

    Maintainer:  Hendrik Reimann
    Email:       hendrik.reimann@ini.rub.de
    Date:        2010 10 27

    Description: Virtual class for a simple object geometry for visualization with OpenGL

    Credits:

======================================================================================================================*/

#ifndef CEDAR_AUX_GL_OBJECT_VISUALIZATION_H
#define CEDAR_AUX_GL_OBJECT_VISUALIZATION_H

#define GL_GLEXT_PROTOTYPES // to avoid a problem with finding some GL stuff, apparently caused by Qt
#define BUFFER_OFFSET(i) ((char *)NULL + (i))

// CEDAR INCLUDES
#include "cedar/auxiliaries/gl/namespace.h"
#include "cedar/auxiliaries/namespace.h"
#include "cedar/auxiliaries/LocalCoordinateFrame.h"
#include "cedar/auxiliaries/math/tools.h"

// SYSTEM INCLUDES
#include <string>
#include <vector>
#include <opencv2/opencv.hpp>
#include <QObject>
#include <QGLViewer/qglviewer.h>

/*!@brief Base class for simple OpenGL visualizations of geometric objects
 *
 */
class cedar::aux::gl::ObjectVisualization : public QObject
{
  //--------------------------------------------------------------------------------------------------------------------
  // structs
  //--------------------------------------------------------------------------------------------------------------------
protected:
  //!@brief container for vertex and related data
  typedef struct
  {
    //!@brief position of the vertex
    GLfloat location[3];
    //!@brief texture coordinates
    GLfloat tex[2];
    //!@brief normal
    GLfloat normal[3];
    //!@brief color
    GLfloat colour[4];
    //!@brief pads the struct out to 64 bytes for performance increase
    GLubyte padding[16];
  } Vertex;

private:
  Q_OBJECT
  
public:
  //--------------------------------------------------------------------------------------------------------------------
  // constructors and destructor
  //--------------------------------------------------------------------------------------------------------------------
  /*!@brief standard constructor.
   * @param pLocalCoordinateFrame pointer to the LocalCoordinateFrame of the visualized object
   */
  ObjectVisualization(cedar::aux::LocalCoordinateFramePtr pLocalCoordinateFrame);
  
  /*!@brief destructor. */
  virtual ~ObjectVisualization();
  
  //--------------------------------------------------------------------------------------------------------------------
  // public methods
  //--------------------------------------------------------------------------------------------------------------------
public:
  //!@brief the visualization initializes resources in the current GL context
  virtual void initializeGl();

  //!@brief draws a visualization of the object in the current GL context
  virtual void draw() = 0;
  
  //!@brief returns visibility state
  bool isVisible() const;

  //!@brief returns type of the object
  const std::string& getObjectVisualizationType() const;
  
  //!@brief returns current resolution
  int getResolution() const;
  
  //!@brief returns R value of main object color in RGB
  double getColorR() const;
  
  //!@brief returns G value of main object color in RGB
  double getColorG() const;
  
  //!@brief returns B value of main object color in RGB
  double getColorB() const;
  
  //!@brief switch between drawing the object with full surfaces or as wire frame only
  void setDrawAsWireFrame(const bool state);

  //!@brief get state of object being drawn with full surfaces or as wire frame only
  bool isDrawnAsWireFrame() const;
  
  //!@brief switch drawing the local coordinate frame of the rigid body
  void setDrawLocalCoordinateFrame(const bool state);

  //!@brief true if the local coordinate frame of the rigid body is being drawn
  bool isDrawingLocalCoordinateFrame() const;

  /*!@brief set the length of the axes in the local coordinate frame visualization
   * @param value    new axis length
   */
  void setAxisLength(double value);

  /*!@brief get the length of the axes in the local coordinate frame visualization
   * @return axis length
   */
  double getAxisLength() const;

  /*!@brief set the general resolution of the object, 10 is a usual value
   * @param value    new resolution value
   */
  void setResolution(int value);
  
  /*!@brief sets the main color of the object, in RGB
   * @param r    value for red channel in RGB color
   * @param g    value for green channel in RGB color
   * @param b    value for blue channel in RGB color
   */
  void setColor(double r, double g, double b);

  /*!@brief returns a smart pointer to the local coordinate frame of the visualized object
   *
   * @return smart pointer to the LocalCoordinateFrame
   */
  cedar::aux::LocalCoordinateFramePtr getLocalCoordinateFrame();

  //--------------------------------------------------------------------------------------------------------------------
  // protected methods
  //--------------------------------------------------------------------------------------------------------------------
protected:

  //!@brief prepares the drawing by moving to the local coordinate frame of the object
  void prepareDraw();

  //!@brief draws the local coordinate frame of the rigid body if mIsDrawingLocalCoordinateFrame flag is set
  void drawLocalCoordinateFrame();

  /*!@brief loads vertex data from file into a vertex array
   * @param dataFileName specifies the file where the vertex data are stored
   * @param numberOfVertices specifies how many vertices are in the file
   * @param vertices array of vertices the data is stored into
   */
  void loadVertexData(const QString& dataFileName, unsigned int numberOfVertices, Vertex* vertices);

  /*!@brief loads index data from file into a vertex array
   * @param dataFileName specifies the file where the vertex data are stored
   * @param numberOfFaces specifies how many faces are in the file (3 indices for each face)
   * @param vertices array where the data is stored into
   */
  void loadIndexData(const QString& dataFileName, unsigned int numberOfFaces, GLushort* indices);

  void drawElement
  (
    const GLuint vertexVboId,
    const GLuint indexVboId,
    unsigned int numberOfFaces
  );

  //todo: this should be done with a struct for material
  void setMaterial(int material);

  //--------------------------------------------------------------------------------------------------------------------
  // private methods
  //--------------------------------------------------------------------------------------------------------------------
private:
  //!@brief initialization function
  virtual void init();
  
public slots:
  //!@brief turns the visibility of the object on and off
  void setVisibility(bool state = true);
  
  //--------------------------------------------------------------------------------------------------------------------
  // members
  //--------------------------------------------------------------------------------------------------------------------
protected:
  //!@brief geometric type of the object
  std::string mObjectType;

  //!@brief the object will only be drawn if this is true
  bool mIsVisible;
  //!@brief determines if the object is drawn with full surfaces or as wire frame only
  bool mIsDrawnAsWireFrame;
  //!@brief determines whether the local coordinate frame of the rigid body is drawn
  bool mIsDrawingLocalCoordinateFrame;
  //!@brief length of the local coordinate frame axis arrows
  double mAxisLength;
  //!@brief determines how well curves and surfaces are approximated (default 10)
  int mResolution;
  
  //!@brief object color in RGB, R channel
  double mColorR;
  //!@brief object color in RGB, G channel
  double mColorG;
  //!@brief object color in RGB, B channel
  double mColorB;
  
  //!@brief pointer to the LocalCoordinateFrame of the visualized object
  cedar::aux::LocalCoordinateFramePtr mpLocalCoordinateFrame;

  //!@brief dummy matrix to hold the transpose of the current object transformation (it's what OpenGL needs)
  cv::Mat mTransformationTranspose;

  //!@brief encodes the different materials
  enum MaterialType {
                      NO_MATERIAL,
                      SEGMENT,
                      CHROME,
                      BLACK
                    };

  static const float mNoSpecular[3];
  static const float mSegment_Ambient[3];
  static const float mSegment_Diffuse[3];
  static const float mSegment_Specular[3];
  static const float mSegment_Shininess[1];
  static const float mChrome_Ambient[3];
  static const float mChrome_Diffuse[3];
  static const float mChrome_Specular[3];
  static const float mChrome_Shininess[1];
  static const float mBlack_Ambient[3];
  static const float mBlack_Diffuse[3];
  static const float mBlack_Specular[3];
  static const float mBlack_Shininess[1];
private:

};

#endif  // CEDAR_AUX_GL_OBJECT_VISUALIZATION_H
