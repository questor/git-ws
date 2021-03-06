// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_TESTS_TESTSPREPROCESSOR
#define SSVU_TESTS_TESTSPREPROCESSOR

SSVUT_TEST(PreprocessorTests)
{
	SSVUT_EXPECT(SSVPP_ARGCOUNT() == 0);
	SSVUT_EXPECT(SSVPP_ARGCOUNT(1) == 1);
	SSVUT_EXPECT(SSVPP_ARGCOUNT(1, 1) == 2);
	SSVUT_EXPECT(SSVPP_ARGCOUNT(1, 1, 1) == 3);
	SSVUT_EXPECT(SSVPP_ARGCOUNT(1, 1, 1, 1) == 4);
	SSVUT_EXPECT(SSVPP_ARGCOUNT(1, 1, 1, 1, 1) == 5);
	SSVUT_EXPECT(SSVPP_ARGCOUNT(1, 1, 1, 1, 1, 1) == 6);

	SSVUT_EXPECT(std::string{SSVPP_TOSTR(SSVPP_CAT())} == "");
	SSVUT_EXPECT(SSVPP_CAT(15) == 15);
	SSVUT_EXPECT(SSVPP_CAT(15, 15) == 1515);
	SSVUT_EXPECT(SSVPP_CAT(15, 15, 15) == 151515);

	SSVUT_EXPECT(SSVPP_INCREMENT(1) == 2);
	SSVUT_EXPECT(SSVPP_INCREMENT(100) == 101);
	SSVUT_EXPECT(SSVPP_DECREMENT(1) == 0);
	SSVUT_EXPECT(SSVPP_DECREMENT(100) == 99);

	SSVUT_EXPECT(SSVPP_BOOL(0) == 0);
	SSVUT_EXPECT(SSVPP_BOOL(1) == 1);
	SSVUT_EXPECT(SSVPP_BOOL(2) == 1);
	SSVUT_EXPECT(SSVPP_BOOL(3) == 1);

	SSVUT_EXPECT(SSVPP_AND(0, 0) == 0);
	SSVUT_EXPECT(SSVPP_AND(0, 53) == 0);
	SSVUT_EXPECT(SSVPP_AND(22, 0) == 0);
	SSVUT_EXPECT(SSVPP_AND(35, 11) == 1);

	SSVUT_EXPECT(SSVPP_OR(0, 0) == 0);
	SSVUT_EXPECT(SSVPP_OR(0, 53) == 1);
	SSVUT_EXPECT(SSVPP_OR(22, 0) == 1);
	SSVUT_EXPECT(SSVPP_OR(35, 11) == 1);

	SSVUT_EXPECT(SSVPP_NOR(0, 0) == 1);
	SSVUT_EXPECT(SSVPP_NOR(0, 53) == 0);
	SSVUT_EXPECT(SSVPP_NOR(22, 0) == 0);
	SSVUT_EXPECT(SSVPP_NOR(35, 11) == 0);

	SSVUT_EXPECT(SSVPP_XOR(0, 0) == 0);
	SSVUT_EXPECT(SSVPP_XOR(0, 53) == 1);
	SSVUT_EXPECT(SSVPP_XOR(22, 0) == 1);
	SSVUT_EXPECT(SSVPP_XOR(35, 11) == 0);

	SSVUT_EXPECT(SSVPP_NOT(35) == 0);
	SSVUT_EXPECT(SSVPP_NOT(0) == 1);

	SSVUT_EXPECT(SSVPP_IF(SSVPP_XOR(35, 11), 10, 20) == 20);
	SSVUT_EXPECT(SSVPP_IF(1, 10, 20) == 10);

	{
		int k{0};

		#define SSVU_TEST_FOREFFECT(mIdx, mData, mArg)	k += mIdx; k += mArg;
		//					v action(idx, data, arg)	v data				v vargs
		SSVPP_FOREACH(SSVU_TEST_FOREFFECT,		SSVPP_EMPTY(),	1, 2, 3, 4)
		#undef SSVU_TEST_FOREFFECT

		SSVUT_EXPECT(k == 16);
	}

	{
		std::string s(SSVPP_TOSTR_SEP(", ", 1, 2, 3));
		SSVUT_EXPECT(s == "1, 2, 3");
	}

	{
		#define SSVU_TEST_ADDTEN(mX)					SSVPP_CAT(1, mX)
		#define SSVU_TEST_FOREFFECT(mIdx, mData, mArg)	SSVU_TEST_ADDTEN(mArg)SSVPP_COMMA_IF(mIdx)

		std::string s(SSVPP_TOSTR_SEP(", ", SSVPP_FOREACH(SSVU_TEST_FOREFFECT, SSVPP_EMPTY(), 1, 2, 3, 4)));
		SSVUT_EXPECT(s == "11, 12, 13, 14");

		#undef SSVU_TEST_ADDTEN
		#undef SSVU_TEST_FOREFFECT
	}

	{
		SSVUT_EXPECT(SSVPP_TPL_ELEM((1, 2, 3), 0) == 1);
		SSVUT_EXPECT(SSVPP_TPL_ELEM((1, 2, 3), 1) == 2);
		SSVUT_EXPECT(SSVPP_TPL_ELEM((1, 2, 3), 2) == 3);

		SSVUT_EXPECT(SSVPP_TPL_ELEM(SSVPP_TPL_MAKE(1, 2, 3), 0) == 1);
		SSVUT_EXPECT(SSVPP_TPL_ELEM(SSVPP_TPL_MAKE(1, 2, 3), 1) == 2);
		SSVUT_EXPECT(SSVPP_TPL_ELEM(SSVPP_TPL_MAKE(1, 2, 3), 2) == 3);

		SSVUT_EXPECT(SSVPP_TPL_ELEM(SSVPP_TPL_EXPLODE(((1, 2))), 0) == 1);
		SSVUT_EXPECT(std::string{SSVPP_TOSTR_SEP(", ", SSVPP_TPL_EXPLODE((1, 2)))} == "1, 2");

		#define SSVU_TEST_CONCATENATED_TPLS SSVPP_TPL_CAT((1, 2), (3, 4))
		SSVUT_EXPECT(SSVPP_TPL_ELEM(SSVU_TEST_CONCATENATED_TPLS, 0) == 1);
		SSVUT_EXPECT(SSVPP_TPL_ELEM(SSVU_TEST_CONCATENATED_TPLS, 1) == 2);
		SSVUT_EXPECT(SSVPP_TPL_ELEM(SSVU_TEST_CONCATENATED_TPLS, 2) == 3);
		SSVUT_EXPECT(SSVPP_TPL_ELEM(SSVU_TEST_CONCATENATED_TPLS, 3) == 4);
		#undef SSVU_TEST_CONCATENATED_TPLS

		SSVUT_EXPECT(SSVPP_TPL_ELEM(SSVPP_TPL_CAT((1), (3)), 1) == 3);

		#define SSVU_TEST_CONCATENATED_TPLS SSVPP_TPL_CAT((1), (1, 2), (3, 4), (4, 5, 6), (1))

		SSVUT_EXPECT(SSVPP_TPL_ELEM(SSVU_TEST_CONCATENATED_TPLS, 0) == 1);
		SSVUT_EXPECT(SSVPP_TPL_ELEM(SSVU_TEST_CONCATENATED_TPLS, 1) == 1);
		SSVUT_EXPECT(SSVPP_TPL_ELEM(SSVU_TEST_CONCATENATED_TPLS, 2) == 2);
		SSVUT_EXPECT(SSVPP_TPL_ELEM(SSVU_TEST_CONCATENATED_TPLS, 3) == 3);
		SSVUT_EXPECT(SSVPP_TPL_ELEM(SSVU_TEST_CONCATENATED_TPLS, 4) == 4);
		SSVUT_EXPECT(SSVPP_TPL_ELEM(SSVU_TEST_CONCATENATED_TPLS, 5) == 4);
		SSVUT_EXPECT(SSVPP_TPL_ELEM(SSVU_TEST_CONCATENATED_TPLS, 6) == 5);
		SSVUT_EXPECT(SSVPP_TPL_ELEM(SSVU_TEST_CONCATENATED_TPLS, 7) == 6);
		SSVUT_EXPECT(SSVPP_TPL_ELEM(SSVU_TEST_CONCATENATED_TPLS, 8) == 1);

		SSVUT_EXPECT(SSVPP_TPL_ELEM_REVERSE(SSVU_TEST_CONCATENATED_TPLS, 0) == 1);
		SSVUT_EXPECT(SSVPP_TPL_ELEM_REVERSE(SSVU_TEST_CONCATENATED_TPLS, 1) == 6);
		SSVUT_EXPECT(SSVPP_TPL_ELEM_REVERSE(SSVU_TEST_CONCATENATED_TPLS, 2) == 5);
		SSVUT_EXPECT(SSVPP_TPL_ELEM_REVERSE(SSVU_TEST_CONCATENATED_TPLS, 3) == 4);
		SSVUT_EXPECT(SSVPP_TPL_ELEM_REVERSE(SSVU_TEST_CONCATENATED_TPLS, 4) == 4);
		SSVUT_EXPECT(SSVPP_TPL_ELEM_REVERSE(SSVU_TEST_CONCATENATED_TPLS, 5) == 3);
		SSVUT_EXPECT(SSVPP_TPL_ELEM_REVERSE(SSVU_TEST_CONCATENATED_TPLS, 6) == 2);
		SSVUT_EXPECT(SSVPP_TPL_ELEM_REVERSE(SSVU_TEST_CONCATENATED_TPLS, 7) == 1);
		SSVUT_EXPECT(SSVPP_TPL_ELEM_REVERSE(SSVU_TEST_CONCATENATED_TPLS, 8) == 1);

		#undef SSVU_TEST_CONCATENATED_TPLS

		SSVUT_EXPECT(SSVPP_TPL_SIZE((1, 2, 3)) == 3);
		SSVUT_EXPECT(SSVPP_TPL_SIZE((2)) == 1);
		SSVUT_EXPECT(SSVPP_TPL_SIZE((1, 2, 3, 1, 1)) == 5);
		SSVUT_EXPECT(SSVPP_TPL_SIZE(()) == 0);

		SSVUT_EXPECT(SSVPP_TPL_HEAD((1, 2, 3)) == 1);
		SSVUT_EXPECT(SSVPP_TPL_TAIL((1, 2, 3)) == 3);

		SSVUT_EXPECT(SSVPP_TPL_HEAD((1, 2)) == 1);
		SSVUT_EXPECT(SSVPP_TPL_TAIL((1, 2)) == 2);

		SSVUT_EXPECT(SSVPP_TPL_HEAD((1)) == 1);
		SSVUT_EXPECT(SSVPP_TPL_TAIL((1)) == 1);

		SSVUT_EXPECT(SSVPP_TPL_ELEM(SSVPP_TPL_PUSH_FRONT((1, 2), 3), 0) == 3);
		SSVUT_EXPECT(SSVPP_TPL_ELEM(SSVPP_TPL_PUSH_FRONT((1, 2), 3), 2) == 2);

		SSVUT_EXPECT(SSVPP_TPL_ELEM(SSVPP_TPL_PUSH_BACK((1, 2), 3), 0) == 1);
		SSVUT_EXPECT(SSVPP_TPL_ELEM(SSVPP_TPL_PUSH_BACK((1, 2), 3), 2) == 3);
	}

	{
		SSVUT_EXPECT(SSVPP_TPL_SIZE(SSVPP_TPL_FILL(())) == SSVPP_TPL_MAX_SIZE);
		SSVUT_EXPECT(SSVPP_TPL_SIZE(SSVPP_TPL_FILL((1))) == SSVPP_TPL_MAX_SIZE);
		SSVUT_EXPECT(SSVPP_TPL_SIZE(SSVPP_TPL_FILL((1, 1))) == SSVPP_TPL_MAX_SIZE);
		SSVUT_EXPECT(SSVPP_TPL_SIZE(SSVPP_TPL_FILL((1, 1, 1))) == SSVPP_TPL_MAX_SIZE);
	}

	{
		#define SSVU_TEST_GEN_LMBD(mReturn, mName, mBody) auto mName = []() -> mReturn { mBody };

		SSVU_TEST_GEN_LMBD(__R(std::pair<int, int>), __R(lambda1), __R(return std::make_pair(1, 5);));
		SSVU_TEST_GEN_LMBD(__R(std::pair<int, std::pair<float, float>>), __R(lambda2), __R(return std::make_pair(2, std::pair<float, float>(1.5f, 2.5f));));

		SSVUT_EXPECT(lambda1().first == 1);
		SSVUT_EXPECT(lambda1().second == 5);

		SSVUT_EXPECT(lambda2().first == 2);
		SSVUT_EXPECT(lambda2().second.first == 1.5f);
		SSVUT_EXPECT(lambda2().second.second == 2.5f);

		#undef SSVU_TEST_GEN_LMBD
	}
}

#endif
