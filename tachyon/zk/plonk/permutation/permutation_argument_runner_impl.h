// Copyright 2020-2022 The Electric Coin Company
// Copyright 2022 The Halo2 developers
// Use of this source code is governed by a MIT/Apache-2.0 style license that
// can be found in the LICENSE-MIT.halo2 and the LICENCE-APACHE.halo2
// file.

#ifndef TACHYON_ZK_PLONK_PERMUTATION_PERMUTATION_ARGUMENT_RUNNER_IMPL_H_
#define TACHYON_ZK_PLONK_PERMUTATION_PERMUTATION_ARGUMENT_RUNNER_IMPL_H_

#include <functional>
#include <utility>
#include <vector>

#include "tachyon/base/logging.h"
#include "tachyon/zk/base/blinded_polynomial.h"
#include "tachyon/zk/plonk/circuit/rotation.h"
#include "tachyon/zk/plonk/permutation/grand_product_argument.h"
#include "tachyon/zk/plonk/permutation/permutation_argument_runner.h"
#include "tachyon/zk/plonk/permutation/permutation_table_store.h"
#include "tachyon/zk/plonk/permutation/permutation_utils.h"
#include "tachyon/zk/plonk/permutation/permuted_table.h"
#include "tachyon/zk/plonk/permutation/unpermuted_table.h"

namespace tachyon::zk {

template <typename Poly, typename Evals>
template <typename PCS, typename F>
PermutationCommitted<Poly>
PermutationArgumentRunner<Poly, Evals>::CommitArgument(
    ProverBase<PCS>* prover, const PermutationArgument& argument,
    RefTable<Evals>& table, size_t constraint_system_degree,
    const PermutationProvingKey<Poly, Evals>& permutation_proving_key,
    const F& beta, const F& gamma) {
  // How many columns can be included in a single permutation polynomial?
  // We need to multiply by z(X) and (1 - (l_last(X) + l_blind(X))). This
  // will never underflow because of the requirement of at least a degree
  // 3 circuit for the permutation argument.
  CHECK_GE(constraint_system_degree, argument.RequiredDegree());

  size_t chunk_len = ComputePermutationChunkLength(constraint_system_degree);
  size_t chunk_num = (argument.columns().size() + chunk_len - 1) / chunk_len;

  UnpermutedTable<Evals> unpermuted_table = UnpermutedTable<Evals>::Construct(
      argument.columns().size(), prover->pcs().N(), prover->domain());
  PermutedTable<Evals> permuted_table(&permutation_proving_key.permutations());
  PermutationTableStore<Evals> table_store(
      argument.columns(), table, permuted_table, unpermuted_table, chunk_len);

  std::vector<BlindedPolynomial<Poly>> grand_product_polys;
  grand_product_polys.reserve(chunk_num);

  // Track the "last" value from the previous column set.
  F last_z = F::One();

  for (size_t i = 0; i < chunk_num; ++i) {
    std::vector<base::Ref<const Evals>> permuted_columns =
        table_store.GetPermutedColumns(i);
    std::vector<base::Ref<const Evals>> unpermuted_columns =
        table_store.GetUnpermutedColumns(i);
    std::vector<base::Ref<const Evals>> value_columns =
        table_store.GetValueColumns(i);

    size_t chunk_size = table_store.GetChunkSize(i);
    BlindedPolynomial<Poly> grand_product_poly =
        GrandProductArgument::CommitExcessive(
            prover,
            CreateNumeratorCallback<F>(unpermuted_columns, value_columns, beta,
                                       gamma),
            CreateDenominatorCallback<F>(permuted_columns, value_columns, beta,
                                         gamma),
            chunk_size, last_z);

    grand_product_polys.push_back(std::move(grand_product_poly));
  }

  return PermutationCommitted<Poly>(std::move(grand_product_polys));
}

template <typename Poly, typename Evals>
template <typename PCS, typename F>
PermutationEvaluated<Poly>
PermutationArgumentRunner<Poly, Evals>::EvaluateCommitted(
    ProverBase<PCS>* prover, PermutationCommitted<Poly>&& committed,
    const F& x) {
  int32_t blinding_factors =
      static_cast<int32_t>(prover->blinder().blinding_factors());

  std::vector<BlindedPolynomial<Poly>> product_polys =
      std::move(committed).TakeProductPolys();

  for (size_t i = 0; i < product_polys.size(); ++i) {
    const Poly& poly = product_polys[i].poly();

    CHECK(prover->Evaluate(poly, x));

    F x_next = Rotation::Next().RotateOmega(prover->domain(), x);
    CHECK(prover->Evaluate(poly, x_next));

    // If we have any remaining sets to process, evaluate this set at ωᵘ
    // so we can constrain the last value of its running product to equal the
    // first value of the next set's running product, chaining them together.
    if (i != product_polys.size() - 1) {
      F x_last =
          Rotation(-(blinding_factors + 1)).RotateOmega(prover->domain(), x);
      CHECK(prover->Evaluate(poly, x_last));
    }
  }

  return PermutationEvaluated<Poly>(std::move(product_polys));
}

template <typename Poly, typename Evals>
template <typename PCS, typename F>
std::vector<ProverQuery<PCS>>
PermutationArgumentRunner<Poly, Evals>::OpenEvaluated(
    const ProverBase<PCS>* prover, const PermutationEvaluated<Poly>& evaluated,
    const F& x) {
  const std::vector<BlindedPolynomial<Poly>>& product_polys =
      evaluated.product_polys();

  std::vector<ProverQuery<PCS>> ret;
  ret.reserve(product_polys.size() * 3 - 1);

  F x_next = Rotation::Next().RotateOmega(prover->domain(), x);
  for (const BlindedPolynomial<Poly>& blinded_poly : product_polys) {
    ret.emplace_back(x, blinded_poly.ToRef());
    ret.emplace_back(x_next, blinded_poly.ToRef());
  }

  int32_t blinding_factors =
      static_cast<int32_t>(prover->blinder().blinding_factors());
  F x_last = Rotation(-(blinding_factors + 1)).RotateOmega(prover->domain(), x);
  for (auto it = product_polys.rbegin() + 1; it != product_polys.rend(); ++it) {
    ret.emplace_back(x_last, it->ToRef());
  }
  return ret;
}

template <typename Poly, typename Evals>
template <typename PCS, typename F>
std::vector<BlindedPolynomial<Poly>>
PermutationArgumentRunner<Poly, Evals>::BlindProvingKey(
    ProverBase<PCS>* prover,
    const PermutationProvingKey<Poly, Evals>& proving_key) {
  std::vector<BlindedPolynomial<Poly>> ret;
  ret.reserve(proving_key.polys().size());
  for (Poly& poly : proving_key.polys()) {
    F blind = prover->blinder().Generate();
    ret.emplace_back(std::move(poly), std::move(blind));
  }
  return ret;
}

template <typename Poly, typename Evals>
template <typename PCS, typename F>
std::vector<ProverQuery<PCS>>
PermutationArgumentRunner<Poly, Evals>::OpenBlindedPolynomials(
    const std::vector<BlindedPolynomial<Poly>>& blinded_polys, const F& x) {
  std::vector<ProverQuery<PCS>> ret;
  ret.reserve(blinded_polys.size());
  for (const BlindedPolynomial<Poly>& blind_poly : blinded_polys) {
    ret.emplace_back(x, blind_poly.ToRef());
  }
  return ret;
}

template <typename Poly, typename Evals>
template <typename PCS, typename F>
void PermutationArgumentRunner<Poly, Evals>::EvaluateProvingKey(
    ProverBase<PCS>* prover,
    const PermutationProvingKey<Poly, Evals>& proving_key, const F& x) {
  for (const Poly& poly : proving_key.polys()) {
    CHECK(prover->Evaluate(poly, x));
  }
}

template <typename Poly, typename Evals>
template <typename F>
std::function<base::ParallelizeCallback3<F>(size_t)>
PermutationArgumentRunner<Poly, Evals>::CreateNumeratorCallback(
    const std::vector<base::Ref<const Evals>>& unpermuted_columns,
    const std::vector<base::Ref<const Evals>>& value_columns, const F& beta,
    const F& gamma) {
  // vᵢ(ωʲ) + β * δⁱ * ωʲ + γ
  return [&unpermuted_columns, &value_columns, &beta,
          &gamma](size_t column_index) {
    const Evals& unpermuted_values = *unpermuted_columns[column_index];
    const Evals& values = *value_columns[column_index];
    return [&unpermuted_values, &values, &beta, &gamma](
               absl::Span<F> chunk, size_t chunk_index, size_t chunk_size_in) {
      size_t i = chunk_index * chunk_size_in;
      for (F& result : chunk) {
        result *= *values[i] + beta * *unpermuted_values[i] + gamma;
        ++i;
      }
    };
  };
}

template <typename Poly, typename Evals>
template <typename F>
std::function<base::ParallelizeCallback3<F>(size_t)>
PermutationArgumentRunner<Poly, Evals>::CreateDenominatorCallback(
    const std::vector<base::Ref<const Evals>>& permuted_columns,
    const std::vector<base::Ref<const Evals>>& value_columns, const F& beta,
    const F& gamma) {
  // vᵢ(ωʲ) + β * sᵢ(ωʲ) + γ
  return [&permuted_columns, &value_columns, &beta,
          &gamma](size_t column_index) {
    const Evals& permuted_values = *permuted_columns[column_index];
    const Evals& values = *value_columns[column_index];
    return [&permuted_values, &values, &beta, &gamma](
               absl::Span<F> chunk, size_t chunk_index, size_t chunk_size_in) {
      size_t i = chunk_index * chunk_size_in;
      for (F& result : chunk) {
        result *= *values[i] + beta * *permuted_values[i] + gamma;
        ++i;
      }
    };
  };
}

}  // namespace tachyon::zk

#endif  // TACHYON_ZK_PLONK_PERMUTATION_PERMUTATION_ARGUMENT_RUNNER_IMPL_H_
