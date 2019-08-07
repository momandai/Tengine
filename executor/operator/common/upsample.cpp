//
// Created by neptune on 19-7-25.
//
#include <iostream>
#include <functional>
#include <stdlib.h>

#include "logger.hpp"
#include "node_ops.hpp"
#include "tensor_mem.hpp"
#include "graph.hpp"
#include "operator/upsample.hpp"
#include "data_type.hpp"

namespace  TEngine {
    namespace UpSampleRef {
        struct UpSampleOps : public NodeOps {

            void UpSampling(float* input, float* output, int inc, int inh, int inw) {
                float* out_ptr = output;
                float* in_ptr = input;
                for (int c = 0; c < inc; c++) {
                    for (int h = 0; h < inh; h++) {
                        for (int w = 0; w < inw; w++) {
                            *out_ptr = *in_ptr;
                            *(out_ptr + 1) = *in_ptr;
                            *(out_ptr + 2*inw) = *in_ptr;
                            *(out_ptr + 2*inw + 1) = *in_ptr;
                            out_ptr += 2;
                        }
                        out_ptr += 2*inw;
                    }
                }
            }

            bool Run(Node* node) {
//                UpSample* upsample_op = dynamic_cast<UpSample*>(node->GetOp());
//                UpSampleParam* param_ = upsample_op->GetParam();

                // input, output, shape
                Tensor* itensor = node->GetInputTensor(0);
                const TShape& ishape = itensor->GetShape();
                Tensor* otensor = node->GetOutputTensor(0);
                TShape& oshape = otensor->GetShape();
                int input_c = ishape.GetC();
                int input_h = ishape.GetH();
                int input_w = ishape.GetW();
                int input_n = ishape.GetN();
//                int elem_size = DataType::GetTypeSize(itensor->GetDataType());

                int output_h = oshape.GetH();
                int output_w = oshape.GetW();

                int in_hw = input_w * input_h;
                int in_chw = input_c * in_hw;

                int out_hw = output_h * output_w;
                int out_chw = input_c * out_hw;

                // data
                uint8_t* input_data = ( uint8_t* )get_tensor_mem(itensor);
                uint8_t* output_data = ( uint8_t* )get_tensor_mem(otensor);

                if(exec_attr->graph_layout == TENGINE_LAYOUT_NCHW) {
                    for (int n = 0; n < input_n; n++) {
                        UpSampling((float*)input_data + n * in_chw, (float*)output_data + n * out_chw, input_c, input_h, input_w);
                    }
                } else {
                    std::cout << "not implement!\n";
                    return false;
                }
                return true;
            }
        };
        NodeOps* SelectFunc(const CPUInfo* cpu_info, Node* node)
        {
            Tensor* input = node->GetInputTensor(0);
            const int data_type = input->GetDataType();
            const ExecAttr* exec_attr = any_cast<const ExecAttr*>(node->GetAttr(ATTR_EXEC_ATTR));
            if(data_type != TENGINE_DT_FP32 || exec_attr->graph_layout != TENGINE_LAYOUT_NCHW)
                return nullptr;

            UpSampleOps* ops = new UpSampleOps();

            return ops;
        }

    }    // namespace UpSampleRef
    using namespace UpSampleRef;
    void RegisterUpSample_NodeExec(void) {
        NodeOpsRegistryManager::RegisterOPImplementor("common", "UpSample", UpSampleRef::SelectFunc, 1000);
    }
}  // namespace TEngine
