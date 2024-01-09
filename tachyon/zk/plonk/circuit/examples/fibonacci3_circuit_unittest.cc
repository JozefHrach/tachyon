#include "tachyon/zk/plonk/circuit/examples/fibonacci3_circuit.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "tachyon/zk/plonk/circuit/examples/circuit_test.h"
#include "tachyon/zk/plonk/circuit/floor_planner/simple_floor_planner.h"
#include "tachyon/zk/plonk/halo2/pinned_verifying_key.h"
#include "tachyon/zk/plonk/keys/proving_key.h"

namespace tachyon::zk::halo2 {

namespace {

constexpr uint8_t kExpectedProof[] = {
    33,  231, 248, 121, 132, 226, 31,  179, 86,  67,  213, 109, 201, 53,  147,
    93,  41,  30,  16,  172, 115, 107, 0,   172, 225, 38,  5,   143, 136, 68,
    3,   47,  222, 110, 120, 246, 167, 202, 30,  64,  192, 113, 187, 20,  209,
    5,   19,  35,  161, 55,  99,  167, 153, 242, 153, 84,  169, 135, 224, 148,
    140, 22,  18,  148, 5,   145, 196, 201, 20,  216, 6,   183, 82,  73,  62,
    87,  36,  44,  76,  29,  120, 103, 124, 253, 87,  100, 218, 122, 245, 84,
    26,  140, 240, 97,  130, 134, 92,  104, 200, 22,  60,  26,  130, 227, 203,
    220, 139, 80,  219, 111, 129, 53,  194, 0,   185, 59,  50,  147, 22,  2,
    25,  58,  255, 70,  194, 103, 39,  5,   143, 199, 14,  63,  41,  37,  111,
    200, 181, 194, 119, 182, 175, 185, 182, 254, 90,  205, 250, 56,  35,  47,
    214, 38,  249, 166, 235, 34,  66,  177, 210, 132, 1,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   150, 107, 12,
    124, 196, 77,  15,  79,  155, 216, 235, 209, 100, 122, 149, 249, 157, 89,
    67,  210, 148, 56,  142, 81,  28,  163, 148, 208, 127, 138, 114, 131, 23,
    164, 155, 40,  100, 94,  242, 163, 88,  183, 181, 36,  19,  103, 5,   63,
    60,  235, 84,  80,  196, 17,  56,  251, 100, 214, 7,   143, 210, 12,  216,
    133, 119, 11,  133, 168, 250, 105, 120, 203, 125, 206, 178, 6,   227, 250,
    106, 23,  147, 20,  33,  100, 193, 139, 236, 71,  123, 126, 3,   82,  117,
    176, 47,  8,   74,  59,  111, 112, 95,  65,  253, 25,  129, 46,  119, 141,
    194, 161, 69,  95,  77,  169, 167, 232, 64,  188, 94,  92,  88,  131, 225,
    32,  248, 11,  226, 22,  29,  178, 49,  93,  246, 21,  2,   57,  104, 185,
    162, 71,  192, 133, 30,  192, 181, 133, 235, 240, 44,  84,  146, 30,  220,
    199, 56,  4,   123, 81,  147, 1,   116, 75,  114, 57,  186, 124, 103, 222,
    252, 87,  49,  106, 43,  42,  94,  200, 87,  93,  224, 25,  207, 65,  27,
    194, 70,  179, 167, 124, 134, 174, 221, 25,  91,  100, 85,  168, 54,  69,
    92,  205, 156, 50,  122, 149, 41,  184, 177, 13,  222, 84,  147, 191, 160,
    32,  88,  86,  224, 30,  6,   171, 107, 175, 51,  42,  85,  114, 224, 19,
    26,  101, 58,  148, 98,  155, 69,  195, 246, 144, 148, 161, 195, 117, 140,
    50,  149, 91,  56,  166, 196, 7,   65,  41,  142, 127, 141, 5,   106, 59,
    97,  110, 21,  101, 115, 49,  188, 125, 242, 25,  35,  102, 134, 196, 98,
    154, 226, 68,  209, 238, 65,  189, 86,  114, 68,  226, 250, 201, 138, 13,
    1,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   194, 190, 145, 230, 207, 201, 92,  213, 120, 15,  23,  70,  11,
    109, 176, 69,  68,  7,   167, 163, 188, 71,  92,  5,   161, 253, 74,  20,
    162, 75,  100, 165, 194, 190, 145, 230, 207, 201, 92,  213, 120, 15,  23,
    70,  11,  109, 176, 69,  68,  7,   167, 163, 188, 71,  92,  5,   161, 253,
    74,  20,  162, 75,  100, 165};

class Fibonacci3CircuitTest : public CircuitTest {};

}  // namespace

TEST_F(Fibonacci3CircuitTest, Configure) {
  ConstraintSystem<F> constraint_system;
  Fibonacci3Config<F> config =
      Fibonacci3Circuit<F, SimpleFloorPlanner>::Configure(constraint_system);
  EXPECT_EQ(config.selector, Selector::Simple(0));
  EXPECT_EQ(config.a, AdviceColumnKey(0));
  EXPECT_EQ(config.b, AdviceColumnKey(1));
  EXPECT_EQ(config.c, AdviceColumnKey(2));
  EXPECT_EQ(config.a_equals_b.value_inv(), AdviceColumnKey(4));
  EXPECT_EQ(
      config.a_equals_b.expr(),
      ExpressionFactory<F>::Sum(
          ExpressionFactory<F>::Constant(F::One()),
          ExpressionFactory<F>::Negated(ExpressionFactory<F>::Product(
              ExpressionFactory<F>::Sum(
                  ExpressionFactory<F>::Advice(
                      AdviceQuery(0, Rotation::Cur(), config.a)),
                  ExpressionFactory<F>::Negated(ExpressionFactory<F>::Advice(
                      AdviceQuery(1, Rotation::Cur(), config.b)))),
              ExpressionFactory<F>::Advice(AdviceQuery(
                  2, Rotation::Cur(), config.a_equals_b.value_inv()))))));
  EXPECT_EQ(config.output, AdviceColumnKey(3));
  EXPECT_EQ(constraint_system.num_fixed_columns(), 0);
  EXPECT_EQ(constraint_system.num_advice_columns(), 5);
  EXPECT_EQ(constraint_system.num_instance_columns(), 0);
  EXPECT_EQ(constraint_system.num_selectors(), 1);
  EXPECT_EQ(constraint_system.num_challenges(), 0);
  std::vector<Phase> expected_advice_column_phases = {
      kFirstPhase, kFirstPhase, kFirstPhase, kFirstPhase, kFirstPhase};
  EXPECT_EQ(constraint_system.advice_column_phases(),
            expected_advice_column_phases);
  EXPECT_TRUE(constraint_system.challenge_phases().empty());
  EXPECT_TRUE(constraint_system.selector_map().empty());
  std::vector<Gate<F>> expected_gates;
  std::vector<std::unique_ptr<Expression<F>>> is_zero_polys;
  {
    std::unique_ptr<Expression<F>> poly = ExpressionFactory<F>::Product(
        ExpressionFactory<F>::Product(
            ExpressionFactory<F>::Selector(Selector::Simple(0)),
            ExpressionFactory<F>::Sum(
                ExpressionFactory<F>::Advice(
                    AdviceQuery(0, Rotation::Cur(), AdviceColumnKey(0))),
                ExpressionFactory<F>::Negated(ExpressionFactory<F>::Advice(
                    AdviceQuery(1, Rotation::Cur(), AdviceColumnKey(1)))))),
        ExpressionFactory<F>::Sum(
            ExpressionFactory<F>::Constant(F::One()),
            ExpressionFactory<F>::Negated(ExpressionFactory<F>::Product(
                ExpressionFactory<F>::Sum(
                    ExpressionFactory<F>::Advice(
                        AdviceQuery(0, Rotation::Cur(), AdviceColumnKey(0))),
                    ExpressionFactory<F>::Negated(ExpressionFactory<F>::Advice(
                        AdviceQuery(1, Rotation::Cur(), AdviceColumnKey(1))))),
                ExpressionFactory<F>::Advice(
                    AdviceQuery(2, Rotation::Cur(), AdviceColumnKey(4)))))));
    is_zero_polys.push_back(std::move(poly));
  }
  expected_gates.push_back(Gate<F>("is_zero", {""}, std::move(is_zero_polys),
                                   {Selector::Simple(0)},
                                   {
                                       {AdviceColumnKey(0), Rotation::Cur()},
                                       {AdviceColumnKey(1), Rotation::Cur()},
                                       {AdviceColumnKey(4), Rotation::Cur()},
                                   }));

  std::vector<std::unique_ptr<Expression<F>>> function_polys;
  function_polys.emplace_back(ExpressionFactory<F>::Product(
      ExpressionFactory<F>::Selector(Selector::Simple(0)),
      ExpressionFactory<F>::Product(
          ExpressionFactory<F>::Sum(
              ExpressionFactory<F>::Constant(F::One()),
              ExpressionFactory<F>::Negated(ExpressionFactory<F>::Product(
                  ExpressionFactory<F>::Sum(
                      ExpressionFactory<F>::Advice(
                          AdviceQuery(0, Rotation::Cur(), AdviceColumnKey(0))),
                      ExpressionFactory<F>::Negated(
                          ExpressionFactory<F>::Advice(AdviceQuery(
                              1, Rotation::Cur(), AdviceColumnKey(1))))),
                  ExpressionFactory<F>::Advice(
                      AdviceQuery(2, Rotation::Cur(), AdviceColumnKey(4)))))),
          ExpressionFactory<F>::Sum(
              ExpressionFactory<F>::Advice(
                  AdviceQuery(4, Rotation::Cur(), AdviceColumnKey(3))),
              ExpressionFactory<F>::Negated(ExpressionFactory<F>::Advice(
                  AdviceQuery(3, Rotation::Cur(), AdviceColumnKey(2))))))));
  function_polys.emplace_back(ExpressionFactory<F>::Product(
      ExpressionFactory<F>::Product(
          ExpressionFactory<F>::Selector(Selector::Simple(0)),
          ExpressionFactory<F>::Sum(
              ExpressionFactory<F>::Constant(F::One()),
              ExpressionFactory<F>::Negated(ExpressionFactory<F>::Sum(
                  ExpressionFactory<F>::Constant(F::One()),
                  ExpressionFactory<F>::Negated(ExpressionFactory<F>::Product(
                      ExpressionFactory<F>::Sum(
                          ExpressionFactory<F>::Advice(AdviceQuery(
                              0, Rotation::Cur(), AdviceColumnKey(0))),
                          ExpressionFactory<F>::Negated(
                              ExpressionFactory<F>::Advice(AdviceQuery(
                                  1, Rotation::Cur(), AdviceColumnKey(1))))),
                      ExpressionFactory<F>::Advice(AdviceQuery(
                          2, Rotation::Cur(), AdviceColumnKey(4))))))))),
      ExpressionFactory<F>::Sum(
          ExpressionFactory<F>::Advice(
              AdviceQuery(4, Rotation::Cur(), AdviceColumnKey(3))),
          ExpressionFactory<F>::Negated(ExpressionFactory<F>::Sum(
              ExpressionFactory<F>::Advice(
                  AdviceQuery(0, Rotation::Cur(), AdviceColumnKey(0))),
              ExpressionFactory<F>::Negated(ExpressionFactory<F>::Advice(
                  AdviceQuery(1, Rotation::Cur(), AdviceColumnKey(1)))))))));
  expected_gates.push_back(Gate<F>("f(a, b, c) = if a == b {c} else {a - b}",
                                   {"", ""}, std::move(function_polys),
                                   {Selector::Simple(0)},
                                   {
                                       {AdviceColumnKey(0), Rotation::Cur()},
                                       {AdviceColumnKey(1), Rotation::Cur()},
                                       {AdviceColumnKey(2), Rotation::Cur()},
                                       {AdviceColumnKey(3), Rotation::Cur()},
                                   }));
  EXPECT_EQ(constraint_system.gates(), expected_gates);
  std::vector<AdviceQueryData> expected_advice_queries = {
      AdviceQueryData(Rotation::Cur(), AdviceColumnKey(0)),
      AdviceQueryData(Rotation::Cur(), AdviceColumnKey(1)),
      AdviceQueryData(Rotation::Cur(), AdviceColumnKey(4)),
      AdviceQueryData(Rotation::Cur(), AdviceColumnKey(2)),
      AdviceQueryData(Rotation::Cur(), AdviceColumnKey(3)),
  };
  EXPECT_EQ(constraint_system.advice_queries(), expected_advice_queries);
  std::vector<size_t> expected_num_advice_queries = {1, 1, 1, 1, 1};
  EXPECT_EQ(constraint_system.num_advice_queries(),
            expected_num_advice_queries);
  EXPECT_TRUE(constraint_system.instance_queries().empty());
  EXPECT_TRUE(constraint_system.fixed_queries().empty());
  EXPECT_TRUE(constraint_system.permutation().columns().empty());
  EXPECT_TRUE(constraint_system.lookups().empty());
  EXPECT_TRUE(constraint_system.general_column_annotations().empty());
  EXPECT_FALSE(constraint_system.minimum_degree().has_value());
}

TEST_F(Fibonacci3CircuitTest, Synthesize) {
  size_t n = 16;
  CHECK(prover_->pcs().UnsafeSetup(n, F(2)));
  prover_->set_domain(Domain::Create(n));
  const Domain* domain = prover_->domain();

  ConstraintSystem<F> constraint_system;
  Fibonacci3Config config =
      Fibonacci3Circuit<F, SimpleFloorPlanner>::Configure(constraint_system);
  Assembly<PCS> assembly =
      VerifyingKey<PCS>::CreateAssembly(domain, constraint_system);

  Fibonacci3Circuit<F, SimpleFloorPlanner> circuit;
  circuit.floor_planner().Synthesize(&assembly, circuit, std::move(config),
                                     constraint_system.constants());

  EXPECT_TRUE(assembly.fixed_columns().empty());

  EXPECT_TRUE(assembly.permutation().columns().empty());

  const CycleStore& cycle_store = assembly.permutation().cycle_store();
  EXPECT_TRUE(cycle_store.mapping().IsEmpty());
  EXPECT_TRUE(cycle_store.aux().IsEmpty());
  EXPECT_TRUE(cycle_store.sizes().IsEmpty());

  // clang-format off
  std::vector<std::vector<bool>> expected_selectors = {
      { true, false, false, false, false, false, false, false,
       false, false, false, false, false, false, false, false}};
  // clang-format on
  EXPECT_EQ(assembly.selectors(), expected_selectors);
  EXPECT_EQ(assembly.usable_rows(), base::Range<size_t>::Until(10));
}

TEST_F(Fibonacci3CircuitTest, LoadVerifyingKey) {
  size_t n = 16;
  CHECK(prover_->pcs().UnsafeSetup(n, F(2)));
  prover_->set_domain(Domain::Create(n));

  Fibonacci3Circuit<F, SimpleFloorPlanner> circuit;

  VerifyingKey<PCS> vkey;
  ASSERT_TRUE(vkey.Load(prover_.get(), circuit));

  EXPECT_TRUE(vkey.permutation_verifying_key().commitments().empty());

  std::vector<Commitment> expected_fixed_commitments;
  {
    std::vector<Point> points = {
        {"0x0006d246b1045b5cf7ef706abdca51d7f88992335199a85f594360f81b3435a9",
         "0x0ce5093fe91a56ef9d54ef73d457aa726be43905202132cd98cb1893cfee96a"
         "6"},
    };
    expected_fixed_commitments = CreateCommitments(points);
  }
  EXPECT_EQ(vkey.fixed_commitments(), expected_fixed_commitments);

  F expected_transcript_repr = F::FromHexString(
      "0x015bd699cd46cf1250b4b2b2553645628551f6c76bd3e5cc995a355972340be4");
  EXPECT_EQ(vkey.transcript_repr(), expected_transcript_repr);
}

TEST_F(Fibonacci3CircuitTest, LoadProvingKey) {
  size_t n = 16;
  CHECK(prover_->pcs().UnsafeSetup(n, F(2)));
  prover_->set_domain(Domain::Create(n));

  Fibonacci3Circuit<F, SimpleFloorPlanner> circuit;

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
          "0x2d5e098bb31e86271ccb415b196942d755b0a9c3f21dd9882fa3d63ab100000"
          "1",
          "0x2d5e098bb31e86271ccb415b196942d755b0a9c3f21dd9882fa3d63ab100000"
          "1",
          "0x2d5e098bb31e86271ccb415b196942d755b0a9c3f21dd9882fa3d63ab100000"
          "1",
          "0x2d5e098bb31e86271ccb415b196942d755b0a9c3f21dd9882fa3d63ab100000"
          "1",
          "0x2d5e098bb31e86271ccb415b196942d755b0a9c3f21dd9882fa3d63ab100000"
          "1",
          "0x2d5e098bb31e86271ccb415b196942d755b0a9c3f21dd9882fa3d63ab100000"
          "1",
          "0x2d5e098bb31e86271ccb415b196942d755b0a9c3f21dd9882fa3d63ab100000"
          "1",
          "0x2d5e098bb31e86271ccb415b196942d755b0a9c3f21dd9882fa3d63ab100000"
          "1",
          "0x2d5e098bb31e86271ccb415b196942d755b0a9c3f21dd9882fa3d63ab100000"
          "1",
          "0x2d5e098bb31e86271ccb415b196942d755b0a9c3f21dd9882fa3d63ab100000"
          "1",
          "0x2d5e098bb31e86271ccb415b196942d755b0a9c3f21dd9882fa3d63ab100000"
          "1",
          "0x2d5e098bb31e86271ccb415b196942d755b0a9c3f21dd9882fa3d63ab100000"
          "1",
          "0x2d5e098bb31e86271ccb415b196942d755b0a9c3f21dd9882fa3d63ab100000"
          "1",
          "0x2d5e098bb31e86271ccb415b196942d755b0a9c3f21dd9882fa3d63ab100000"
          "1",
          "0x2d5e098bb31e86271ccb415b196942d755b0a9c3f21dd9882fa3d63ab100000"
          "1",
          "0x2d5e098bb31e86271ccb415b196942d755b0a9c3f21dd9882fa3d63ab100000"
          "1",
      };
      expected_l_first = CreatePoly(poly);
    }
    EXPECT_EQ(pkey.l_first(), expected_l_first);

    Poly expected_l_last;
    {
      std::vector<std::string_view> poly = {
          "0x2d5e098bb31e86271ccb415b196942d755b0a9c3f21dd9882fa3d63ab100000"
          "1",
          "0x2014447de15a99b6df03833e95f96ae1299c9ec6ff990b6e75fa3b3b04846a5"
          "7",
          "0x0f1f5883e65f820d14d56342dc92fd12a944d4cbbdce5377b7439bd07108fc9"
          "d",
          "0x02b337de1c8c14f22ec9b9e2f96afef3652627366f8170a0a948dad4ac1bd5e"
          "8",
          "0x030644e72e131a029b85045b68181585d2833e84879b9709143e1f593f00000"
          "0",
          "0x105009f4ffd70672d94cc277eb87ed7bfe9749817a206522cde7ba58eb7b95a"
          "a",
          "0x2144f5eefad21e1ca37ae273a4ee5b4a7eef137cbbeb1d198c9e59c37ef7036"
          "4",
          "0x2db11694c4a58b3789868bd388165969c30dc1120a37fff09a991abf43e42a1"
          "9",
          "0x2d5e098bb31e86271ccb415b196942d755b0a9c3f21dd9882fa3d63ab100000"
          "1",
          "0x2014447de15a99b6df03833e95f96ae1299c9ec6ff990b6e75fa3b3b04846a5"
          "7",
          "0x0f1f5883e65f820d14d56342dc92fd12a944d4cbbdce5377b7439bd07108fc9"
          "d",
          "0x02b337de1c8c14f22ec9b9e2f96afef3652627366f8170a0a948dad4ac1bd5e"
          "8",
          "0x030644e72e131a029b85045b68181585d2833e84879b9709143e1f593f00000"
          "0",
          "0x105009f4ffd70672d94cc277eb87ed7bfe9749817a206522cde7ba58eb7b95a"
          "a",
          "0x2144f5eefad21e1ca37ae273a4ee5b4a7eef137cbbeb1d198c9e59c37ef7036"
          "4",
          "0x2db11694c4a58b3789868bd388165969c30dc1120a37fff09a991abf43e42a1"
          "9",
      };
      expected_l_last = CreatePoly(poly);
    }
    EXPECT_EQ(pkey.l_last(), expected_l_last);

    Poly expected_l_active_row;
    {
      std::vector<std::string_view> poly = {
          "0x12259d6b14729c0fa51e1a2470908122ef13771b2da58a367974bc177a00000"
          "1",
          "0x1a8133201ba2fe22aff30c7fd04fadcb17ceab0715a13371b06d35acd9695b3"
          "d",
          "0x0d49c50dd1c3ec703dc7be1c836fd7f62c140afcf284ce19b9a99affac7b95a"
          "a",
          "0x117dae38a05bcb8c7c290ee16cec493d73ef8dafa5c61fa4a2efd9a39e63abf"
          "4",
          "0x0c19139cb84c680a79505ee7747ae78cd6c196473632bc6ea3057c773208fc9"
          "d",
          "0x136156e428a2662bc2fddfd3b39f6475dafecb8699a611f3da6edd22c3479af"
          "1",
          "0x2aaad1ad96927134ee0187781ffe43e3f08a828d829c68e7865afb6604e42a1"
          "9",
          "0x28d7d254c17b7ea40ebc4659996adacebd0d8f52d021284040d407c2f33b896"
          "f",
          "0x000000000000000000000000000000000000000000000000000000000000000"
          "0",
          "0x1ef7221577b1e8f8cebdcb2fcd10296b6d9d43267b395c820757c0fc87681d8"
          "1",
          "0x1d0dff96b3477fb4437e7ee32de1555b5719604277fd746561bc1be1c5846a5"
          "7",
          "0x1be193603aacb384678281793fa8f20b949cf5976d9493017ba8a357ee49da5"
          "7",
          "0x1e3eb107ccbf041a07f5de183cd645c4ac6bd4f8344f861078603a6a3ff7036"
          "4",
          "0x20080468beb85b16c9f71b3ea2ce10fb6cdc81c346721c888ebc9109900adec"
          "6",
          "0x30114169cfaa9b194b94fb3e12d441cabad6d0fa619f4a28d8ecb10f5d1bd5e"
          "9",
          "0x2edcc3ce4ec47abd9b83b31a4db9571236223b590c30997fd30ce24f7bf2fdd"
          "6",
      };
      expected_l_active_row = CreatePoly(poly);
    }
    EXPECT_EQ(pkey.l_active_row(), expected_l_active_row);

    std::vector<Evals> expected_fixed_columns;
    {
      // clang-format off
      std::vector<std::vector<std::string_view>> evals = {{
          "0x0000000000000000000000000000000000000000000000000000000000000001",
          "0x0000000000000000000000000000000000000000000000000000000000000000",
          "0x0000000000000000000000000000000000000000000000000000000000000000",
          "0x0000000000000000000000000000000000000000000000000000000000000000",
          "0x0000000000000000000000000000000000000000000000000000000000000000",
          "0x0000000000000000000000000000000000000000000000000000000000000000",
          "0x0000000000000000000000000000000000000000000000000000000000000000",
          "0x0000000000000000000000000000000000000000000000000000000000000000",
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
          "0x18afdf23e9bd9302673fc1e076a492d4d65bd18ebc4d854ed189139bab313e52",
          "0x2f0e061e83e8b12c1bdf8df7cca02483295e89f78a462ce38859661e06501815",
          "0x133f51f46c2a51201fbb89dc1d6868acfef0f7ce0e572ccb98e04e99eac3ea36",
          "0x1240a374ee6f71e12df10a129946ca9ba824f58a50e3a58b4c68dd5590b74ad8",
          "0x009553089042fe83ab570b54e4e64d307d8e8b20d568e782ece3926981d0a96c",
          "0x14a6c152ace4b16a42e1377e400798e15ac60320c21d90277890dbdd551d1912",
          "0x035992598be4d2ae5334f24f30355a1fca7f0e28762a178d79e993dca70eaabf",
          "0x03b645319eb70d7a692ea8c87fbcab1f292cd502071d9ffb7291ebe28356aa07",
          "0x30644e72e131a029b85045b68181585d2833e84879b9709143e1f593f0000000",
          "0x0f5c21d0ca65e0db9be1f670eca407d08ec5ec67626a74f892ccebcd0cf9b9f6",
          "0x0530d09118705106cbb4a786ead16926d5d174e181a26686af5448492e42a181",
          "0x1fe9a328fad7382fddb3730a89f574d14e57caeac619eeb30d24fb38a4fc6fbe",
          "0x0000000000000000b3c4d79d41a91758cb49c3517c4604a520cff123608fc9cb",
          "0x0dd360411caed09700b52c71a6655715c4d558439e2d34f4307da9a4be13c42e",
          "0x130b17119778465cfb3acaee30f81dee20710ead41671f568b11d9ab07b95a9b",
          "0x02e40daf409556c02bfc85eb303402b774954d30aeb0337eb85a71e6373428de",
      },
      {
          "0x21082ca216cbbf4e1c6e4f4594dd508c996dfbe1174efb98b11509c6e306460b",
          "0x13b360d4e82fe915fed16081038f98c211427b87a281bd733c277dbadf10372b",
          "0x08e7cbfea108224b0777f0558503af41585b75ab8d4d807505158f4bc8c771de",
          "0x0b6f861977ce57ddb2e647048ed9b9433cac640ba0599e264e24446765d915d3",
          "0x26f93d99832c6285d9abf8c5b896ea753ed137516972c5ddcc464b56c488d600",
          "0x086398ace043cf0db4e99f2712f392ddbd9c7b9202b2fcadf26e9b145316df9f",
          "0x04765b5102d6627cfcc389436e96bbc9aa478dcb720b546c77063077a40910ce",
          "0x2e39c17d3e15071a9341cfcea233ad5f25a14dea28716de88ae369ac2c9944ac",
          "0x2741e304be6aaf5f53641f0bacb8e9ebccd45eba1b23316bbcd39ed80acc165f",
          "0x1d24fc7e75074f099894bbda6418efb02942f07a6b6243c5ab01a6fa053c15cb",
          "0x1e23aafdf2c22e488a5f3ba3e83a8dc1800ef2be28d5cb05f779183e5f48b529",
          "0x2fcefb6a50eea1a60cf93a619c9b0b2caaa55d27a450890e56fe632a6e2f5695",
          "0x1bbd8d20344ceebf756f0e384179bf7bcd6de527b79be069cb5119b69ae2e6ef",
          "0x2d0abc19554ccd7b651b5367514bfe3d5db4da20038f5903c9f861b748f15542",
          "0x2cae0941427a92af4f219cee01c4ad3dff071346729bd095d15009b16ca955fa",
          "0x0d4615fec1d5611cd5ffa9e358e64eb494829d350acf01c136f55acac8e3624c",
      },
      {
          "0x2b337de1c8c14f22ec9b9e2f96afef3652627366f8170a0a948dad4ac1bd5e80",
          "0x107aab49e65a67f9da9cd2abf78be38bd9dc1d5db39f81de36bcfa5b4b039043",
          "0x30644e72e131a029048b6e193fd841045cea24f6fd736bec231204708f703636",
          "0x2290ee31c482cf92b79b1944db1c0147635e9004db8c3b9d13644bef31ec3bd3",
          "0x1d59376149b959ccbd157ac850893a6f07c2d99b3852513ab8d01be8e846a566",
          "0x2d8040c3a09c49698c53bfcb514d55a5b39e9b17cb093d128b8783adb8cbd723",
          "0x231e38741f5c3f0ce2509bd3289b09a893b14b136eea6ed00cec9ac9271c9db5",
          "0x034183d253b6b250dae0a457797029523434d2b6bc41c09b6ef409bd970e4208",
          "0x1cb0ed9df901b713b97ee5357df1bf9b16f16cc0d737b31e07ba77d910efc8d6",
          "0x277c827440297ddeb0d85560fc7da91bcfd8729cec6bf01c3ecc664827388e23",
          "0x24f4c8596963484c0569feb1f2a79f19eb87843cd95fd26af5bdb12c8a26ea2e",
          "0x096b10d95e053da3dea44cf0c8ea6de7e962b0f71046aab3779baa3d2b772a01",
          "0x2800b5c600edd11c0366a68f6e8dc57f6a976cb6770673e351735a7f9ce92062",
          "0x2bedf321de5b3dacbb8cbc7312ea9c937dec5a7d07ae1c24ccdbc51c4bf6ef33",
          "0x022a8cf5a31c990f250e75e7df4daafe02929a5e514802a8b8fe8be7c366bb55",
          "0x06272e83847dd219527e22d89b87a6efdf7fc2b0f32d564762853d937378875d",
      },
      {
          "0x0000000000000000000000000000000000000000000000000000000000000001",
          "0x09226b6e22c6f0ca64ec26aad4c86e715b5f898e5e963f25870e56bbe533e9a2",
          "0x2a3d1fef5cb3ce1065d222dde5f9b16d48b42597868c1a49e15cb8007c8778a4",
          "0x0f34fda7cd268bf095354ea6c2631826c349d7518bf094361bc91f61c519c7fb",
          "0x2f549305063b1803a77ba92486c5383e00468fc857ec66d944868c4a309e596a",
          "0x1e626bf9ef3c8920522383ff9be21287c3af8d47fe61ff9af6c6d8d0118154bc",
          "0x02898db49f7022cac0aeabf721a3c7ab96e14ad35e7d6b0410b868ff504b62fc",
          "0x2a150ee7450ad90cace368ea424c8fdf36b3821e79f5f1c617a5f8e74c19b4cf",
          "0x17b46f4ef7740d27511083d60adcc58851d816b9bd6beb427258e1f844cec1af",
          "0x015648545d48eefd9c70b7beb4e133d9fed55e50ef7343adbb888f75e9afe7ec",
          "0x2d22caa08d7aedd8dd6fa15f08112f0af3ff1591bd77aff5d4edebd658f1bdf9",
          "0x212f50cb140b1439231af70fbf1e403664ea10f6edc8dc5b2818d6322ae63806",
          "0x010fbb6ddaf6882610d49c91fabc201f27ed588021cd09b7ff5b6949bf61a697",
          "0x1201e278f1f51709662cc1b6e59f45d564845b007b5770f64d1b1cc3de7eab45",
          "0x2ddac0be41c17d5ef7a199bf5fdd90b191529d751b3c058d33298c949fb49d05",
          "0x064f3f8b9c26c71d0b6cdccc3f34c87df1806629ffc37ecb2c3bfcaca3e64b32",
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
          "0x112272f91e39f0f3754b36d52fafaa32b73194639e9077aeeb8fd9eeb5c197f6",
          "0x2c5b7efa2ecceeea67640c3a314bf22430a3635c16e265bf41b69cae74bb6ca8",
          "0x2b63732376158eeb59affea24c42fe2b9f5f0ed91512e866a5010c4a422a2210",
          "0x15ef7f2ee3cb842ed2df84652b6d941208ee251aa1d3d440fdef29ab9c91123d",
          "0x1efdce0e537a38a6167bb190151b64ce84ad66c093e64acda84995afbad32c38",
          "0x15fccfcbfbb7d19b3579da7e187ea1bfc00c071f5fcc688c162006b8fb3802c1",
          "0x044d3c43a1d8bdf98d9ce3973799577b3cc148f71f05c1381c7b28a42b010bdb",
          "0x2601b44a9ceaf52a979de420fd3c6a7d8ef043cd6b845368cf317809c7603844",
          "0x2238916f0b1084c82c027898e274eb46c7d8ac0c96a2b245d563706526ea705e",
          "0x0000000000000000000000000000000000000000000000000000000000000000",
          "0x2c8157f94657d88c59af021da365441045bc63e1b38b4487c91b519f9ff62533",
          "0x27fb1c055aaf8a4205acd5be9a2be2e6718de3cb2c83c8e64a3dc0676bb8a945",
          "0x14c116fa641eb6536a6e13a3830fb449d47d31606dd888bdd65630ee6be537d8",
          "0x27edcb6842c33cd5a3127fa5ad1ad538ba7001c66e8b349b320ce35a0d11b8c1",
          "0x22cf5fc5ab1490168dd5c1acb0870ec4a605e9ca6353518e5012c3677d12dfab",
          "0x17e8e6e9a190194640ee7602c85d0c7aeb8bc51862d349be78fb720940e9833e",
      },
      {
          "0x1f09a0f7ba26bdc402f8617328d38bce213bad6def415a22398d2727cbfda3c6",
          "0x27918de28986445f3b86531a98ae3c84a22efadff737cb6654200067594f146b",
          "0x0e4adbd349654830f53e209fe0803dc6c9b38c1bd257ffeb4719661b05bca7c8",
          "0x07ba99b0454b12d883a90b7d9fc585a570a5609c117948673d653b75b4f8dbeb",
          "0x05ee0abc9f39a24a6c0448af3b29cc5b70b02961d28a7c75c4bf5665b6e33de3",
          "0x07ba99b0454b12d883a90b7d9fc585a570a5609c117948673d653b75b4f8dbeb",
          "0x089d2cd7436de0d9bdd87214d79aebde9f5f670221c1f5bc7a6905fa720e17ff",
          "0x07ba99b0454b12d883a90b7d9fc585a570a5609c117948673d653b75b4f8dbeb",
          "0x2dc0c43913c4431c08ed1d329bd5dddd857f8aec88417ee833a16c5814c27120",
          "0x07ba99b0454b12d883a90b7d9fc585a570a5609c117948673d653b75b4f8dbeb",
          "0x2ff80704d0f96302a799aa96eeff44148da396e5b3114bb690e7167eacf7da6f",
          "0x07ba99b0454b12d883a90b7d9fc585a570a5609c117948673d653b75b4f8dbeb",
          "0x24b74ff3c01e251b3a5e0ffe31b8ddb64b8fd2879fd76451063d87485fac338f",
          "0x07ba99b0454b12d883a90b7d9fc585a570a5609c117948673d653b75b4f8dbeb",
          "0x2766720e645263aa8c323963ce29fd397a3f1027ef0edd97bbe736dd1ad70dab",
          "0x07ba99b0454b12d883a90b7d9fc585a570a5609c117948673d653b75b4f8dbeb",
      },
      {
          "0x19e45f96ed87e5016568eaf3d9ddfa979cde528c9a9f889ea6bca69314ae07a8",
          "0x113ad62e5f1760833e3c623377b1da8c53b6df849f9d26ebb0dff4c13d89c129",
          "0x06a1a39fdf7310f4c8bd3dc6ba194a8c2c064994f519c4ad0bab55e6044ec707",
          "0x14ac062955c7bf10fba42f8d8917de76b0bf5acda89874fe70a323ce68dd67a7",
          "0x0608fe05ef99aa7bfd9e1d5c20d096da7e14e4bbf4c3757ecfb29aa624682461",
          "0x1e4d544947af4acf44e5a2f787700883bcf361afc726f707c15ecd73bf2991a2",
          "0x0c8b10d3d236bf6bb5d7855a3610b44707a62b6bf4cc12c5c7ea30fa87afe7ce",
          "0x0da3a967ee55218c7aeb0e79471780de9df3e0e43b99ab84492e5cee6178812c",
          "0x1f49ce3e263fe94985bc9c4fc8a6da078e88183b84f2d5eccbdd23322ed0e52a",
          "0x2f854e8576b393f8281a4ee34765fae491d456b5ebc255984aee6d1a6bfddb24",
          "0x251d65931d09aea2d18ce4339b3df0c44d97bcb6c89c5352b7033b959b751b4e",
          "0x126bc9da37891c2fb865fc63042ed2d0eac5970369a4c64b02e5c21180fb9905",
          "0x2caedffb118383bf855a6d2133dac4c209be168701ea35614d2972d596102a16",
          "0x08fc6c0a8cc8cd105ef293dbc8d9ea934ba1bbf9b15c190d110056914fb59ce8",
          "0x10eefb3ba0dde80ebdbe59c08c3cc0acf8c8112a8c117c16ffbb599693eb8e42",
          "0x2c0d237a46bd438641ab51032ea47e9416eefddba59bfa54cb0ca85994ae7e2a",
      },
      {
          "0x16b8295dfc7aac9a92f40830d0a18021db1c3c32cb0186b2bbea437e4992bc9e",
          "0x060b53de43d5419e8776eaefc4651c57b587045d5ac2ae857fe87be466e6c9d4",
          "0x2ac49b1b54eb7dffed2e32d42b9b780c717802960ace5b4be230326426266058",
          "0x26370289d5f64795d58f0d5cf98a2e694311753f5c5f0fc453d79600c3771454",
          "0x05b3d1abef33209714601dd3dfd3067cdf6aa1e9122d8e22a05ea1bcffbb9c6c",
          "0x300297d66a50a1974cc152a3dc9dc5cb9337b131b3255c36cdada2d141ac49f4",
          "0x19eacc03d9e4d435d98cc9717e74a7c39011da0e8780d9e26c7ac11e1d45d05e",
          "0x0f1d418da7ee6f503af23a71e6423b0dd1fb15de160753b2b961c42fd97b34e4",
          "0x094fff48f42c489724b45af5ff0bf5c125c8a524c2d1fa34029ee95313b6426c",
          "0x2b460fee9892bd0cf8c38333135a007b59a73bb46cccc65df68c525bc3ff4704",
          "0x25f8c1e4a021048da4b79acff3497b746fc72cf27c7351128d8db0c38d0dfeff",
          "0x142b65b35f124182a2c1bc038acbdf94da71b68ed1af1b1f0788ff44f44f7d4c",
          "0x10956ff3bf348daac08be5b154b80d5f5c5a6ab649135c5881905f83a42f14f7",
          "0x1e119c3a2345365cdf28ad7c4801925298d3b68aa4669027b1c0c6302fd293f4",
          "0x2c71de2892808d0391c25b0369e19f351bc0b4f0cdc4c6438c0374f410b980ba",
          "0x1d360eeea3494b3491a1a8901b219910755f94931eb85d5d0f97ca305ff1e9ea",
      }};
      // clang-format on
      expected_permutations_polys = CreatePolys(polys);
    }
    EXPECT_EQ(pkey.permutation_proving_key().polys(),
              expected_permutations_polys);
  }
}

TEST_F(Fibonacci3CircuitTest, CreateProof) {
  size_t n = 16;
  CHECK(prover_->pcs().UnsafeSetup(n, F(2)));
  prover_->set_domain(Domain::Create(n));

  Fibonacci3Circuit<F, SimpleFloorPlanner> circuit;
  std::vector<Fibonacci3Circuit<F, SimpleFloorPlanner>> circuits = {
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

TEST_F(Fibonacci3CircuitTest, VerifyProof) {
  size_t n = 16;
  CHECK(prover_->pcs().UnsafeSetup(n, F(2)));
  prover_->set_domain(Domain::Create(n));

  Fibonacci3Circuit<F, SimpleFloorPlanner> circuit;

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
         "0x1d42911d6e4d1799ebf92175f0a9210e911ff8dce85cce34235704fb0fd3c6e"
         "3"},
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
         "0x1cae21f5360efe30b4c22549b03a77e390f71935cd4d13f8a512a9004e035af"
         "5"},
        {"0x10b2dfa3130614812395142f52e95d19b01656f9c54f737c582c4f1f3f7c3f45",
         "0x26b6ea518c416bb1533fd07d2b9719b14d4c3166a9e26388444240d723da868"
         "b"},
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
         "0x000000000000000000000000000000000000000000000000000000000000000"
         "2"});
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
         "0x26adf7792cd0a83e5739ea35e4836db5bd95b83cd9f431205f1489ce1070eec"
         "c"},
        {"0x11124fc40ee633a3db6dfa25a0fe227844da6d97b25bff1539b09934cfa2d7ca",
         "0x0b397bd2223a070097ee1780409dd5a1d85b0ad6b764a7d03878ed4e2ff5383"
         "b"},
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
