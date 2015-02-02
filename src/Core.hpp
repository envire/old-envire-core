#ifndef __ENVIRE_CORE_HPP__
#define __ENVIRE_CORE_HPP__

#include <list>
#include <map>
#include <string>

#include <Eigen/Core>
#include <Eigen/Geometry>

#include <boost/intrusive_ptr.hpp>
#include <vector>
#include <stdexcept>


#include <envire/core/Environment.hpp>
#include <envire/core/EventSource.hpp>
#include <envire/core/EventTypes.hpp>
#include <envire/core/Transform.hpp>
#include <envire/core/Holder.hpp>
#include <envire/core/FrameNode.hpp>
#include <envire/core/Layer.hpp>
#include <envire/core/Operator.hpp>
#include <envire/core/CartesianMap.hpp>
#include <base/samples/rigid_body_state.h>

namespace envire
{
    class Layer;
    class CartesianMap;
    class FrameNode;
    class Operator;
    class Environment;
    class EnvironmentItem;
    class Serialization;
    class EventHandler;
    class SynchronizationEventQueue;
    class Event;
    class SerializationFactory;
}

#endif
