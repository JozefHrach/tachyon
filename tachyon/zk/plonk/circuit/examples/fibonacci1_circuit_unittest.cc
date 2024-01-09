#include "tachyon/zk/plonk/circuit/examples/fibonacci1_circuit.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "tachyon/zk/plonk/circuit/examples/circuit_test.h"
#include "tachyon/zk/plonk/circuit/floor_planner/simple_floor_planner.h"
#include "tachyon/zk/plonk/halo2/pinned_verifying_key.h"
#include "tachyon/zk/plonk/keys/proving_key.h"

namespace tachyon::zk::halo2 {

namespace {

constexpr uint8_t kExpectedProof[] = {
    144, 229, 155, 204, 124, 168, 31,  49,  47,  46,  92,  241, 59,  42,  185,
    207, 170, 63,  142, 222, 11,  96,  209, 55,  68,  158, 132, 196, 160, 37,
    58,  9,   11,  179, 5,   174, 11,  70,  229, 110, 251, 12,  31,  8,   37,
    111, 214, 202, 175, 178, 120, 225, 0,   82,  92,  48,  127, 152, 74,  122,
    163, 9,   254, 153, 49,  52,  6,   115, 102, 78,  171, 187, 144, 238, 90,
    52,  145, 83,  130, 239, 132, 191, 0,   148, 105, 169, 113, 24,  122, 67,
    220, 66,  79,  109, 109, 145, 215, 185, 20,  25,  8,   95,  65,  94,  241,
    47,  22,  239, 60,  158, 15,  50,  48,  37,  14,  243, 165, 251, 206, 113,
    202, 182, 105, 210, 20,  188, 112, 167, 160, 154, 26,  232, 195, 109, 87,
    214, 52,  119, 103, 23,  241, 34,  56,  77,  93,  212, 230, 105, 49,  91,
    103, 90,  11,  235, 92,  70,  120, 241, 188, 166, 95,  80,  233, 24,  197,
    250, 65,  97,  144, 90,  180, 63,  253, 120, 80,  160, 41,  216, 41,  138,
    115, 66,  144, 91,  112, 53,  201, 202, 239, 26,  10,  29,  94,  208, 120,
    229, 1,   10,  214, 54,  47,  145, 41,  173, 9,   194, 207, 251, 146, 191,
    9,   68,  146, 90,  193, 157, 106, 73,  74,  178, 54,  217, 98,  158, 1,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   219, 157, 171, 66,  103, 81,  22,  164, 19,  188, 56,  33,  158, 202,
    12,  198, 128, 106, 23,  67,  107, 103, 168, 110, 14,  179, 94,  167, 13,
    61,  179, 167, 50,  240, 123, 151, 105, 120, 101, 2,   15,  196, 211, 59,
    243, 218, 49,  0,   245, 52,  77,  198, 197, 24,  165, 161, 37,  228, 246,
    108, 71,  91,  32,  133, 149, 116, 138, 123, 109, 184, 57,  177, 127, 10,
    124, 141, 234, 255, 145, 129, 89,  34,  165, 204, 234, 181, 120, 50,  163,
    107, 95,  39,  140, 230, 158, 10,  192, 48,  15,  132, 58,  152, 138, 188,
    110, 196, 172, 33,  36,  144, 183, 102, 91,  105, 218, 82,  29,  172, 15,
    52,  157, 43,  244, 54,  47,  205, 59,  31,  241, 156, 247, 11,  161, 78,
    80,  165, 78,  49,  30,  151, 189, 118, 7,   50,  9,   230, 165, 205, 102,
    29,  203, 216, 246, 53,  202, 121, 132, 10,  148, 13,  46,  119, 34,  75,
    144, 185, 180, 241, 38,  44,  177, 108, 174, 33,  232, 128, 38,  111, 165,
    221, 32,  98,  193, 212, 104, 9,   230, 248, 83,  44,  241, 15,  1,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    113, 79,  191, 153, 44,  228, 167, 73,  34,  80,  131, 95,  91,  104, 126,
    205, 63,  17,  64,  95,  31,  62,  222, 186, 10,  60,  26,  191, 226, 212,
    77,  13,  221, 87,  131, 100, 197, 188, 127, 9,   104, 188, 116, 239, 24,
    167, 123, 201, 224, 198, 93,  34,  190, 156, 216, 174, 19,  46,  168, 32,
    143, 245, 90,  27,  50,  151, 180, 199, 125, 175, 253, 173, 83,  41,  9,
    233, 103, 238, 194, 116, 161, 13,  255, 73,  47,  216, 229, 188, 233, 57,
    223, 17,  43,  158, 142, 1,   209, 145, 204, 46,  194, 29,  40,  158, 25,
    126, 215, 21,  18,  123, 189, 3,   91,  133, 197, 64,  181, 146, 233, 11,
    235, 5,   175, 1,   75,  62,  177, 44,  121, 190, 248, 112, 86,  14,  237,
    63,  102, 98,  244, 66,  93,  7,   42,  81,  34,  127, 88,  136, 135, 151,
    131, 113, 84,  129, 238, 104, 217, 63,  111, 45,  5,   89,  186, 170, 90,
    189, 240, 118, 118, 73,  165, 111, 120, 44,  239, 173, 219, 234, 123, 223,
    162, 201, 72,  181, 152, 78,  207, 173, 33,  9,   39,  1,   198, 158, 50,
    86,  19,  119, 35,  195, 220, 93,  25,  13,  97,  227, 56,  135, 29,  87,
    230, 224, 52,  197, 46,  64,  48,  239, 114, 167, 84,  213, 189, 16,  136,
    235, 52,  22,  164, 43,  66,  213, 41,  221, 25,  187, 247, 194, 102, 183,
    39,  43,  246, 52,  153, 37,  185, 109, 138, 248, 18,  249, 146, 143, 33,
    44,  208, 10,  117, 76,  162, 227, 217, 181, 18,  94,  109, 153, 215, 198,
    8,   145, 63,  39,  67,  39,  17,  143, 23,  227, 53,  121, 117, 177, 211,
    45,  172, 17,  24,  253, 239, 97,  234, 75,  134, 84,  84,  21,  202, 171,
    32,  57,  251, 148, 218, 178, 111, 83,  24,  81,  23,  85,  106, 158, 55,
    193, 127, 77,  136, 40,  249, 124, 213, 246, 102, 138, 250, 95,  136, 151,
    223, 97,  202, 6,   4,   206, 14,  136, 23,  131, 79,  102, 221, 236, 126,
    3,   127, 240, 7,   40,  201, 22,  58,  87,  110, 132, 68,  196, 31,  15,
    197, 99,  190, 175, 0,   194, 125, 28,  6,   75,  132, 136, 46,  65,  28,
    113, 28,  242, 112, 166, 193, 161, 91,  34,  111, 25,  100, 38,  232, 194,
    201, 119, 23,  137, 80,  142, 17,  157, 63,  49,  157, 88,  251, 28,  160,
    162, 97,  155, 41,  216, 30,  86,  102, 143, 88,  3,   8,   85,  140, 140,
    37,  162, 32,  64,  9,   21,  25,  236, 68,  212, 175, 27,  165, 42,  17,
    117, 236, 45,  97,  58,  222, 187, 73,  45,  14,  56,  91,  11,  226, 42,
    64,  110, 114, 48,  71,  244, 200, 6,   67,  193, 57,  0,   135, 221, 74,
    173, 1,   83,  116, 3,   141, 27,  225, 243, 162, 60,  167, 174, 252, 3,
    45,  89,  237, 104, 78,  9,   93,  237, 141, 38,  154, 230, 42,  97,  246,
    195, 109, 178, 27,  218, 148, 53,  58,  74,  42,  76,  170, 183, 114, 198,
    171, 12,  91,  132, 53,  72,  40,  39,  24,  165, 65,  86,  3,   247, 147,
    129, 241, 238, 80,  202, 150, 53,  48,  93,  148, 158, 157, 17,  95,  181,
    208, 159, 109, 6};

class Fibonacci1CircuitTest : public CircuitTest {};

}  // namespace

TEST_F(Fibonacci1CircuitTest, Configure) {
  ConstraintSystem<F> constraint_system;
  Fibonacci1Config<F> config =
      Fibonacci1Circuit<F, SimpleFloorPlanner>::Configure(constraint_system);
  EXPECT_EQ(config.col_a, AdviceColumnKey(0));
  EXPECT_EQ(config.col_b, AdviceColumnKey(1));
  EXPECT_EQ(config.col_c, AdviceColumnKey(2));
  EXPECT_EQ(config.instance, InstanceColumnKey(0));
  EXPECT_EQ(config.selector, Selector::Simple(0));
  EXPECT_EQ(constraint_system.num_fixed_columns(), 0);
  EXPECT_EQ(constraint_system.num_advice_columns(), 3);
  EXPECT_EQ(constraint_system.num_instance_columns(), 1);
  EXPECT_EQ(constraint_system.num_selectors(), 1);
  EXPECT_EQ(constraint_system.num_challenges(), 0);
  std::vector<Phase> expected_advice_column_phases = {kFirstPhase, kFirstPhase,
                                                      kFirstPhase};
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
                                          0, Rotation::Cur(), config.col_a)),
                                      ExpressionFactory<F>::Advice(AdviceQuery(
                                          1, Rotation::Cur(), config.col_b))),
            ExpressionFactory<F>::Negated(ExpressionFactory<F>::Advice(
                AdviceQuery(2, Rotation::Cur(), config.col_c)))));
    polys.push_back(std::move(poly));
  }
  expected_gates.push_back(Gate<F>("add", {""}, std::move(polys),
                                   {Selector::Simple(0)},
                                   {
                                       {AdviceColumnKey(0), Rotation::Cur()},
                                       {AdviceColumnKey(1), Rotation::Cur()},
                                       {AdviceColumnKey(2), Rotation::Cur()},
                                   }));
  EXPECT_EQ(constraint_system.gates(), expected_gates);
  std::vector<AdviceQueryData> expected_advice_queries = {
      AdviceQueryData(Rotation::Cur(), AdviceColumnKey(0)),
      AdviceQueryData(Rotation::Cur(), AdviceColumnKey(1)),
      AdviceQueryData(Rotation::Cur(), AdviceColumnKey(2)),
  };
  EXPECT_EQ(constraint_system.advice_queries(), expected_advice_queries);
  std::vector<size_t> expected_num_advice_queries = {1, 1, 1};
  EXPECT_EQ(constraint_system.num_advice_queries(),
            expected_num_advice_queries);
  std::vector<InstanceQueryData> expected_instance_queries = {
      InstanceQueryData(Rotation::Cur(), InstanceColumnKey(0)),
  };
  EXPECT_EQ(constraint_system.instance_queries(), expected_instance_queries);
  EXPECT_TRUE(constraint_system.fixed_queries().empty());
  std::vector<AnyColumnKey> expected_permutation_columns = {
      AdviceColumnKey(0),
      AdviceColumnKey(1),
      AdviceColumnKey(2),
      InstanceColumnKey(0),
  };
  EXPECT_EQ(constraint_system.permutation().columns(),
            expected_permutation_columns);
  EXPECT_TRUE(constraint_system.lookups().empty());
  EXPECT_TRUE(constraint_system.general_column_annotations().empty());
  EXPECT_FALSE(constraint_system.minimum_degree().has_value());
}

TEST_F(Fibonacci1CircuitTest, Synthesize) {
  size_t n = 16;
  CHECK(prover_->pcs().UnsafeSetup(n, F(2)));
  prover_->set_domain(Domain::Create(n));
  const Domain* domain = prover_->domain();

  ConstraintSystem<F> constraint_system;
  Fibonacci1Config config =
      Fibonacci1Circuit<F, SimpleFloorPlanner>::Configure(constraint_system);
  Assembly<PCS> assembly =
      VerifyingKey<PCS>::CreateAssembly(domain, constraint_system);

  Fibonacci1Circuit<F, SimpleFloorPlanner> circuit;
  circuit.floor_planner().Synthesize(&assembly, circuit, std::move(config),
                                     constraint_system.constants());

  EXPECT_TRUE(assembly.fixed_columns().empty());

  std::vector<AnyColumnKey> expected_columns = {
      AdviceColumnKey(0),
      AdviceColumnKey(1),
      AdviceColumnKey(2),
      InstanceColumnKey(0),
  };
  EXPECT_EQ(assembly.permutation().columns(), expected_columns);

  const CycleStore& cycle_store = assembly.permutation().cycle_store();
  // clang-format off
  CycleStore::Table<Label> expected_mapping({
      {{3, 0}, {3, 1},  {1, 1},  {1, 2},  {1, 3},  {1, 4},  {1, 5},  {1, 6},
       {0, 8}, {0, 9}, {0, 10}, {0, 11}, {0, 12}, {0, 13}, {0, 14}, {0, 15}},
      {{0, 1}, {2, 0},  {2, 1},  {2, 2},  {2, 3},  {2, 4},  {2, 5},  {2, 6},
       {1, 8}, {1, 9}, {1, 10}, {1, 11}, {1, 12}, {1, 13}, {1, 14}, {1, 15}},
      {{0, 2}, {0, 3},  {0, 4},  {0, 5},  {0, 6},  {0, 7},  {1, 7},  {3, 2},
       {2, 8}, {2, 9}, {2, 10}, {2, 11}, {2, 12}, {2, 13}, {2, 14}, {2, 15}},
      {{0, 0}, {1, 0},  {2, 7},  {3, 3},  {3, 4},  {3, 5},  {3, 6},  {3, 7},
       {3, 8}, {3, 9}, {3, 10}, {3, 11}, {3, 12}, {3, 13}, {3, 14}, {3, 15}},
  });
  CycleStore::Table<Label> expected_aux({
      {{0, 0}, {1, 0},  {1, 1},  {1, 2},  {1, 3},  {1, 4},  {1, 5},  {1, 6},
       {0, 8}, {0, 9}, {0, 10}, {0, 11}, {0, 12}, {0, 13}, {0, 14}, {0, 15}},
      {{1, 0}, {1, 1},  {1, 2},  {1, 3},  {1, 4},  {1, 5},  {1, 6},  {1, 7},
       {1, 8}, {1, 9}, {1, 10}, {1, 11}, {1, 12}, {1, 13}, {1, 14}, {1, 15}},
      {{1, 1}, {1, 2},  {1, 3},  {1, 4},  {1, 5},  {1, 6},  {1, 7},  {2, 7},
       {2, 8}, {2, 9}, {2, 10}, {2, 11}, {2, 12}, {2, 13}, {2, 14}, {2, 15}},
      {{0, 0}, {1, 0},  {2, 7},  {3, 3},  {3, 4},  {3, 5},  {3, 6},  {3, 7},
       {3, 8}, {3, 9}, {3, 10}, {3, 11}, {3, 12}, {3, 13}, {3, 14}, {3, 15}},
  });
  CycleStore::Table<size_t> expected_sizes({
      {2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
      {3, 3, 3, 3, 3, 3, 3, 2, 1, 1, 1, 1, 1, 1, 1, 1},
      {1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1},
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

TEST_F(Fibonacci1CircuitTest, LoadVerifyingKey) {
  size_t n = 16;
  CHECK(prover_->pcs().UnsafeSetup(n, F(2)));
  prover_->set_domain(Domain::Create(n));

  Fibonacci1Circuit<F, SimpleFloorPlanner> circuit;

  VerifyingKey<PCS> vkey;
  ASSERT_TRUE(vkey.Load(prover_.get(), circuit));

  std::vector<Commitment> expected_permutation_verifying_key;
  {
    std::vector<Point> points = {
        {"0x28472c5c287afbed2c1cbad418f7736db3be191e735e933e7531f790cc0b454b",
         "0x2553db7d81ee798baa1b89b83e3cfd5516872410ac1ef7ff8e97801a32f54b22"},
        {"0x1881f5e53bde69bb20eec1af0e8b17f23f3e49cf6929f13417a5dc389f70acec",
         "0x19e1cd19519ba4353b95ba5a16a10479eec522952c97acd31199eb96b558b158"},
        {"0x1a899815b7cb019ce89ac7da39c4c0e071bd042ebffe13965de4e9e1cb968e17",
         "0x1c2bc1e987ea49a29fa8a6e2168541a29fc27b0150b2b85f8bb2d76e1abd029b"},
        {"0x02f5f4023c8be80b0fe6ff231b1c292527881597645688da1cbfe83be61bfdc9",
         "0x0eb912c0ac4f39a2e76c0fb7ac2d3977a4cc909db67680eb9e3c2436f4dc85ae"},
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
      "0x0e149c09b16d13bdc8a09508e1dab4af7399ebe708e0fc37a7fd59d43974596f");
  EXPECT_EQ(vkey.transcript_repr(), expected_transcript_repr);
}

TEST_F(Fibonacci1CircuitTest, LoadProvingKey) {
  size_t n = 16;
  CHECK(prover_->pcs().UnsafeSetup(n, F(2)));
  prover_->set_domain(Domain::Create(n));

  Fibonacci1Circuit<F, SimpleFloorPlanner> circuit;

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

TEST_F(Fibonacci1CircuitTest, CreateProof) {
  size_t n = 16;
  CHECK(prover_->pcs().UnsafeSetup(n, F(2)));
  prover_->set_domain(Domain::Create(n));

  Fibonacci1Circuit<F, SimpleFloorPlanner> circuit;
  std::vector<Fibonacci1Circuit<F, SimpleFloorPlanner>> circuits = {
      std::move(circuit)};

  F a(1);
  F b(1);
  F out(55);
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

TEST_F(Fibonacci1CircuitTest, Verify) {
  size_t n = 16;
  CHECK(prover_->pcs().UnsafeSetup(n, F(2)));
  prover_->set_domain(Domain::Create(n));

  Fibonacci1Circuit<F, SimpleFloorPlanner> circuit;

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
        {"0x093a25a0c4849e4437d1600bde8e3faacfb92a3bf15c2e2f311fa87ccc9be590",
         "0x23f368976bd1e762f05c5efc8db4e7590d2300e60ef4a7bd898cc2f5bbd771e6"},
        {"0x19fe09a37a4a987f305c5200e178b2afcad66f25081f0cfb6ee5460bae05b30b",
         "0x0e106dd8def54c87c517f1eb20a56175822ce6e65f99a3a0655fc3e2e8a3e94b"},
        {"0x116d6d4f42dc437a1871a9699400bf84ef825391345aee90bbab4e6673063431",
         "0x2c2d516d5c04579cca6c30460e27130515680a99050770aabcdd8ec5855a9373"},
    };
    expected_advice_commitments_vec.push_back(CreateCommitments(points));
  }
  EXPECT_EQ(proof.advices_commitments_vec, expected_advice_commitments_vec);

  EXPECT_TRUE(proof.challenges.empty());

  F expected_theta = F::FromHexString(
      "0x26f0af3189edb2949640abe830c64cafe9b64dabe01919447ce64af067414b17");
  EXPECT_EQ(proof.theta, expected_theta);

  ASSERT_EQ(proof.lookup_permuted_commitments_vec.size(), 1);
  EXPECT_TRUE(proof.lookup_permuted_commitments_vec[0].empty());

  F expected_beta = F::FromHexString(
      "0x1fe61de3fc90fc3a44075edf3979b5baa2d04d5bf8ab1b2abcd01e9dbcc602ef");
  EXPECT_EQ(proof.beta, expected_beta);

  F expected_gamma = F::FromHexString(
      "0x2c1de10a0bf09aec80c63b700f7c3df55a8d50869b65f110d80e7ae7b59fc822");
  EXPECT_EQ(proof.gamma, expected_gamma);

  std::vector<std::vector<Commitment>>
      expected_permutation_product_commitments_vec;
  {
    std::vector<Point> points = {
        {"0x2770bc14d269b6ca71cefba5f30e2530320f9e3cef162ff15e415f081914b9d7",
         "0x2b959c428ecbf18d311fee695e26efd283b15eed0cbb8ede6dd05e306c7e11bf"},
        {"0x26bcf178465ceb0b5a675b3169e6d45d4d3822f117677734d6576dc3e81a9aa0",
         "0x0d2301bc506d2c098bc94e735719c8d9a6123ff716b682bea4c01c1f998e8c3b"},
        {"0x1d0a1aefcac935705b9042738a29d829a05078fd3fb45a906141fac518e9505f",
         "0x27037d4c8cebdc8e6ed860d0c579327adcc6805b26a384613ec5697110741cd4"},
        {"0x1e62d936b24a496a9dc15a924409bf92fbcfc209ad29912f36d60a01e578d05e",
         "0x1f88f4bebf0300e8812d1226783eb076e7d9e5d3ba955ab46ac4491fe6c57d1b"},
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
      "0x1968fa684fa3c6f3beb4d6311b39e6bfb2938e7bd5da3c0ecb20811550d0334c");
  EXPECT_EQ(proof.y, expected_y);

  std::vector<Commitment> expected_vanishing_h_poly_commitments;
  {
    std::vector<Point> points = {
        {"0x27b33d0da75eb30e6ea8676b43176a80c60cca9e2138bc13a416516742ab9ddb",
         "0x2bbc051552caf49df89d7f0213b9940175af69a22e8e6638b85864abdaab85fd"},
        {"0x05205b476cf6e425a1a518c5c64d34f50031daf33bd3c40f02657869977bf032",
         "0x2ccf76cb7b4631490ac236c5836ca9c3b7ac5d4ce46b262e056a528d4597ad77"},
    };
    expected_vanishing_h_poly_commitments = CreateCommitments(points);
  }
  EXPECT_EQ(proof.vanishing_h_poly_commitments,
            expected_vanishing_h_poly_commitments);

  F expected_x = F::FromHexString(
      "0x1d99eef2451a8f49c1a9c4678ede5d6f542baa278555db98b1736941f5f6318d");
  EXPECT_EQ(proof.x, expected_x);

  std::vector<std::vector<F>> expected_advice_evals_vec;
  {
    std::vector<std::string_view> evals = {
        "0x0a9ee68c275f6ba33278b5eacca522598191ffea8d7c0a7fb139b86d7b8a7495",
        "0x1f3bcd2f36f42b9d340fac1d52da695b66b7902421acc46ebc8a983a840f30c0",
        "0x0d940a8479ca35f6d8cb1d66cda5e609320776bd971e314ea5504ea10bf79cf1",
    };
    expected_advice_evals_vec.push_back(CreateEvals(evals));
  }
  EXPECT_EQ(proof.advice_evals_vec, expected_advice_evals_vec);

  std::vector<F> expected_fixed_evals;
  {
    std::vector<std::string_view> evals = {
        "0x0ff12c53f8e60968d4c16220dda56f2680e821ae6cb12c26f1b4b9904b22772e",
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
        "0x0d4dd4e2bf1a3c0abade3e1f5f40113fcd7e685b5f83502249a7e42c99bf4f71",
        "0x1b5af58f20a82e13aed89cbe225dc6e0c97ba718ef74bc68097fbcc5648357dd",
        "0x018e9e2b11df39e9bce5d82f49ff0da174c2ee67e9092953adfdaf7dc7b49732",
        "0x2cb13e4b01af05eb0be992b540c5855b03bd7b1215d77e199e281dc22ecc91d1",
    };
    expected_common_permutation_evals = CreateEvals(evals);
  }
  EXPECT_EQ(proof.common_permutation_evals, expected_common_permutation_evals);

  std::vector<std::vector<F>> expected_permutation_product_evals_vec;
  {
    std::vector<std::string_view> evals = {
        "0x2d6f3fd968ee81547183978788587f22512a075d42f462663fed0e5670f8be79",
        "0x2c218f92f912f88a6db9259934f62b27b766c2f7bb19dd29d5422ba41634eb88",
        "0x16c92807f07f037eecdd664f8317880ece0406ca61df97885ffa8a66f6d57cf9",
        "0x0b5b380e2d49bbde3a612dec75112aa51bafd444ec1915094020a2258c8c5508",
    };
    expected_permutation_product_evals_vec.push_back(CreateEvals(evals));
  }
  EXPECT_EQ(proof.permutation_product_evals_vec,
            expected_permutation_product_evals_vec);

  std::vector<std::vector<F>> expected_permutation_product_next_evals_vec;
  {
    std::vector<std::string_view> evals = {
        "0x01270921adcf4e98b548c9a2df7beadbadef2c786fa5497676f0bd5aaaba5905",
        "0x11ac2dd3b1757935e3178f112743273f9108c6d7996d5e12b5d9e3a24c750ad0",
        "0x225ba1c1a670f21c711c412e88844b061c7dc200afbe63c50f1fc444846e573a",
        "0x03fcaea73ca2f3e11b8d03745301ad4add870039c14306c8f44730726e402ae2",
    };
    expected_permutation_product_next_evals_vec.push_back(CreateEvals(evals));
  }
  EXPECT_EQ(proof.permutation_product_next_evals_vec,
            expected_permutation_product_next_evals_vec);

  std::vector<std::vector<std::optional<F>>>
      expected_permutation_product_last_evals_vec;
  {
    std::vector<std::string_view> evals = {
        "0x10bdd554a772ef30402ec534e0e6571d8738e3610d195ddcc323771356329ec6",
        "0x28884d7fc1379e6a55175118536fb2da94fb3920abca155454864bea61effd18",
        "0x03588f66561ed8299b61a2a01cfb589d313f9d118e50891777c9c2e82664196f",
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
