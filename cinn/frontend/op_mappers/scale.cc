#include <variant>

#include "cinn/frontend/op_mapper_registry.h"
#include "cinn/frontend/op_mappers/common_utils.h"
#include "cinn/utils/string.h"

namespace cinn {
namespace frontend {
namespace op_mappers {

void ScaleOpMapper(const paddle::cpp::OpDesc& op_desc, const cinn::frontend::OpMapperContext& ctx) {
  CHECK_EQ(op_desc.Input("X").size(), 1UL);
  auto x_name = op_desc.Input("X").front();
  auto x      = ctx.GetVar(x_name);

  auto scale            = utils::GetAttrOrDefault<float>(op_desc, "scale", 1.0f);
  auto bias             = utils::GetAttrOrDefault<float>(op_desc, "bias", 0.0f);
  auto bias_after_scale = utils::GetAttrOrDefault<bool>(op_desc, "bias_after_scale", true);

  auto out = ctx.builder_->scale(x, scale, bias, bias_after_scale);
  CHECK_EQ(op_desc.Output("Out").size(), 1UL);
  auto out_name = op_desc.Output("Out").front();
  ctx.AddVar(out_name, out);
  ctx.AddVarModelToProgramMap(out_name, out->id);
}

}  // namespace op_mappers
}  // namespace frontend
}  // namespace cinn

CINN_REGISTER_HELPER(scale) { CINN_REGISTER_OP_MAPPER(scale, cinn::frontend::op_mappers::ScaleOpMapper) }