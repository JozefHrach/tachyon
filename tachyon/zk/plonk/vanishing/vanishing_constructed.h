// Copyright 2020-2022 The Electric Coin Company
// Copyright 2022 The Halo2 developers
// Use of this source code is governed by a MIT/Apache-2.0 style license that
// can be found in the LICENSE-MIT.halo2 and the LICENCE-APACHE.halo2
// file.

#ifndef TACHYON_ZK_PLONK_VANISHING_VANISHING_CONSTRUCTED_H_
#define TACHYON_ZK_PLONK_VANISHING_VANISHING_CONSTRUCTED_H_

#include <utility>
#include <vector>

#include "tachyon/zk/base/entities/entity_ty.h"
#include "tachyon/zk/plonk/vanishing/vanishing_committed.h"

namespace tachyon::zk {

template <EntityTy EntityType, typename PCS>
class VanishingConstructed;

template <typename PCS>
class VanishingConstructed<EntityTy::kProver, PCS> {
 public:
  using F = typename PCS::Field;
  using Poly = typename PCS::Poly;
  using ExtendedPoly = typename PCS::ExtendedPoly;

  VanishingConstructed() = default;
  VanishingConstructed(std::vector<Poly>&& h_pieces, std::vector<F>&& h_blinds,
                       VanishingCommitted<EntityTy::kProver, PCS>&& committed)
      : h_pieces_(std::move(h_pieces)),
        h_blinds_(std::move(h_blinds)),
        committed_(std::move(committed)) {}

  const std::vector<Poly>& h_pieces() const { return h_pieces_; }
  const std::vector<F>& h_blinds() const { return h_blinds_; }

  VanishingCommitted<EntityTy::kProver, PCS>&& TakeCommitted() && {
    return std::move(committed_);
  }

 private:
  std::vector<Poly> h_pieces_;
  std::vector<F> h_blinds_;
  VanishingCommitted<EntityTy::kProver, PCS> committed_;
};

template <typename PCS>
class VanishingConstructed<EntityTy::kVerifier, PCS> {
 public:
  using Commitment = typename PCS::Commitment;

  VanishingConstructed() = default;
  VanishingConstructed(std::vector<Commitment>&& h_commitments,
                       Commitment&& random_poly_commitment)
      : h_commitments_(std::move(h_commitments)),
        random_poly_commitment_(std::move(random_poly_commitment)) {}

  std::vector<Commitment>&& TakeHCommitments() && {
    return std::move(h_commitments_);
  }
  Commitment&& TakeRandomPolyCommitment() && {
    return std::move(random_poly_commitment_);
  }

 private:
  std::vector<Commitment> h_commitments_;
  Commitment random_poly_commitment_;
};

}  // namespace tachyon::zk

#endif  // TACHYON_ZK_PLONK_VANISHING_VANISHING_CONSTRUCTED_H_
