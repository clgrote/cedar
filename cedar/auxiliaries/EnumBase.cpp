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

    File:        EnumBase.cpp

    Maintainer:  Oliver Lomp
    Email:       oliver.lomp@ini.ruhr-uni-bochum.de
    Date:        2011 05 27

    Description: Base class for enums.

    Credits:

======================================================================================================================*/

// LOCAL INCLUDES
#include "auxiliaries/EnumBase.h"

// PROJECT INCLUDES

// SYSTEM INCLUDES

//----------------------------------------------------------------------------------------------------------------------
// constructors and destructor
//----------------------------------------------------------------------------------------------------------------------

cedar::aux::EnumBase::EnumBase(const std::string& prefix)
:
mUndefined (Enum::UNDEFINED, prefix + "UNDEFINED")
{
  this->def(mUndefined);
}

cedar::aux::EnumBase::~EnumBase()
{
}

//----------------------------------------------------------------------------------------------------------------------
// methods
//----------------------------------------------------------------------------------------------------------------------

void cedar::aux::EnumBase::def(const cedar::aux::Enum& rEnum)
{
  //! @todo check for duplicate values
  this->mEnumFromId[rEnum.id()] = rEnum;
  this->mEnumFromString[rEnum.name()] = rEnum;
  if (rEnum != cedar::aux::Enum::UNDEFINED)
  {
    this->mEnumList.push_back(rEnum);
  }
}

const cedar::aux::Enum& cedar::aux::EnumBase::get(const cedar::aux::EnumId id) const
{
  std::map<cedar::aux::EnumId, cedar::aux::Enum>::const_iterator it;
  it = this->mEnumFromId.find(id);
  if (it != this->mEnumFromId.end())
  {
    return it->second;
  }
  else
  {
    return this->mUndefined;
  }
}

const cedar::aux::Enum& cedar::aux::EnumBase::get(const std::string& id) const
{
  std::map<std::string, cedar::aux::Enum>::const_iterator it;
  it = this->mEnumFromString.find(id);
  if (it != this->mEnumFromString.end())
  {
    return it->second;
  }
  else
  {
    return this->mUndefined;
  }
}
