#pragma once

namespace ccl::ds
{
    enum class LossPolicy
    {
        OVERWRITE_OLDEST,
        ERROR,
        BLOCK
    };

    enum class Selector2D
    {
        ROWS,
        COLUMNS
    };

    enum class Ordering2DPolicy
    {
        ROW_MAJOR,
        COLUMN_MAJOR
    };

    // The two possible byte order
    enum class ByteOrder
    {
        BigEndian = __BIG_ENDIAN,
        LittleEndian = __LITTLE_ENDIAN
    };
}