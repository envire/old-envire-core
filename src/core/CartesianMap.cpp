#include "CartesianMap.hpp"
#include "FrameNode.hpp"

using namespace envire;

const std::string CartesianMap::className = "envire::CartesianMap";

CartesianMap::CartesianMap(std::string const& id)
    : Layer(id)
{
}

void CartesianMap::setFrameNode(FrameNode* node)
{
    if(!env)
      throw std::runtime_error("Before setting the frame node add the object to the environment.");
    env->setFrameNode(this, node);
}

FrameNode* CartesianMap::getFrameNode() 
{
    return const_cast<FrameNode*>( static_cast<const CartesianMap&>( *this ).getFrameNode() );
}

const FrameNode* CartesianMap::getFrameNode() const 
{
    assert( env );
    return env->getFrameNode(const_cast<CartesianMap*>(this));
}

void CartesianMap::cloneTo(Environment& env) const
{
    CartesianMap* this_copy = clone();
    env.attachItem(this_copy);

    // Get the frame stack for +this+
    std::vector<FrameNode const*> frame_stack;
    FrameNode const* root_frame = getEnvironment()->getRootNode();
    {
        FrameNode const* frame = getFrameNode();
        while (frame != root_frame)
        {
            frame_stack.push_back(frame);
            frame = frame->getParent();
        }
    }

    // And duplicate it on the target environment
    {
        FrameNode* frame = env.getRootNode();
        while (!frame_stack.empty())
        {
            FrameNode* new_frame = frame_stack.back()->clone();
            frame_stack.pop_back();
            frame->addChild(new_frame);
            frame = new_frame;
        }
        this_copy->setFrameNode(frame);
    }
}

