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

    File:        DeclarationRegistry.cpp

    Maintainer:  Oliver Lomp,
                 Mathis Richter,
                 Stephan Zibner
    Email:       oliver.lomp@ini.ruhr-uni-bochum.de,
                 mathis.richter@ini.ruhr-uni-bochum.de,
                 stephan.zibner@ini.ruhr-uni-bochum.de
    Date:        2011 11 18

    Description:

    Credits:

======================================================================================================================*/

// LOCAL INCLUDES
#include "cedar/processing/DeclarationRegistry.h"
#include "cedar/processing/Element.h"
#include "cedar/processing/ElementDeclaration.h"

// PROJECT INCLUDES

// SYSTEM INCLUDES

cedar::proc::ElementPtr cedar::proc::DeclarationRegistry::createInstance(
                                                                          const std::string& classId,
                                                                          const std::string& name
                                                                        )
{
  ElementPtr object = this->allocateClass(classId);
  object->setName(name);
  return object;
}

void cedar::proc::DeclarationRegistry::declareClass(cedar::proc::ElementDeclarationPtr pDeclaration)
{
  const std::string& class_id = pDeclaration->getClassId();
  if (this->mDeclarations.find(class_id) != this->mDeclarations.end())
  {
    CEDAR_THROW(cedar::proc::InvalidNameException, "Duplicate class declaration: " + class_id);
  }
  this->mDeclarations[class_id] = pDeclaration;

  // manage category
  Categories::iterator category_iter;
  category_iter = this->mDeclarationsByCategory.find(pDeclaration->getCategory());
  if (category_iter == this->mDeclarationsByCategory.end())
  {
    this->mCategories.insert(pDeclaration->getCategory());
    this->mDeclarationsByCategory[pDeclaration->getCategory()] = CategoryEntries();
    category_iter = this->mDeclarationsByCategory.find(pDeclaration->getCategory());
    CEDAR_DEBUG_ASSERT(category_iter != this->mDeclarationsByCategory.end());
  }
  category_iter->second.push_back(pDeclaration);
}

const cedar::proc::DeclarationRegistry::CategoryList& cedar::proc::DeclarationRegistry::getCategories() const
{
  return this->mCategories;
}

const cedar::proc::DeclarationRegistry::CategoryEntries& cedar::proc::DeclarationRegistry::getCategoryEntries(const std::string& category) const
{
  Categories::const_iterator it = this->mDeclarationsByCategory.find(category);
  if (it != this->mDeclarationsByCategory.end())
  {
    return this->mDeclarationsByCategory.find(category)->second;
  }
  else
  {
    CEDAR_THROW(cedar::proc::InvalidCategoryException, category + " is not a valid category");
  }
}

cedar::proc::ElementDeclarationPtr cedar::proc::DeclarationRegistry::getDeclarationOf(cedar::proc::ElementPtr object)
{
  for (std::map<std::string, cedar::proc::ElementDeclarationPtr>::iterator iter = this->mDeclarations.begin();
       iter != mDeclarations.end();
       ++iter)
  {
    if (iter->second->isObjectInstanceOf(object))
    {
      return iter->second;
    }
  }
  // if this point is reached, no declaration was found - throw
  CEDAR_THROW(cedar::proc::MissingDeclarationException, "cannot find a declaration that fits the given object");
}

const cedar::proc::DeclarationRegistry::Declarations& cedar::proc::DeclarationRegistry::declarations() const
{
  return this->mDeclarations;
}

cedar::proc::ElementPtr cedar::proc::DeclarationRegistry::allocateClass(const std::string& classId) const
{
  std::map<std::string, cedar::proc::ElementDeclarationPtr>::const_iterator iter;
  iter = mDeclarations.find(classId);

  if (iter != mDeclarations.end())
  {
    return iter->second->getObjectFactory()->allocate();
  }
  // if this point is reached, no factory could be found for the given class id - throw
  CEDAR_THROW(cedar::proc::InvalidNameException, "Cannot allocate object of class \""
                                                 + classId + "\": class unknown.")
}
