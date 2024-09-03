#pragma once
#include <any>
#include <cstdint>
#include <evs/evs.hpp>

namespace evs
{
    template<typename DataType = std::any, typename IdType = std::uint64_t>
    struct Event
    {
        using data_type = DataType;
        using id_type = IdType;

    public:
        Event(IdType id, DataType data);

    public:
        const DataType  Data() const;
        const IdType    Id() const;

    private:
        const IdType   m_Id;
        const DataType m_Data;
    };
}

namespace evs
{
    template<typename DataType, typename IdType>
    Event<DataType, IdType>::Event(IdType id, DataType data)
        : m_Id(id), m_Data(data)
    {
    }

    template<typename DataType, typename IdType>
    const DataType Event<DataType, IdType>::Data() const
    {
        return m_Data;
    }

    template<typename DataType, typename IdType>
    const IdType Event<DataType, IdType>::Id() const
    {
        return m_Id;
    }
}
