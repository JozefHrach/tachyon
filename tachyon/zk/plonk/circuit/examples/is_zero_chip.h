#ifndef TACHYON_ZK_PLONK_CIRCUIT_EXAMPLES_IS_ZERO_CHIP_H_
#define TACHYON_ZK_PLONK_CIRCUIT_EXAMPLES_IS_ZERO_CHIP_H_

#include <memory>
#include <utility>

#include "tachyon/zk/expressions/expression_factory.h"
#include "tachyon/zk/plonk/circuit/region.h"
#include "tachyon/zk/plonk/constraint_system.h"

namespace tachyon::zk {

template <typename F>
class IsZeroConfig {
 public:
  using Field = F;

  IsZeroConfig(const AdviceColumnKey& value_inv,
               std::unique_ptr<Expression<F>> is_zero_expr)
      : value_inv_(value_inv), is_zero_expr_(std::move(is_zero_expr)) {}

  const AdviceColumnKey& value_inv() const { return value_inv_; }
  std::unique_ptr<Expression<F>> expr() const { return is_zero_expr_->Clone(); }

  IsZeroConfig Clone() const {
    return IsZeroConfig(value_inv_, is_zero_expr_->Clone());
  }

 private:
  AdviceColumnKey value_inv_;
  std::unique_ptr<Expression<F>> is_zero_expr_;
};

template <typename F>
class IsZeroChip {
 public:
  explicit IsZeroChip(IsZeroConfig<F>&& config) : config_(std::move(config)) {}

  static IsZeroConfig<F> Configure(
      ConstraintSystem<F>& meta,
      base::OnceCallback<std::unique_ptr<Expression<F>>(VirtualCells<F>&)>
          q_enable,
      base::OnceCallback<std::unique_ptr<Expression<F>>(VirtualCells<F>&)>
          value,
      const AdviceColumnKey& value_inv) {
    std::unique_ptr<Expression<F>> is_zero_expr =
        ExpressionFactory<F>::Constant(F::Zero());

    meta.CreateGate("is_zero", [&q_enable, &value,
                                value_inv](VirtualCells<F>& meta) mutable {
      // clang-format off
      //
      // q_enable | value |  value_inv |  1 - value * value_inv | value * (1 - value * value_inv)
      // ---------+-------+------------+------------------------+-------------------------------
      //   yes    |   x   |    1/x     |         0              |         0
      //   no     |   x   |    0       |         1              |         x
      //   yes    |   0   |    0       |         1              |         0
      //   yes    |   0   |    y       |         1              |         0
      //
      // clang-format on
      std::unique_ptr<Expression<F>> q_enable_res =
          std::move(q_enable).Run(meta);
      std::unique_ptr<Expression<F>> value_res = std::move(value).Run(meta);
      std::unique_ptr<Expression<F>> value_inv_res =
          meta.QueryAdvice(value_inv, Rotation::Cur());

      std::unique_ptr<Expression<F>> is_zero_expr =
          std::move(ExpressionFactory<F>::Constant(F::One())) -
          std::move(value_res->Clone()) * std::move(value_inv_res);
      std::vector<Constraint<F>> constraints;
      constraints.emplace_back(std::move(q_enable_res) * std::move(value_res) *
                               std::move(is_zero_expr));
      return constraints;
    });

    return IsZeroConfig<F>(value_inv, std::move(is_zero_expr));
  }

  void Assign(Region<F>& region, size_t offset, const Value<F>& value) const {
    const F value_inv = value.IsOne() ? F::Zero() : value.value().Inverse();
    region.AssignAdvice("value inv", config_.value_inv(), offset,
                        [&value_inv]() { return Value<F>::Known(value_inv); });
  }

 private:
  IsZeroConfig<F> config_;
};

}  // namespace tachyon::zk

#endif  // TACHYON_ZK_PLONK_CIRCUIT_EXAMPLES_IS_ZERO_CHIP_H_
