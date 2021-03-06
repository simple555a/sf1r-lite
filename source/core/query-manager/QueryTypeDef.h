///
/// @File   QueryTypeDef.h
/// @brief  The file which stores query type list and its number definition.
/// @author Dohyun Yun
/// @date   2009.08.10
/// @details
/// - Log
///

#ifndef _QUERYTYPEDEF_H_
#define _QUERYTYPEDEF_H_

#include <3rdparty/msgpack/msgpack.hpp>
#include <common/PropertyValue.h>


namespace sf1r {
namespace QueryFiltering {

enum FilteringOperation
{
    NULL_OPERATOR = 0,
    EQUAL,
    NOT_EQUAL,
    INCLUDE,
    EXCLUDE,
    GREATER_THAN,
    GREATER_THAN_EQUAL,
    LESS_THAN,
    LESS_THAN_EQUAL,
    RANGE,
    PREFIX,
    SUFFIX,
    SUB_STRING,
    MAX_OPERATOR
}; // end - enum

enum InterFilteringLogic
{
    AND,
    OR
};

struct FilteringType
{
    FilteringOperation operation_;
    std::string property_;
    InterFilteringLogic logic_;
    std::vector<PropertyValue> values_;

    FilteringType()
        : operation_(NULL_OPERATOR)
        , logic_(AND) 
    {}

    bool operator==(const FilteringType& obj) const
    {
        return operation_ == obj.operation_
                && property_ == obj.property_
                && logic_ == obj.logic_
                && values_ == obj.values_;
    }

    MSGPACK_DEFINE(operation_, property_, logic_, values_)
    DATA_IO_LOAD_SAVE(FilteringType, & operation_ & property_ & logic_ & values_);

private:
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version)
    {
        ar & operation_;
        ar & property_;
        ar & logic_;
        ar & values_;
    }
};

struct FilteringTreeValue
{
    FilteringType fitleringType_;
    bool isRelationNode_;
    std::string relation_;
    unsigned int childNum_;

    FilteringTreeValue()
        : isRelationNode_(false)
        , childNum_(0)
    {
    }

    bool operator==(const FilteringTreeValue& obj) const
    {
        return fitleringType_ == obj.fitleringType_
                && isRelationNode_ == obj.isRelationNode_
                && relation_ == obj.relation_
                && childNum_ == obj.childNum_;
    }

    void print()
    {
        std::cout << "is RelationNode:" << isRelationNode_ << std::endl;
        if (isRelationNode_)
        {
            std::cout << "relation_" << relation_ << std::endl;
        }
        std::cout << "childNum_:" << childNum_ << std::endl << std::endl;
    }

    MSGPACK_DEFINE(fitleringType_, isRelationNode_, relation_, childNum_)
    DATA_IO_LOAD_SAVE(FilteringTreeValue, & fitleringType_ & isRelationNode_ & relation_ & childNum_);

private:
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version)
    {
        ar & fitleringType_;
        ar & isRelationNode_;
        ar & relation_;
        ar & childNum_;
    }
};

//

} // end namespace QueryFiltering
} // end namespace sf1r

namespace msgpack
{
/// QueryFiltering::FilteringOperation
inline sf1r::QueryFiltering::FilteringOperation& operator>>(object o, sf1r::QueryFiltering::FilteringOperation& v)
{
    signed int iv = type::detail::convert_integer<signed int>(o);
    v = static_cast<sf1r::QueryFiltering::FilteringOperation>(iv);
    return v;
}

template <typename Stream>
inline packer<Stream>& operator<< (packer<Stream>& o, const sf1r::QueryFiltering::FilteringOperation& v)
    { o.pack_int(static_cast<int>(v)); return o; }

inline void operator<< (object& o, sf1r::QueryFiltering::FilteringOperation v)
    { o.type = type::POSITIVE_INTEGER, o.via.u64 = static_cast<int>(v); }

inline void operator<< (object::with_zone& o, sf1r::QueryFiltering::FilteringOperation v)
    { static_cast<object&>(o) << static_cast<int>(v); }

/// QueryFiltering::InterFilteringLogic
inline sf1r::QueryFiltering::InterFilteringLogic& operator>>(object o, sf1r::QueryFiltering::InterFilteringLogic& v)
{
    signed int iv = type::detail::convert_integer<signed int>(o);
    v = static_cast<sf1r::QueryFiltering::InterFilteringLogic>(iv);
    return v;
}

template <typename Stream>
inline packer<Stream>& operator<< (packer<Stream>& o, const sf1r::QueryFiltering::InterFilteringLogic& v)
    { o.pack_int(static_cast<int>(v)); return o; }

inline void operator<< (object& o, sf1r::QueryFiltering::InterFilteringLogic v)
    { o.type = type::POSITIVE_INTEGER, o.via.u64 = static_cast<int>(v); }

inline void operator<< (object::with_zone& o, sf1r::QueryFiltering::InterFilteringLogic v)
    { static_cast<object&>(o) << static_cast<int>(v); }
}

MAKE_FEBIRD_SERIALIZATION(sf1r::QueryFiltering::FilteringType)

#endif // _QUERYTYPEDEF_H_
