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

    File:        Chessboard.cpp

    Maintainer:  Hendrik Reimann
    Email:       hendrik.reimann@ini.rub.de
    Date:        2010 11 29

    Description: Visualization of a chessboard

    Credits:

======================================================================================================================*/

#ifndef CEDAR_AUX_GL_CHESSBOARD_H
#define CEDAR_AUX_GL_CHESSBOARD_H

// CEDAR INCLUDES
#include "cedar/auxiliaries/gl/namespace.h"
#include "cedar/auxiliaries/gl/RigidBodyVisualization.h"
#include "cedar/auxiliaries/namespace.h"
#include "cedar/auxiliaries/RigidBody.h"

// SYSTEM INCLUDES


/*!@brief Simple OpenGL visualization of a chess board
 *
 * This class visualizes an instance of cedar::aux::RigidBodyVisualization as a chess board with specified dimensions
 *
 * @remarks To get a simple visualization of the RigidBody on screen, add an instance of this class to a
 * cedar::aux::gl::Scene and create a cedar::aux::gui::Viewer for it
 */
class cedar::aux::gl::Chessboard : public cedar::aux::gl::RigidBodyVisualization
{
public:
  //--------------------------------------------------------------------------------------------------------------------
  // constructors and destructor
  //--------------------------------------------------------------------------------------------------------------------
  /*!@brief standard constructor. 
   * @param pRigidBody pointer to the aux::RigidBody being visualized
   */
  Chessboard(cedar::aux::RigidBodyPtr pRigidBody);

  /*!@brief constructor. 
   * @param pRigidBody pointer to the aux::RigidBody being visualized
   * @param length extension of the board in x-direction of the object coordinate frame
   * @param width extension of the board in y-direction of the object coordinate frame
   * @param height extension of the board in z-direction of the object coordinate frame
   * @param rows number of rows in the chessboard
   * @param columns number of columns in the chessboard
   * @param R color, value for red channel in RGB
   * @param G color, value for green channel in RGB
   * @param B color, value for blue channel in RGB
   */
  Chessboard(
              cedar::aux::RigidBodyPtr pRigidBody,
              double length,
              double width,
              double height,
              int rows,
              int columns,
              double R = 0,
              double G = 0,
              double B = 0
            );
  
  //--------------------------------------------------------------------------------------------------------------------
  // public methods
  //--------------------------------------------------------------------------------------------------------------------
public:
  //!@brief draws a visualization of the object in the current GL context
  void draw();
  
  /*!@brief set extension of the board in x-direction of the object coordinate frame
   * @param value new extension value
   */
  void setLength(double value);

  /*!@brief set extension of the board in y-direction of the object coordinate frame
   * @param value new extension value
   */
  void setWidth(double value);

  /*!@brief set extension of the board in z-direction of the object coordinate frame
   * @param value new extension value
   */
  void setHeight(double value);
  
  /*!@brief set the number of rows in the chessboard
   * @param value new extension value
   */
  void setNumberOfRows(int value);
  
  /*!@brief set the number of columns in the chessboard
   * @param value new extension value
   */
  void setNumberOfColumns(int value);

  /*!@brief get extension of the board in x-direction of the object coordinate frame
   * @return extension value
   */
  double length() const;

  /*!@brief get extension of the board in y-direction of the object coordinate frame
   * @return extension value
   */
  double width() const;

  /*!@brief get extension of the board in z-direction of the object coordinate frame
   * @return extension value
   */
  double height() const;
  
  /*!@brief get the number of rows in the chessboard
   * @return number of rows
   */
  int numberOfRows() const;
  
  /*!@brief get the number of columns in the chessboard
   * @return number of columns
   */
  int numberOfColumns() const;
  
  //--------------------------------------------------------------------------------------------------------------------
  // members
  //--------------------------------------------------------------------------------------------------------------------
private:
  double mLength;
  double mWidth;
  double mHeight;
  int mNumberOfRows;
  int mNumberOfColumns;
};

#endif // CEDAR_AUX_GL_CHESSBOARD_H
