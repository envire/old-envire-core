#include "Layer.hpp"
#include "Serialization.hpp"
#include "Operator.hpp"
#include "EnvironmentItem.hpp"
#include "Environment.hpp"

#include <stdexcept>
#include <fstream>
#include <string>

#include "boost/filesystem/path.hpp"

using namespace envire;
namespace fs = boost::filesystem;

const std::string Layer::className = "envire::Layer";

Layer::Layer(std::string const& id) :
    EnvironmentItem(id), immutable(false), dirty(false)
{
}

Layer::Layer(const Layer& other) :
    EnvironmentItem( other ),
    immutable( other.immutable ),
    dirty( other.dirty )
{
    // copy the data map, and clone the holders
    for( DataMap::const_iterator it = other.data_map.begin(); it != other.data_map.end(); it++ )
	data_map.insert( std::make_pair( it->first, it->second->clone() ) );
}

Layer& Layer::operator=(const Layer& other)
{
    if( this != &other )
    {
	EnvironmentItem::operator=( other );
	immutable = other.immutable;
	dirty = other.dirty;
	removeData();
	for( DataMap::const_iterator it = other.data_map.begin(); it != other.data_map.end(); it++ )
	    data_map.insert( std::make_pair( it->first, it->second->clone() ) );
    }
    return *this;
}

void Layer::serialize(Serialization& so)
{
    EnvironmentItem::serialize(so);

    so.write( "immutable", immutable );
}

void Layer::unserialize(Serialization& so)
{
    EnvironmentItem::unserialize(so);

    so.read( "immutable", immutable );
}

Layer::~Layer()
{
    removeData();
}

void Layer::addChild( Layer* child ) 
{
    assert( env );
    env->addChild(this, child);
}

std::list<Layer*> Layer::getParents()
{
    assert( env );
    return env->getParents(this);
}

bool Layer::isImmutable() const
{
    return immutable;
}

void Layer::setImmutable()
{
    immutable = true;
}

void Layer::resetDirty() 
{
    dirty = false;
}

void Layer::setDirty() 
{
    dirty = true;
}

bool Layer::isDirty() const
{
    return dirty;
}

bool Layer::detachFromOperator()
{
    if( isGenerated() ) 
	return env->removeInput( getGenerator(), this );

    return true;
}

bool Layer::isGenerated() const 
{
    return getGenerator();
}

Operator* Layer::getGenerator() const
{
    assert( env );
    return env->getGenerator(const_cast<Layer*>(this));
}

void Layer::updateFromOperator() 
{
    if( isGenerated() && isDirty() )
        getGenerator()->updateAll();
}

const std::string Layer::getMapFileName() const 
{
    return getMapFileName(getClassName());
}

const std::string Layer::getMapFileName(const std::string& className) const 
{
    std::string uniqueId = getUniqueId();
    std::replace(uniqueId.begin(), uniqueId.end(), '/', '_');
    return className + uniqueId;
}

const std::string Layer::getMapFileName(const std::string& path, const std::string& className) const 
{
    fs::path scenePath(path); 

    std::string fileName = getMapFileName(className);

    return (scenePath / fileName).string();
}

bool Layer::hasData(const std::string& type) const
{
    return data_map.count(type);
}

void Layer::removeData(const std::string& type)
{
    if( data_map.count( type ) )
    {
	delete data_map[type];
	data_map.erase( type );
    }
}

void Layer::removeData()
{
    for( DataMap::iterator it = data_map.begin();it != data_map.end(); it++)
	delete it->second;
    
    data_map.clear();
}