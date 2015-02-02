#include "EnvironmentItem.hpp"
#include "Environment.hpp"

using namespace std;
using namespace envire;

const std::string EnvironmentItem::className = "envire::EnvironmentItem";

void envire::intrusive_ptr_add_ref( EnvironmentItem* item ) { item->ref_count++; }
void envire::intrusive_ptr_release( EnvironmentItem* item ) { if(!--item->ref_count) delete item; }

EnvironmentItem::EnvironmentItem(std::string const& unique_id)
    : ref_count(0), unique_id(unique_id), env(NULL)
{
}

EnvironmentItem::EnvironmentItem(Environment* envPtr)
   : ref_count(0), unique_id( Environment::ITEM_NOT_ATTACHED ), env(NULL)
{
    envPtr->attachItem( this );
}

EnvironmentItem::EnvironmentItem(const EnvironmentItem& item)
    : ref_count(0), unique_id( Environment::ITEM_NOT_ATTACHED ), env(NULL)
{
}

EnvironmentItem& EnvironmentItem::operator=(const EnvironmentItem& other)
{
    return *this;
}

EnvironmentItem::~EnvironmentItem()
{
    assert( !isAttached() );
}

bool EnvironmentItem::isAttached() const
{
    return env;
}

void EnvironmentItem::setUniqueId(std::string const& id)
{
    if (isAttached())
        throw std::logic_error("trying to change an item's ID after it was attached to an environment");

    unique_id = id;
}

std::string EnvironmentItem::getUniqueId() const
{
    return unique_id;
}

std::string EnvironmentItem::getUniqueIdPrefix() const
{
    size_t idpos = unique_id.rfind('/');
    std::string prefix_str = "";
    if(idpos != string::npos && idpos > 0)
    {
        prefix_str = unique_id.substr(0,idpos);
    }
    return prefix_str;
}

std::string EnvironmentItem::getUniqueIdSuffix() const
{
    size_t idpos = unique_id.rfind('/');
    std::string idstr;
    if(idpos != string::npos)
    {
        idstr = unique_id.substr(idpos+1);
    }
    else
    {
        // backward compatibility
        idstr = unique_id;
    }
    return idstr;
}

long EnvironmentItem::getUniqueIdNumericalSuffix() const
{
    return boost::lexical_cast<long>(getUniqueIdSuffix());
}
