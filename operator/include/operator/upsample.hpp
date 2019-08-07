//
// Created by neptune on 19-7-25.
//

#ifndef __UPSAMPLE_HPP__
#define __UPSAMPLE_HPP__

#include "operator.hpp"
#include "upsample_param.hpp"

namespace TEngine {
    class UpSample : public OperatorWithParam<UpSample, UpSampleParam> {
    public:
        UpSample() {
            name_ = "UpSample";
        }
        UpSample(const UpSample& src) = default;

        virtual ~UpSample() {}

        bool InferShape(const std::vector<TEngine::TShape>& ishape, std::vector<TEngine::TShape>& oshape,
                        int layout) override;
        float GetFops(const std::vector<TEngine::TShape>& ishape, const std::vector<TEngine::TShape>& oshape) override;

        void SetSchema(void) override;
    };
}    // namespace TEngine

#endif //__UPSAMPLE_HPP__
