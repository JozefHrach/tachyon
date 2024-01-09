#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "tachyon/zk/plonk/circuit/examples/circuit_test.h"
#include "tachyon/zk/plonk/circuit/examples/fibonacci2_circuit.h"
#include "tachyon/zk/plonk/circuit/floor_planner/v1/v1_floor_planner.h"
#include "tachyon/zk/plonk/halo2/pinned_verifying_key.h"
#include "tachyon/zk/plonk/keys/proving_key.h"

namespace tachyon::zk::halo2 {

namespace {

constexpr uint8_t kExpectedProof[] = {
    102, 195, 181, 202, 144, 137, 33,  216, 68,  179, 205, 95,  227, 168, 162,
    99,  242, 209, 126, 138, 153, 132, 61,  39,  144, 81,  117, 233, 132, 215,
    141, 169, 142, 156, 62,  46,  95,  107, 168, 112, 10,  151, 114, 12,  24,
    131, 178, 49,  223, 153, 242, 211, 181, 231, 61,  73,  191, 51,  189, 147,
    253, 210, 169, 139, 69,  63,  124, 63,  31,  79,  44,  88,  124, 115, 79,
    197, 249, 86,  22,  176, 25,  93,  233, 82,  47,  20,  149, 35,  129, 20,
    6,   19,  163, 223, 178, 144, 1,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   43,  107, 43,  171, 68,  242, 48,
    111, 141, 166, 91,  171, 236, 75,  66,  138, 127, 250, 233, 98,  145, 219,
    13,  108, 119, 96,  53,  81,  227, 249, 59,  20,  202, 215, 162, 207, 52,
    153, 176, 57,  21,  255, 91,  178, 151, 109, 218, 68,  120, 34,  254, 160,
    37,  250, 109, 219, 163, 51,  230, 14,  196, 79,  18,  145, 248, 199, 43,
    69,  89,  130, 76,  12,  76,  189, 145, 133, 5,   214, 202, 148, 20,  204,
    70,  135, 162, 235, 80,  183, 197, 194, 161, 87,  74,  28,  8,   28,  169,
    119, 171, 165, 175, 150, 116, 224, 140, 53,  75,  218, 186, 151, 32,  85,
    223, 223, 55,  81,  130, 81,  133, 4,   154, 81,  23,  158, 23,  133, 185,
    5,   129, 121, 50,  196, 243, 124, 174, 141, 106, 16,  224, 190, 54,  62,
    188, 227, 149, 177, 136, 111, 109, 135, 20,  23,  180, 41,  70,  235, 104,
    215, 84,  46,  6,   52,  133, 31,  92,  141, 107, 216, 34,  79,  40,  40,
    22,  151, 130, 129, 9,   166, 86,  16,  165, 196, 90,  19,  211, 121, 61,
    209, 134, 43,  30,  18,  1,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   86,  156, 101, 85,  161, 30,  215, 50,
    227, 196, 250, 133, 195, 127, 46,  32,  179, 245, 166, 172, 111, 171, 235,
    9,   29,  173, 159, 117, 225, 119, 130, 31,  196, 79,  246, 58,  227, 155,
    113, 54,  155, 225, 46,  253, 63,  68,  152, 74,  236, 81,  178, 58,  60,
    142, 221, 113, 126, 151, 199, 97,  102, 154, 233, 23,  107, 205, 158, 146,
    16,  19,  161, 43,  162, 139, 88,  251, 118, 69,  215, 44,  73,  50,  54,
    145, 244, 146, 170, 95,  35,  150, 44,  229, 137, 229, 37,  48,  121, 234,
    45,  46,  88,  33,  242, 0,   180, 189, 83,  239, 199, 133, 20,  31,  205,
    199, 6,   86,  244, 181, 47,  109, 147, 54,  113, 162, 217, 156, 30,  20,
    139, 254, 20,  88,  209, 12,  27,  2,   185, 131, 172, 40,  110, 98,  30,
    0,   169, 240, 85,  115, 1,   20,  178, 192, 188, 231, 147, 132, 27,  106,
    99,  9,   157, 125, 89,  153, 8,   135, 155, 232, 117, 252, 210, 211, 25,
    104, 119, 104, 134, 42,  217, 76,  53,  53,  36,  229, 25,  59,  75,  81,
    160, 213, 56,  26,  251, 61,  32,  50,  22,  176, 212, 114, 41,  122, 68,
    66,  1,   236, 233, 18,  27,  109, 195, 72,  39,  77,  98,  168, 64,  25,
    100, 79,  155, 162, 221, 45,  210, 170, 28,  11,  58,  70,  140, 134, 30,
    9,   29,  29,  143, 7,   207, 30,  140, 75,  135, 10,  141, 197, 31,  37,
    166, 93,  136, 246, 173, 53,  53,  31,  229, 211, 74,  72,  40,  34,  114,
    58,  110, 142, 176, 234, 55,  188, 219, 197, 238, 162, 145, 138, 206, 4,
    134, 10,  209, 197, 100, 51,  159, 1,   155, 30};

class Fibonacci2V1CircuitTest : public CircuitTest {};

}  // namespace

TEST_F(Fibonacci2V1CircuitTest, Configure) {
  ConstraintSystem<F> constraint_system;
  Fibonacci2Config<F> config =
      Fibonacci2Circuit<F, V1FloorPlanner>::Configure(constraint_system);
  EXPECT_EQ(config.advice, AdviceColumnKey(0));
  EXPECT_EQ(config.instance, InstanceColumnKey(0));
  EXPECT_EQ(config.selector, Selector::Simple(0));
  EXPECT_EQ(constraint_system.num_fixed_columns(), 0);
  EXPECT_EQ(constraint_system.num_advice_columns(), 1);
  EXPECT_EQ(constraint_system.num_instance_columns(), 1);
  EXPECT_EQ(constraint_system.num_selectors(), 1);
  EXPECT_EQ(constraint_system.num_challenges(), 0);
  std::vector<Phase> expected_advice_column_phases = {kFirstPhase};
  EXPECT_EQ(constraint_system.advice_column_phases(),
            expected_advice_column_phases);
  EXPECT_TRUE(constraint_system.challenge_phases().empty());
  EXPECT_TRUE(constraint_system.selector_map().empty());
  std::vector<Gate<F>> expected_gates;
  std::vector<std::unique_ptr<Expression<F>>> polys;
  {
    std::unique_ptr<Expression<F>> poly = ExpressionFactory<F>::Product(
        ExpressionFactory<F>::Selector(config.selector),
        ExpressionFactory<F>::Sum(
            ExpressionFactory<F>::Sum(ExpressionFactory<F>::Advice(AdviceQuery(
                                          0, Rotation::Cur(), config.advice)),
                                      ExpressionFactory<F>::Advice(AdviceQuery(
                                          1, Rotation::Next(), config.advice))),
            ExpressionFactory<F>::Negated(ExpressionFactory<F>::Advice(
                AdviceQuery(2, Rotation(2), config.advice)))));
    polys.push_back(std::move(poly));
  }
  expected_gates.push_back(Gate<F>("add", {""}, std::move(polys),
                                   {Selector::Simple(0)},
                                   {
                                       {AdviceColumnKey(0), Rotation::Cur()},
                                       {AdviceColumnKey(0), Rotation::Next()},
                                       {AdviceColumnKey(0), Rotation(2)},
                                   }));
  EXPECT_EQ(constraint_system.gates(), expected_gates);
  std::vector<AdviceQueryData> expected_advice_queries = {
      AdviceQueryData(Rotation::Cur(), AdviceColumnKey(0)),
      AdviceQueryData(Rotation::Next(), AdviceColumnKey(0)),
      AdviceQueryData(Rotation(2), AdviceColumnKey(0)),
  };
  EXPECT_EQ(constraint_system.advice_queries(), expected_advice_queries);
  std::vector<size_t> expected_num_advice_queries = {3};
  EXPECT_EQ(constraint_system.num_advice_queries(),
            expected_num_advice_queries);
  std::vector<InstanceQueryData> expected_instance_queries = {
      InstanceQueryData(Rotation::Cur(), InstanceColumnKey(0)),
  };
  EXPECT_EQ(constraint_system.instance_queries(), expected_instance_queries);
  EXPECT_TRUE(constraint_system.fixed_queries().empty());
  std::vector<AnyColumnKey> expected_permutation_columns = {
      AdviceColumnKey(0),
      InstanceColumnKey(0),
  };
  EXPECT_EQ(constraint_system.permutation().columns(),
            expected_permutation_columns);
  EXPECT_TRUE(constraint_system.lookups().empty());
  EXPECT_TRUE(constraint_system.general_column_annotations().empty());
  EXPECT_FALSE(constraint_system.minimum_degree().has_value());
}

TEST_F(Fibonacci2V1CircuitTest, Synthesize) {
  size_t n = 16;
  CHECK(prover_->pcs().UnsafeSetup(n, F(2)));
  prover_->set_domain(Domain::Create(n));
  const Domain* domain = prover_->domain();

  ConstraintSystem<F> constraint_system;
  Fibonacci2Config config =
      Fibonacci2Circuit<F, V1FloorPlanner>::Configure(constraint_system);
  Assembly<PCS> assembly =
      VerifyingKey<PCS>::CreateAssembly(domain, constraint_system);

  Fibonacci2Circuit<F, V1FloorPlanner> circuit;
  circuit.floor_planner().Synthesize(&assembly, circuit, std::move(config),
                                     constraint_system.constants());

  EXPECT_TRUE(assembly.fixed_columns().empty());

  std::vector<AnyColumnKey> expected_columns = {
      AdviceColumnKey(0),
      InstanceColumnKey(0),
  };
  EXPECT_EQ(assembly.permutation().columns(), expected_columns);

  const CycleStore& cycle_store = assembly.permutation().cycle_store();
  // clang-format off
  CycleStore::Table<Label> expected_mapping({
      {{1, 0}, {1, 1},  {0, 2},  {0, 3},  {0, 4},  {0, 5},  {0, 6},  {0, 7},
       {0, 8}, {1, 2}, {0, 10}, {0, 11}, {0, 12}, {0, 13}, {0, 14}, {0, 15}},
      {{0, 0}, {0, 1},  {0, 9},  {1, 3},  {1, 4},  {1, 5},  {1, 6},  {1, 7},
       {1, 8}, {1, 9}, {1, 10}, {1, 11}, {1, 12}, {1, 13}, {1, 14}, {1, 15}},
  });
  CycleStore::Table<Label> expected_aux({
      {{0, 0}, {0, 1},  {0, 2},  {0, 3},  {0, 4},  {0, 5},  {0, 6},  {0, 7},
       {0, 8}, {0, 9}, {0, 10}, {0, 11}, {0, 12}, {0, 13}, {0, 14}, {0, 15}},
      {{0, 0}, {0, 1},  {0, 9},  {1, 3},  {1, 4},  {1, 5},  {1, 6},  {1, 7},
       {1, 8}, {1, 9}, {1, 10}, {1, 11}, {1, 12}, {1, 13}, {1, 14}, {1, 15}},
  });
  CycleStore::Table<size_t> expected_sizes({
      {2, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1},
      {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
  });
  // clang-format on
  EXPECT_EQ(cycle_store.mapping(), expected_mapping);
  EXPECT_EQ(cycle_store.aux(), expected_aux);
  EXPECT_EQ(cycle_store.sizes(), expected_sizes);

  // clang-format off
  std::vector<std::vector<bool>> expected_selectors = {
      { true,  true,  true,  true,  true,  true,  true,  true,
       false, false, false, false, false, false, false, false}};
  // clang-format on
  EXPECT_EQ(assembly.selectors(), expected_selectors);
  EXPECT_EQ(assembly.usable_rows(), base::Range<size_t>::Until(10));
}

TEST_F(Fibonacci2V1CircuitTest, LoadVerifyingKey) {
  size_t n = 16;
  CHECK(prover_->pcs().UnsafeSetup(n, F(2)));
  prover_->set_domain(Domain::Create(n));

  Fibonacci2Circuit<F, V1FloorPlanner> circuit;

  VerifyingKey<PCS> vkey;
  ASSERT_TRUE(vkey.Load(prover_.get(), circuit));

  std::vector<Commitment> expected_permutation_verifying_key;
  {
    std::vector<Point> points = {
        {"0x179d4bdbaa0d2b6a977d418a8a90d5fe4d9162cf6569dcab6f2a7ab150cf34f7",
         "0x00638bb01b57c58d95e93d44a6a6bc126133db05161925701f8d0f16bc8f2049"},
        {"0x0356186a0d83b4475fc2e5b01191cff523ee5cc46ef3fa65eff3f611b69aeae6",
         "0x13b0cc2fd4f1145e719c43d03ff5dfad5c0bb3e0642a5a40ae98f19e6a3b76d7"},
    };
    expected_permutation_verifying_key = CreateCommitments(points);
  }
  EXPECT_EQ(vkey.permutation_verifying_key().commitments(),
            expected_permutation_verifying_key);

  std::vector<Commitment> expected_fixed_commitments;
  {
    std::vector<Point> points = {
        {"0x297ff8a661d1fa1196c065b6fb7df901fb16b5b83168ddca6c7749d963cc967a",
         "0x1a7b1f2b5f3e35fc4c706ece6b8f646e95904f9aa417f8a31fd37a41da167ec1"},
    };
    expected_fixed_commitments = CreateCommitments(points);
  }
  EXPECT_EQ(vkey.fixed_commitments(), expected_fixed_commitments);

  F expected_transcript_repr = F::FromHexString(
      "0x0b95ba00d9df3c7f61587edd2ada6ca715d62dc579333da7b80365838146a2a1");
  EXPECT_EQ(vkey.transcript_repr(), expected_transcript_repr);
}

TEST_F(Fibonacci2V1CircuitTest, LoadProvingKey) {
  size_t n = 16;
  CHECK(prover_->pcs().UnsafeSetup(n, F(2)));
  prover_->set_domain(Domain::Create(n));

  Fibonacci2Circuit<F, V1FloorPlanner> circuit;

  for (size_t i = 0; i < 2; ++i) {
    ProvingKey<PCS> pkey;
    bool load_verifying_key = i == 0;
    SCOPED_TRACE(
        absl::Substitute("load_verifying_key: $0", load_verifying_key));
    if (load_verifying_key) {
      VerifyingKey<PCS> vkey;
      ASSERT_TRUE(vkey.Load(prover_.get(), circuit));
      ASSERT_TRUE(
          pkey.LoadWithVerifyingKey(prover_.get(), circuit, std::move(vkey)));
    } else {
      ASSERT_TRUE(pkey.Load(prover_.get(), circuit));
    }

    Poly expected_l_first;
    {
      std::vector<std::string_view> poly = {
          "0x2d5e098bb31e86271ccb415b196942d755b0a9c3f21dd9882fa3d63ab1000001",
          "0x2d5e098bb31e86271ccb415b196942d755b0a9c3f21dd9882fa3d63ab1000001",
          "0x2d5e098bb31e86271ccb415b196942d755b0a9c3f21dd9882fa3d63ab1000001",
          "0x2d5e098bb31e86271ccb415b196942d755b0a9c3f21dd9882fa3d63ab1000001",
          "0x2d5e098bb31e86271ccb415b196942d755b0a9c3f21dd9882fa3d63ab1000001",
          "0x2d5e098bb31e86271ccb415b196942d755b0a9c3f21dd9882fa3d63ab1000001",
          "0x2d5e098bb31e86271ccb415b196942d755b0a9c3f21dd9882fa3d63ab1000001",
          "0x2d5e098bb31e86271ccb415b196942d755b0a9c3f21dd9882fa3d63ab1000001",
          "0x2d5e098bb31e86271ccb415b196942d755b0a9c3f21dd9882fa3d63ab1000001",
          "0x2d5e098bb31e86271ccb415b196942d755b0a9c3f21dd9882fa3d63ab1000001",
          "0x2d5e098bb31e86271ccb415b196942d755b0a9c3f21dd9882fa3d63ab1000001",
          "0x2d5e098bb31e86271ccb415b196942d755b0a9c3f21dd9882fa3d63ab1000001",
          "0x2d5e098bb31e86271ccb415b196942d755b0a9c3f21dd9882fa3d63ab1000001",
          "0x2d5e098bb31e86271ccb415b196942d755b0a9c3f21dd9882fa3d63ab1000001",
          "0x2d5e098bb31e86271ccb415b196942d755b0a9c3f21dd9882fa3d63ab1000001",
          "0x2d5e098bb31e86271ccb415b196942d755b0a9c3f21dd9882fa3d63ab1000001",
      };
      expected_l_first = CreatePoly(poly);
    }
    EXPECT_EQ(pkey.l_first(), expected_l_first);

    Poly expected_l_last;
    {
      std::vector<std::string_view> poly = {
          "0x2d5e098bb31e86271ccb415b196942d755b0a9c3f21dd9882fa3d63ab1000001",
          "0x2014447de15a99b6df03833e95f96ae1299c9ec6ff990b6e75fa3b3b04846a57",
          "0x0f1f5883e65f820d14d56342dc92fd12a944d4cbbdce5377b7439bd07108fc9d",
          "0x02b337de1c8c14f22ec9b9e2f96afef3652627366f8170a0a948dad4ac1bd5e8",
          "0x030644e72e131a029b85045b68181585d2833e84879b9709143e1f593f000000",
          "0x105009f4ffd70672d94cc277eb87ed7bfe9749817a206522cde7ba58eb7b95aa",
          "0x2144f5eefad21e1ca37ae273a4ee5b4a7eef137cbbeb1d198c9e59c37ef70364",
          "0x2db11694c4a58b3789868bd388165969c30dc1120a37fff09a991abf43e42a19",
          "0x2d5e098bb31e86271ccb415b196942d755b0a9c3f21dd9882fa3d63ab1000001",
          "0x2014447de15a99b6df03833e95f96ae1299c9ec6ff990b6e75fa3b3b04846a57",
          "0x0f1f5883e65f820d14d56342dc92fd12a944d4cbbdce5377b7439bd07108fc9d",
          "0x02b337de1c8c14f22ec9b9e2f96afef3652627366f8170a0a948dad4ac1bd5e8",
          "0x030644e72e131a029b85045b68181585d2833e84879b9709143e1f593f000000",
          "0x105009f4ffd70672d94cc277eb87ed7bfe9749817a206522cde7ba58eb7b95aa",
          "0x2144f5eefad21e1ca37ae273a4ee5b4a7eef137cbbeb1d198c9e59c37ef70364",
          "0x2db11694c4a58b3789868bd388165969c30dc1120a37fff09a991abf43e42a19",
      };
      expected_l_last = CreatePoly(poly);
    }
    EXPECT_EQ(pkey.l_last(), expected_l_last);

    Poly expected_l_active_row;
    {
      std::vector<std::string_view> poly = {
          "0x12259d6b14729c0fa51e1a2470908122ef13771b2da58a367974bc177a000001",
          "0x1a8133201ba2fe22aff30c7fd04fadcb17ceab0715a13371b06d35acd9695b3d",
          "0x0d49c50dd1c3ec703dc7be1c836fd7f62c140afcf284ce19b9a99affac7b95aa",
          "0x117dae38a05bcb8c7c290ee16cec493d73ef8dafa5c61fa4a2efd9a39e63abf4",
          "0x0c19139cb84c680a79505ee7747ae78cd6c196473632bc6ea3057c773208fc9d",
          "0x136156e428a2662bc2fddfd3b39f6475dafecb8699a611f3da6edd22c3479af1",
          "0x2aaad1ad96927134ee0187781ffe43e3f08a828d829c68e7865afb6604e42a19",
          "0x28d7d254c17b7ea40ebc4659996adacebd0d8f52d021284040d407c2f33b896f",
          "0x0000000000000000000000000000000000000000000000000000000000000000",
          "0x1ef7221577b1e8f8cebdcb2fcd10296b6d9d43267b395c820757c0fc87681d81",
          "0x1d0dff96b3477fb4437e7ee32de1555b5719604277fd746561bc1be1c5846a57",
          "0x1be193603aacb384678281793fa8f20b949cf5976d9493017ba8a357ee49da57",
          "0x1e3eb107ccbf041a07f5de183cd645c4ac6bd4f8344f861078603a6a3ff70364",
          "0x20080468beb85b16c9f71b3ea2ce10fb6cdc81c346721c888ebc9109900adec6",
          "0x30114169cfaa9b194b94fb3e12d441cabad6d0fa619f4a28d8ecb10f5d1bd5e9",
          "0x2edcc3ce4ec47abd9b83b31a4db9571236223b590c30997fd30ce24f7bf2fdd6",
      };
      expected_l_active_row = CreatePoly(poly);
    }
    EXPECT_EQ(pkey.l_active_row(), expected_l_active_row);

    std::vector<Evals> expected_fixed_columns;
    {
      // clang-format off
      std::vector<std::vector<std::string_view>> evals = {{
          "0x0000000000000000000000000000000000000000000000000000000000000001",
          "0x0000000000000000000000000000000000000000000000000000000000000001",
          "0x0000000000000000000000000000000000000000000000000000000000000001",
          "0x0000000000000000000000000000000000000000000000000000000000000001",
          "0x0000000000000000000000000000000000000000000000000000000000000001",
          "0x0000000000000000000000000000000000000000000000000000000000000001",
          "0x0000000000000000000000000000000000000000000000000000000000000001",
          "0x0000000000000000000000000000000000000000000000000000000000000001",
          "0x0000000000000000000000000000000000000000000000000000000000000000",
          "0x0000000000000000000000000000000000000000000000000000000000000000",
          "0x0000000000000000000000000000000000000000000000000000000000000000",
          "0x0000000000000000000000000000000000000000000000000000000000000000",
          "0x0000000000000000000000000000000000000000000000000000000000000000",
          "0x0000000000000000000000000000000000000000000000000000000000000000",
          "0x0000000000000000000000000000000000000000000000000000000000000000",
          "0x0000000000000000000000000000000000000000000000000000000000000000",
      }};
      // clang-format on
      expected_fixed_columns = CreateColumns(evals);
    }
    EXPECT_EQ(pkey.fixed_columns(), expected_fixed_columns);

    std::vector<Poly> expected_fixed_polys;
    {
      // clang-format off
      std::vector<std::vector<std::string_view>> polys = {{
          "0x183227397098d014dc2822db40c0ac2e9419f4243cdcb848a1f0fac9f8000001",
          "0x1db5b8e23dbf6d914e37d939eb6b037c619b3e5ea827cdb2b030fc247bdc9dcb",
          "0x0000000000000000000000000000000000000000000000000000000000000000",
          "0x15612923e039d29887b96603ef6ab434a2c021b8674489fcfa35d3972944e837",
          "0x0000000000000000000000000000000000000000000000000000000000000000",
          "0x186635fde662f3b15c5e1b5fc456d148c26786b9cda347e81f7f4c2f8c9761ed",
          "0x0000000000000000000000000000000000000000000000000000000000000000",
          "0x14a1b21f8b9c269f87d74740cf8c84a3046338799106ae503c4dfb0f0b0b250e",
          "0x0000000000000000000000000000000000000000000000000000000000000000",
          "0x15b61284f96f4584f96ef5bee1c4a8ae7eca32c5d97b942edf17bbd266f4daf3",
          "0x0000000000000000000000000000000000000000000000000000000000000000",
          "0x11f18ea69ea8787324e8219fecfa5c08c0c5e4859cdefa96fbe66ab1e5689e14",
          "0x0000000000000000000000000000000000000000000000000000000000000000",
          "0x14f69b80a4d1998bf98cd6fbc1e6791ce06d4987033db882212fe34a48bb17ca",
          "0x0000000000000000000000000000000000000000000000000000000000000000",
          "0x0ca20bc2474bfe93330e63c5c5e629d521922ce0c25a74cc6b34babcf6236236",
      }};
      // clang-format on
      expected_fixed_polys = CreatePolys(polys);
    }
    EXPECT_EQ(pkey.fixed_polys(), expected_fixed_polys);

    std::vector<Evals> expected_permutations_columns;
    {
      // clang-format off
      std::vector<std::vector<std::string_view>> evals = {{
          "0x09226b6e22c6f0ca64ec26aad4c86e715b5f898e5e963f25870e56bbe533e9a2",
          "0x133f51f46c2a51201fbb89dc1d6868acfef0f7ce0e572ccb98e04e99eac3ea36",
          "0x2b337de1c8c14f22ec9b9e2f96afef3652627366f8170a0a948dad4ac1bd5e80",
          "0x107aab49e65a67f9da9cd2abf78be38bd9dc1d5db39f81de36bcfa5b4b039043",
          "0x30644e72e131a029048b6e193fd841045cea24f6fd736bec231204708f703636",
          "0x2290ee31c482cf92b79b1944db1c0147635e9004db8c3b9d13644bef31ec3bd3",
          "0x1d59376149b959ccbd157ac850893a6f07c2d99b3852513ab8d01be8e846a566",
          "0x2d8040c3a09c49698c53bfcb514d55a5b39e9b17cb093d128b8783adb8cbd723",
          "0x30644e72e131a029b85045b68181585d2833e84879b9709143e1f593f0000000",
          "0x1240a374ee6f71e12df10a129946ca9ba824f58a50e3a58b4c68dd5590b74ad8",
          "0x0530d09118705106cbb4a786ead16926d5d174e181a26686af5448492e42a181",
          "0x1fe9a328fad7382fddb3730a89f574d14e57caeac619eeb30d24fb38a4fc6fbe",
          "0x0000000000000000b3c4d79d41a91758cb49c3517c4604a520cff123608fc9cb",
          "0x0dd360411caed09700b52c71a6655715c4d558439e2d34f4307da9a4be13c42e",
          "0x130b17119778465cfb3acaee30f81dee20710ead41671f568b11d9ab07b95a9b",
          "0x02e40daf409556c02bfc85eb303402b774954d30aeb0337eb85a71e6373428de",
      },
      {
          "0x0000000000000000000000000000000000000000000000000000000000000001",
          "0x21082ca216cbbf4e1c6e4f4594dd508c996dfbe1174efb98b11509c6e306460b",
          "0x0f5c21d0ca65e0db9be1f670eca407d08ec5ec67626a74f892ccebcd0cf9b9f6",
          "0x009553089042fe83ab570b54e4e64d307d8e8b20d568e782ece3926981d0a96c",
          "0x14a6c152ace4b16a42e1377e400798e15ac60320c21d90277890dbdd551d1912",
          "0x035992598be4d2ae5334f24f30355a1fca7f0e28762a178d79e993dca70eaabf",
          "0x03b645319eb70d7a692ea8c87fbcab1f292cd502071d9ffb7291ebe28356aa07",
          "0x231e38741f5c3f0ce2509bd3289b09a893b14b136eea6ed00cec9ac9271c9db5",
          "0x2741e304be6aaf5f53641f0bacb8e9ebccd45eba1b23316bbcd39ed80acc165f",
          "0x1d24fc7e75074f099894bbda6418efb02942f07a6b6243c5ab01a6fa053c15cb",
          "0x1e23aafdf2c22e488a5f3ba3e83a8dc1800ef2be28d5cb05f779183e5f48b529",
          "0x2fcefb6a50eea1a60cf93a619c9b0b2caaa55d27a450890e56fe632a6e2f5695",
          "0x1bbd8d20344ceebf756f0e384179bf7bcd6de527b79be069cb5119b69ae2e6ef",
          "0x2d0abc19554ccd7b651b5367514bfe3d5db4da20038f5903c9f861b748f15542",
          "0x2cae0941427a92af4f219cee01c4ad3dff071346729bd095d15009b16ca955fa",
          "0x0d4615fec1d5611cd5ffa9e358e64eb494829d350acf01c136f55acac8e3624c",
      }};
      // clang-format on
      expected_permutations_columns = CreateColumns(evals);
    }
    EXPECT_EQ(pkey.permutation_proving_key().permutations(),
              expected_permutations_columns);

    std::vector<Poly> expected_permutations_polys;
    {
      // clang-format off
      std::vector<std::vector<std::string_view>> polys = {{
          "0x05f06af4a5e9253f56ae900500cf8fa172aa95f2f378a820db039783f50af1eb",
          "0x0f0fb0d263f5bb14543ae200a13af2e0780d1a4b90f748455c8a62ac18fa3e92",
          "0x04fef3b4de894e2db58283cef83f883f47375989a41b4f09d88f3fdd6834b359",
          "0x1553255b19cd6f6c6f1ca6bd6a63ffbaa9e6d3c4097a6e345e67309580987282",
          "0x0f4081c215e2331724219e823ac5d230430b72fce41edba72edfca2e06ace94e",
          "0x17af7b00c0b54135ad0385ff54daec3e776245036e7e241cb65d6d913f605e7c",
          "0x268fe52e8552462fad2c76e1c436c051a8dadc6faf57a7edb941360f83e46273",
          "0x2622d75b5033efd7da6fc6323de569b738fc901100e416b703b59f3c0a9c8054",
          "0x258ba71da37b24fe773531d00b1aab7e3beec67e42dc6242f143ea34f99b8b4a",
          "0x1c6c613fe56e8f2979a8dfd46aaf483f368c4225a55dc21e6fbd1f0cd5ac3ea4",
          "0x267d1e5d6adafc1018613e0613aab2e0676202e79239bb59f3b841db8671c9dc",
          "0x1628ecb72f96dad15ec71b17a1863b6504b288ad2cda9c2f6de051236e0e0ab3",
          "0x1c3b905033821726a9c22352d12468ef6b8de97452362ebc9d67b78ae7f993e7",
          "0x13cc971188af090820e03bd5b70f4ee13737176dc7d6e64715ea1427af461eb9",
          "0x04ec2ce3c412040e20b74af347b37ace05be800186fd627613064ba96ac21ac2",
          "0x05593ab6f9305a65f373fba2ce04d168759ccc603570f3acc891e27ce409fce1",
      },
      {
          "0x2a73e37e3b487aea61a1b5b180b1c8bbb58952558640c87068de5e0ffaf50e16",
          "0x0d4a403ce45e823da64a1fc2ea0fc49c47fac7f9b1263e28bab23e0cc4c6eb46",
          "0x104b847e4252b02b6ab6bd3fae4e4abc9da4214de88940106d3b6c414ee5fc2d",
          "0x216ab6dee7297be12501e1057228d4aacb4e78d7420e9f7deccb6dcd1a5fcc46",
          "0x06bb7a5b66c3983d3f1ad9724b177b6499dbbb69197ad1f1164b8c375e3ac163",
          "0x0d91c08cd724088d203e3abe0b7ea14663b74b4c4d3f98a9061431536ace10bc",
          "0x02179a5d2e64b0905bba4db5dc0bbff870c2f4fbed3fc2c0930e5f4a87c5649b",
          "0x25e4597d0c05edad7d34f0b090b4dc59ef8978c0d2c9d724737da712d69a5597",
          "0x16952abd7181b114fced42dfe132aa9cd3263ab7f77c44604c630a92e1ecc29b",
          "0x2fa3e6e10afbdbb10f41baed2131914a9b349adc88e50966ffeb6909ce397ed9",
          "0x1db0d924eac719816e0b332c53c4ea538baec560927b75fd0e0746c2dd1be35b",
          "0x2aab9ef338aff970a688c3fc3c7291a06c27b673776c63b7fe0f728064fdcbb2",
          "0x22d48d0fde7027eea7cfc12c4134fc7821119fc8e303cebbab6bde03a7967386",
          "0x293b996a4e467679d49ba46cac3962795815c3ab51870c6a3d55512fca187461",
          "0x0a20cf33b54dd1c458ad0f578e909dcf8153244c983dc41e24c5cafb3eebc176",
          "0x2902b4caa7502955b50a4334306200ed44d1287c67b043b45c8241117cbb180c",
      }};
      // clang-format on
      expected_permutations_polys = CreatePolys(polys);
    }
    EXPECT_EQ(pkey.permutation_proving_key().polys(),
              expected_permutations_polys);
  }
}

TEST_F(Fibonacci2V1CircuitTest, CreateProof) {
  size_t n = 16;
  CHECK(prover_->pcs().UnsafeSetup(n, F(2)));
  prover_->set_domain(Domain::Create(n));

  Fibonacci2Circuit<F, V1FloorPlanner> circuit;
  std::vector<Fibonacci2Circuit<F, V1FloorPlanner>> circuits = {
      std::move(circuit)};

  F a = F(1);
  F b = F(1);
  F out = F(55);
  std::vector<F> instance_column = {std::move(a), std::move(b), std::move(out)};
  std::vector<Evals> instance_columns = {Evals(std::move(instance_column))};
  std::vector<std::vector<Evals>> instance_columns_vec = {
      std::move(instance_columns)};

  ProvingKey<PCS> pkey;
  ASSERT_TRUE(pkey.Load(prover_.get(), circuit));
  prover_->CreateProof(pkey, std::move(instance_columns_vec), circuits);

  std::vector<uint8_t> proof = prover_->GetWriter()->buffer().owned_buffer();
  std::vector<uint8_t> expected_proof(std::begin(kExpectedProof),
                                      std::end(kExpectedProof));
  EXPECT_THAT(proof, testing::ContainerEq(expected_proof));
}

TEST_F(Fibonacci2V1CircuitTest, VerifyProof) {
  size_t n = 16;
  CHECK(prover_->pcs().UnsafeSetup(n, F(2)));
  prover_->set_domain(Domain::Create(n));

  Fibonacci2Circuit<F, V1FloorPlanner> circuit;

  VerifyingKey<PCS> vkey;
  ASSERT_TRUE(vkey.Load(prover_.get(), circuit));

  std::vector<uint8_t> owned_proof(std::begin(kExpectedProof),
                                   std::end(kExpectedProof));
  std::unique_ptr<Verifier<PCS>> verifier =
      CreateVerifier(CreateBufferWithProof(absl::MakeSpan(owned_proof)));
  F a(1);
  F b(1);
  F out(55);
  std::vector<F> instance_column = {std::move(a), std::move(b), std::move(out)};
  std::vector<Evals> instance_columns = {Evals(std::move(instance_column))};
  std::vector<std::vector<Evals>> instance_columns_vec = {
      std::move(instance_columns)};

  Proof<F, Commitment> proof;
  F h_eval;
  ASSERT_TRUE(verifier->VerifyProofForTesting(vkey, instance_columns_vec,
                                              &proof, &h_eval));

  std::vector<std::vector<Commitment>> expected_advice_commitments_vec;
  {
    std::vector<Point> points = {
        {"0x298dd784e9755190273d84998a7ed1f263a2a8e35fcdb344d8218990cab5c366",
         "0x1d42911d6e4d1799ebf92175f0a9210e911ff8dce85cce34235704fb0fd3c6e3"},
    };
    expected_advice_commitments_vec.push_back(CreateCommitments(points));
  }
  EXPECT_EQ(proof.advices_commitments_vec, expected_advice_commitments_vec);

  EXPECT_TRUE(proof.challenges.empty());

  F expected_theta = F::FromHexString(
      "0x215aef03d8c9eaa8ed2d54dd4b0b4ef4c12cd8609aeec60b4fbbb41f4a72434a");
  EXPECT_EQ(proof.theta, expected_theta);

  ASSERT_EQ(proof.lookup_permuted_commitments_vec.size(), 1);
  EXPECT_TRUE(proof.lookup_permuted_commitments_vec[0].empty());

  F expected_beta = F::FromHexString(
      "0x1d8f66604d3bc5af559640c4c6d7bb68efecf4667dcac5582da09411494ec32a");
  EXPECT_EQ(proof.beta, expected_beta);

  F expected_gamma = F::FromHexString(
      "0x2309b82e21153fbc6a05cb850be92e394fcb0bb34c1a8c862083e5aef9d95c6c");
  EXPECT_EQ(proof.gamma, expected_gamma);

  std::vector<std::vector<Commitment>>
      expected_permutation_product_commitments_vec;
  {
    std::vector<Point> points = {
        {"0x0ba9d2fd93bd33bf493de7b5d3f299df31b283180c72970a70a86b5f2e3e9c8e",
         "0x1cae21f5360efe30b4c22549b03a77e390f71935cd4d13f8a512a9004e035af5"},
        {"0x10b2dfa3130614812395142f52e95d19b01656f9c54f737c582c4f1f3f7c3f45",
         "0x26b6ea518c416bb1533fd07d2b9719b14d4c3166a9e26388444240d723da868b"},
    };
    expected_permutation_product_commitments_vec.push_back(
        CreateCommitments(points));
  }
  EXPECT_EQ(proof.permutation_product_commitments_vec,
            expected_permutation_product_commitments_vec);

  ASSERT_EQ(proof.lookup_product_commitments_vec.size(), 1);
  EXPECT_TRUE(proof.lookup_product_commitments_vec[0].empty());

  Commitment expected_vanishing_random_poly_commitment;
  {
    expected_vanishing_random_poly_commitment = CreateCommitment(
        {"0x0000000000000000000000000000000000000000000000000000000000000001",
         "0x0000000000000000000000000000000000000000000000000000000000000002"});
  }
  EXPECT_EQ(proof.vanishing_random_poly_commitment,
            expected_vanishing_random_poly_commitment);

  F expected_y = F::FromHexString(
      "0x2d74ab3a8b5e63fef90326d15a3af553ac9e47cbce0e1bf8b1dc7f28c85cac3f");
  EXPECT_EQ(proof.y, expected_y);

  std::vector<Commitment> expected_vanishing_h_poly_commitments;
  {
    std::vector<Point> points = {
        {"0x143bf9e3513560776c0ddb9162e9fa7f8a424becab5ba68d6f30f244ab2b6b2b",
         "0x26adf7792cd0a83e5739ea35e4836db5bd95b83cd9f431205f1489ce1070eecc"},
        {"0x11124fc40ee633a3db6dfa25a0fe227844da6d97b25bff1539b09934cfa2d7ca",
         "0x0b397bd2223a070097ee1780409dd5a1d85b0ad6b764a7d03878ed4e2ff5383b"},
    };
    expected_vanishing_h_poly_commitments = CreateCommitments(points);
  }
  EXPECT_EQ(proof.vanishing_h_poly_commitments,
            expected_vanishing_h_poly_commitments);

  F expected_x = F::FromHexString(
      "0x2d3655d7d0297d40431eb99caac37051ff309c26de1fa2a6b04ab3d9fa4d938e");
  EXPECT_EQ(proof.x, expected_x);

  std::vector<std::vector<F>> expected_advice_evals_vec;
  {
    std::vector<std::string_view> evals = {
        "0x1c081c4a57a1c2c5b750eba28746cc1494cad6058591bd4c0c4c8259452bc7f8",
        "0x05b985179e17519a048551825137dfdf552097bada4b358ce07496afa5ab77a9",
        "0x2e54d768eb4629b41714876d6f88b195e3bc3e36bee0106a8dae7cf3c4327981",
    };
    expected_advice_evals_vec.push_back(CreateEvals(evals));
  }
  EXPECT_EQ(proof.advice_evals_vec, expected_advice_evals_vec);

  std::vector<F> expected_fixed_evals;
  {
    std::vector<std::string_view> evals = {
        "0x121e2b86d13d79d3135ac4a51056a6098182971628284f22d86b8d5c1f853406",
    };
    expected_fixed_evals = CreateEvals(evals);
  }
  EXPECT_EQ(proof.fixed_evals, expected_fixed_evals);

  F expected_vanishing_random_eval = F::FromHexString(
      "0x0000000000000000000000000000000000000000000000000000000000000001");
  EXPECT_EQ(proof.vanishing_random_eval, expected_vanishing_random_eval);

  std::vector<F> expected_common_permutation_evals;
  {
    std::vector<std::string_view> evals = {
        "0x1f8277e1759fad1d09ebab6faca6f5b3202e7fc385fac4e332d71ea155659c56",
        "0x17e99a6661c7977e71dd8e3c3ab251ec4a98443ffd2ee19b36719be33af64fc4",
    };
    expected_common_permutation_evals = CreateEvals(evals);
  }
  EXPECT_EQ(proof.common_permutation_evals, expected_common_permutation_evals);

  std::vector<std::vector<F>> expected_permutation_product_evals_vec;
  {
    std::vector<std::string_view> evals = {
        "0x3025e589e52c96235faa92f4913632492cd74576fb588ba22ba11310929ecd6b",
        "0x1a38d5a0514b3b19e52435354cd92a8668776819d3d2fc75e89b870899597d9d",
    };
    expected_permutation_product_evals_vec.push_back(CreateEvals(evals));
  }
  EXPECT_EQ(proof.permutation_product_evals_vec,
            expected_permutation_product_evals_vec);

  std::vector<std::vector<F>> expected_permutation_product_next_evals_vec;
  {
    std::vector<std::string_view> evals = {
        "0x141e9cd9a27136936d2fb5f45606c7cd1f1485c7ef53bdb400f221582e2dea79",
        "0x2ddda29b4f641940a8624d2748c36d1b12e9ec0142447a2972d4b01632203dfb",
    };
    expected_permutation_product_next_evals_vec.push_back(CreateEvals(evals));
  }
  EXPECT_EQ(proof.permutation_product_next_evals_vec,
            expected_permutation_product_next_evals_vec);

  std::vector<std::vector<std::optional<F>>>
      expected_permutation_product_last_evals_vec;
  {
    std::vector<std::string_view> evals = {
        "0x09636a1b8493e7bcc0b214017355f0a9001e626e28ac83b9021b0cd15814fe8b",
        "",
    };
    expected_permutation_product_last_evals_vec.push_back(
        CreateOptionalEvals(evals));
  }
  EXPECT_EQ(proof.permutation_product_last_evals_vec,
            expected_permutation_product_last_evals_vec);

  ASSERT_EQ(proof.lookup_product_evals_vec.size(), 1);
  EXPECT_TRUE(proof.lookup_product_evals_vec[0].empty());

  ASSERT_EQ(proof.lookup_product_next_evals_vec.size(), 1);
  EXPECT_TRUE(proof.lookup_product_next_evals_vec[0].empty());

  ASSERT_EQ(proof.lookup_permuted_input_evals_vec.size(), 1);
  EXPECT_TRUE(proof.lookup_permuted_input_evals_vec[0].empty());

  ASSERT_EQ(proof.lookup_permuted_input_inv_evals_vec.size(), 1);
  EXPECT_TRUE(proof.lookup_permuted_input_inv_evals_vec[0].empty());

  ASSERT_EQ(proof.lookup_permuted_table_evals_vec.size(), 1);
  EXPECT_TRUE(proof.lookup_permuted_table_evals_vec[0].empty());
}

}  // namespace tachyon::zk::halo2
