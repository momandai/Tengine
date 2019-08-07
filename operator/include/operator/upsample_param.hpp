//
// Created by neptune on 19-7-25.
//

#ifndef __UPSAMPLE_PARAM_HPP__
#define __UPSAMPLE_PARAM_HPP__

#include "parameter.hpp"

namespace TEngine {

struct UpSampleParam : public NamedParam
{
    int scale;

    DECLARE_PARSER_STRUCTURE(UpSampleParam)
    {
      DECLARE_PARSER_ENTRY(scale);
    };
};

}    // namespace TEngine

#endif //__UPSAMPLE_PARAM_HPP__
