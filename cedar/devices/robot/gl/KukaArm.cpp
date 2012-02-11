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

    File:        KukaArm.cpp

    Maintainer:  Hendrik Reimann
    Email:       hendrik.reimann@ini.rub.de
    Date:        2010 12 14

    Description: implementation for a class visualizing the arm of CoRA

    Credits:

======================================================================================================================*/

#define NOMINMAX // to avoid Windows issues
#define GL_GLEXT_PROTOTYPES // to avoid a problem with finding some GL stuff, apparently caused by Qt

// CEDAR INCLUDES
#include "cedar/devices/robot/gl/namespace.h"
#include "cedar/devices/robot/gl/KukaArm.h"
#include "cedar/auxiliaries/gl/drawShapes.h"
#include "cedar/auxiliaries/gl/gl.h"
#include "cedar/auxiliaries/gl/glu.h"

// SYSTEM INCLUDES
 #include <QTextStream>
 #include <QFile>


//  // Location/Normals
//  #define X_POS 0
//  #define Y_POS 1
//  #define Z_POS 2
//  // Texture Coordinates
//  #define U_POS 0
//  #define V_POS 1
//  // Colours
//  #define R_POS 0
//  #define G_POS 1
//  #define B_POS 2
//  #define A_POS 3



//----------------------------------------------------------------------------------------------------------------------
// constructors and destructor
//----------------------------------------------------------------------------------------------------------------------

cedar::dev::robot::gl::KukaArm::KukaArm
(
  cedar::dev::robot::KinematicChainModelPtr& rpKinematicChainModel,
  const std::string& pathToPolygonData
)
:
cedar::dev::robot::gl::KinematicChain(rpKinematicChainModel)
{
  loadData(pathToPolygonData);
}

cedar::dev::robot::gl::KukaArm::~KukaArm()
{

}

//----------------------------------------------------------------------------------------------------------------------
// methods
//----------------------------------------------------------------------------------------------------------------------

void cedar::dev::robot::gl::KukaArm::drawBase()
{
  prepareDraw();

  // Colors
  for (int i = 0; i < 8804; i++)
  {
    verts[i].colour[0] = 1.0;
    verts[i].colour[1] = 0.5;
    verts[i].colour[2] = 0.0;
    verts[i].colour[3] = 1.0;
  }


#define BUFFER_OFFSET(i) ((char *)NULL + (i))
  GLuint vboID; // Vertex Buffer, this needs to be accessible wherever we draw from, so in C++, this would be a class member, in regular C, it would probably be a global variable;

  glGenBuffers(1, &vboID); // Create the buffer ID, this is basically the same as generating texture ID's
  glBindBuffer(GL_ARRAY_BUFFER, vboID); // Bind the buffer (vertex array data)

  // Allocate space.  We could pass the mesh in here (where the NULL is), but it's actually faster to do it as a
  // separate step.  We also define it as GL_STATIC_DRAW which means we set the data once, and never
  // update it.  This is not a strict rule code wise, but gives hints to the driver as to where to store the data
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * 8804, NULL, GL_STATIC_DRAW);
  glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertex) * 8804, verts); // Actually upload the data

  // Set the pointers to our data.  Except for the normal value (which always has a size of 3), we must pass
  // the size of the individual component.  i.e. A vertex has 3 points (x, y, z), texture coordinates have 2 (u, v) etc.
  // Basically the arguments are (ignore the first one for the normal pointer), Size (many components to
  // read), Type (what data type is it), Stride (how far to move forward - in bytes - per vertex) and Offset
  // (where in the buffer to start reading the data - in bytes)

  // Make sure you put glVertexPointer at the end as there is a lot of work that goes on behind the scenes
  // with it, and if it's set at the start, it has to do all that work for each gl*Pointer call, rather than once at
  // the end.
  glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), BUFFER_OFFSET(12));
  glNormalPointer(GL_FLOAT, sizeof(Vertex), BUFFER_OFFSET(20));
  glColorPointer(4, GL_FLOAT, sizeof(Vertex), BUFFER_OFFSET(32));
  glVertexPointer(3, GL_FLOAT, sizeof(Vertex), BUFFER_OFFSET(0));

  // When we get here, all the vertex data is effectively on the card

  // Our Index Buffer, same as above, the variable needs to be accessible wherever we draw
  GLuint indexVBOID;
  glGenBuffers(1, &indexVBOID); // Generate buffer
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBOID); // Bind the element array buffer
  // Upload the index array, this can be done the same way as above (with NULL as the data, then a
  // glBufferSubData call, but doing it all at once for convenience)
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3738*3 * sizeof(GLushort), index, GL_STATIC_DRAW);


  // Bind our buffers much like we would for texturing
  glBindBuffer(GL_ARRAY_BUFFER, vboID);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBOID);

  // Set the state of what we are drawing (I don't think order matters here, but I like to do it in the same
  // order I set the pointers
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);
  glEnableClientState(GL_COLOR_ARRAY);
  glEnableClientState(GL_NORMAL_ARRAY);
  glEnableClientState(GL_VERTEX_ARRAY);

  // Resetup our pointers.  This doesn't reinitialise any data, only how we walk through it
  glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), BUFFER_OFFSET(12));
  glNormalPointer(GL_FLOAT, sizeof(Vertex), BUFFER_OFFSET(20));
  glColorPointer(4, GL_FLOAT, sizeof(Vertex), BUFFER_OFFSET(32));
  glVertexPointer(3, GL_FLOAT, sizeof(Vertex), BUFFER_OFFSET(0));

  // Actually do our drawing, parameters are Primitive (Triangles, Quads, Triangle Fans etc), Elements to
  // draw, Type of each element, Start Offset
  glDrawElements(GL_TRIANGLES, 3738*3, GL_UNSIGNED_SHORT, BUFFER_OFFSET(0));

  // Disable our client state back to normal drawing
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);
  glDisableClientState(GL_COLOR_ARRAY);
  glDisableClientState(GL_NORMAL_ARRAY);
  glDisableClientState(GL_VERTEX_ARRAY);







}

void cedar::dev::robot::gl::KukaArm::drawSegment(unsigned int index)
{

}

void cedar::dev::robot::gl::KukaArm::drawEndEffector()
{

}

void cedar::dev::robot::gl::KukaArm::loadData(const std::string& pathToPolygonData)
{
  std::cout << "trying to load data" << std::endl;
  QString base_data = QString(pathToPolygonData.c_str()) + QString("base.ply");
  QFile data(base_data);
//  QFile data("/Users/reimajbi/Desktop/meshes/lbr_base.ply");
  if (data.open(QFile::ReadOnly))
  {
    QTextStream text_stream(&data);
    QString line;
    QTextStream line_stream;
    QString number;
    float scale = 0.001; // mm -> m

    // read header
    for (unsigned int i=0; i<13; i++)
    {
      line = text_stream.readLine();
    }

    // read vertex data
    for (unsigned int i=0; i<8804; i++)
    {
      line = text_stream.readLine();
      QTextStream line_stream(&line);

      // position x
      number = "";
      while (!number.endsWith(" "))
      {
        number.append(line_stream.read(1));
      }
      verts[i].location[0] = number.toFloat() * scale;

      // position y
      number = "";
      while (!number.endsWith(" "))
      {
        number.append(line_stream.read(1));
      }
      verts[i].location[1] = number.toFloat() * scale;

      // position z
      number = "";
      while (!number.endsWith(" "))
      {
        number.append(line_stream.read(1));
      }
      verts[i].location[2] = number.toFloat() * scale;

      // normal x
      number = "";
      while (!number.endsWith(" "))
      {
        number.append(line_stream.read(1));
      }
      verts[i].normal[0] = number.toFloat();

      // normal y
      number = "";
      while (!number.endsWith(" "))
      {
        number.append(line_stream.read(1));
      }
      verts[i].normal[1] = number.toFloat();

      // normal z
      number = "";
      while (!number.endsWith(" "))
      {
        number.append(line_stream.read(1));
      }
      verts[i].normal[2] = number.toFloat();
    }


    // read index data
    for (unsigned int i=0; i<3738; i++)
    {
      line = text_stream.readLine();
      QTextStream line_stream(&line);

      // the leading "3"
      number = "";
      while (!number.endsWith(" "))
      {
        number.append(line_stream.read(1));
      }

      // first index
      number = "";
      while (!number.endsWith(" "))
      {
        number.append(line_stream.read(1));
      }
      index[3*i] = static_cast<GLushort>(number.toInt());

      // second index
      number = "";
      while (!number.endsWith(" "))
      {
        number.append(line_stream.read(1));
      }
      index[3*i+1] = static_cast<GLushort>(number.toInt());

      // third index
      number = "";
      while (!line_stream.atEnd())
      {
        number.append(line_stream.read(1));
      }
      index[3*i+2] = static_cast<GLushort>(number.toInt());


    }
  }
  else
  {
    std::cout << "could not read file" << std::endl;
  }


  std::cout << "first vertex: " << std::endl;
  std::cout << verts[0].location[0] << " ";
  std::cout << verts[0].location[1] << " ";
  std::cout << verts[0].location[2] << " ";
  std::cout << verts[0].normal[0] << " ";
  std::cout << verts[0].normal[1] << " ";
  std::cout << verts[0].normal[2] << std::endl;

  std::cout << "last vertex: " << std::endl;
  std::cout << verts[8803].location[0] << " ";
  std::cout << verts[8803].location[1] << " ";
  std::cout << verts[8803].location[2] << " ";
  std::cout << verts[8803].normal[0] << " ";
  std::cout << verts[8803].normal[1] << " ";
  std::cout << verts[8803].normal[2] << std::endl;

  std::cout << "first indices: " << std::endl;
  std::cout << static_cast<int>(index[0]) << " ";
  std::cout << static_cast<int>(index[1]) << " ";
  std::cout << static_cast<int>(index[2]) << std::endl;
  std::cout << static_cast<int>(index[3]) << " ";
  std::cout << static_cast<int>(index[4]) << " ";
  std::cout << static_cast<int>(index[5]) << std::endl;

  std::cout << "last indices: " << std::endl;
  std::cout << static_cast<int>(index[3736*3]) << " ";
  std::cout << static_cast<int>(index[3736*3+1]) << " ";
  std::cout << static_cast<int>(index[3736*3+2]) << std::endl;
  std::cout << static_cast<int>(index[3737*3]) << " ";
  std::cout << static_cast<int>(index[3737*3+1]) << " ";
  std::cout << static_cast<int>(index[3737*3+2]) << std::endl;
}


