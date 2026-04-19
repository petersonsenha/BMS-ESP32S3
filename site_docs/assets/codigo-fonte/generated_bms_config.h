#pragma once

#include "bms_types.h"

namespace bms {

inline constexpr BmsConfig kGeneratedBmsConfig{
    ProtectionConfig{
        4200,
        3000,
        5000,
        10000,
        0,
        450,
        600,
        4100,
        15,
    },
    FaultHandlingConfig{
        25,
        400,
        20,
        18000,
        true,
        false,
        3000,
        10000,
    },
    OperationalConfig{
        200,
        250,
        1500,
        420,
        true,
        true,
        false,
        1500,
        500,
    },
    EstimationConfig{
        20000,
        3700,
        4200,
        3000,
        30000,
        150,
        995000,
        18f,
        5f,
        true,
    },
};

}  // namespace bms
