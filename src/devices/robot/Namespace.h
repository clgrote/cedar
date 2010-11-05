/*----------------------------------------------------------------------------------------------------------------------
 ----- Institute:   Ruhr-Universitaet Bochum
                    Institut fuer Neuroinformatik

 ----- File:        Namespace.h

 ----- Author:      Mathis Richter
 ----- Email:       mathis.richter@ini.rub.de
 ----- Date:        2010 08 30

 ----- Description: Namespace file for cedar::dev::robot.

 ----- Credits:
 ---------------------------------------------------------------------------------------------------------------------*/

#ifndef CEDAR_DEV_ROBOT_NAMESPACE_H
#define CEDAR_DEV_ROBOT_NAMESPACE_H

// SYSTEM INCLUDES
#include <boost/smart_ptr.hpp>

namespace cedar
{
  namespace dev
  {
    /*! @brief Namespace for all robot interfaces. */
    namespace robot
    {
      class Component;
      typedef boost::shared_ptr<Component> ComponentPtr;

      class KinematicChain;
      typedef boost::shared_ptr<KinematicChain> KinematicChainPtr;

      class KinematicChainModel;
      typedef boost::shared_ptr<KinematicChainModel> KinematicChainModelPtr;

      class ReferenceGeometry;
      typedef boost::shared_ptr<ReferenceGeometry> ReferenceGeometryPtr;
    }
  }
}

#endif // CEDAR_DEV_ROBOT_NAMESPACE_H
