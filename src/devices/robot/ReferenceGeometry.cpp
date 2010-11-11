/*----------------------------------------------------------------------------------------------------------------------
 ----- Institute:   Ruhr-Universitaet Bochum
                    Institut fuer Neuroinformatik

 ----- File:        ReferenceGeometry.cpp

 ----- Author:      Mathis Richter
 ----- Email:       mathis.richter@ini.rub.de
 ----- Date:        2010 10 25

 ----- Description: Header for the \em ReferenceGeometry class.

 ----- Credits:
 ---------------------------------------------------------------------------------------------------------------------*/

// LOCAL INCLUDES
#include "ReferenceGeometry.h"

// PROJECT INCLUDES
#include "src/auxiliaries/Namespace.h"

// SYSTEM INCLUDES
#include <vector>
#include <string>

using namespace cedar::dev::robot;

//----------------------------------------------------------------------------------------------------------------------
// constructors and destructor
//----------------------------------------------------------------------------------------------------------------------

//! constructor
ReferenceGeometry::ReferenceGeometry(const std::string& configFileName)
:
cedar::aux::ConfigurationInterface(configFileName)
{
}

//! destructor
ReferenceGeometry::~ReferenceGeometry()
{
}

//----------------------------------------------------------------------------------------------------------------------
// methods
//----------------------------------------------------------------------------------------------------------------------

void ReferenceGeometry::init()
{
  readConfiguration();
}

const ReferenceGeometry::JointPtr& ReferenceGeometry::getJoint(const unsigned int index) const
{
  return _mJoints[index];
}

const ReferenceGeometry::LinkSegmentPtr& ReferenceGeometry::getLinkSegment(const unsigned int index) const
{
  return _mLinkSegments[index];
}

const std::vector<double>& ReferenceGeometry::getBasePosition() const
{
  return _mBasePosition;
}
