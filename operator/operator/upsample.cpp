//
// Created by neptune on 19-7-25.
//

#include "operator/upsample.hpp"
#include "static_graph.hpp"
#include <cmath>

namespace  TEngine {
    bool UpSample::InferShape(const std::vector<TEngine::TShape>& ishape, std::vector<TEngine::TShape>& oshape, int layout) {
        const TShape& input_shape = ishape[0];
        int input_h = input_shape.GetH();
        int input_w = input_shape.GetW();
        int output_h;
        int output_w;
        output_h = input_h * param_.scale;
        output_w = input_w * param_.scale;
        TShape shape;
        if(layout == TENGINE_LAYOUT_NCHW)
        {
            std::vector<int> dim = {input_shape.GetN(), input_shape.GetC(), output_h, output_w};

            shape.SetDim(dim);
            shape.SetDataLayout(TENGINE_LAYOUT_NCHW);
        }
        else
        {
            std::vector<int> dim = {input_shape.GetN(), output_h, output_w, input_shape.GetC()};

            shape.SetDim(dim);
            shape.SetDataLayout(TENGINE_LAYOUT_NHWC);
        }
        oshape[0] = shape;
        return true;
    }

    float UpSample::GetFops(const std::vector<TShape>& inputs, const std::vector<TShape>& outputs)
    {
        return outputs[0].GetSize();
    }

    void UpSample::SetSchema(void)
    {
        Input({"input:float32"})
                .Output({"output:float32"})
                .SetAttr("scale", 2)
                .SetDoc(R"DOC(Upsample Layer)DOC");
    }
}    // namespace TEngine