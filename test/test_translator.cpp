#include "gtest.h"
#include "translator.h"

TEST(TNumber, can_create_number)
{
	ASSERT_NO_THROW(Number num(1.5));
}

TEST(TNumber, can_get_number_value)
{
	Number num(1.5);

	EXPECT_EQ(num.getValue(), 1.5);
}

TEST(TOperator, can_create_operation)
{
	ASSERT_NO_THROW(Operator op(2.5));
}

TEST(TOperator, can_get_operation)
{
	Operator op('+');

	EXPECT_EQ(op.getOp(), '+');
}

TEST(TOpeningBracket, can_create_open_bracket)
{
	ASSERT_NO_THROW(Bracket ob('('));
}

TEST(TOpeningBracket, can_get_open_bracket)
{
	Bracket ob('(');

	EXPECT_EQ(ob.get_op(), '(');
}

TEST(TClosingBracket, can_create_close_bracket)
{
	ASSERT_NO_THROW(Bracket cb(')'));
}

TEST(TClosingBracket, can_get_close_bracket)
{
	Bracket cb(')');
	EXPECT_EQ(cb.get_op(), ')');
}

TEST(tTranslator, can_create_expression)
{
	ASSERT_NO_THROW(Translator expr("3*2-4"));
}

TEST(tTranslator, impossible_division_by_zero)
{
	Translator expr("34.4/0");

	ASSERT_ANY_THROW(expr.solve());
}

TEST(tTranslator, can_add)
{
	Translator expr1("3++4.2");

	Translator expr2("2+3.8");

	double d = 2 + 3.8;

	ASSERT_ANY_THROW(expr1.solve());

	EXPECT_EQ(5.8, expr2.solve());
}

TEST(tTranslator, can_subtract)
{
	Translator expr1("3-4.2-");

	Translator expr2("2-3.8");

	double d = 2 - 3.8;

	ASSERT_ANY_THROW(expr1.solve());

	EXPECT_EQ(d, expr2.solve());
}

TEST(tTranslator, can_multiply)
{
	Translator expr1("3*4.2**");

	Translator expr2("2*3.8");

	double d = 2 * 3.8;

	ASSERT_ANY_THROW(expr1.solve());

	EXPECT_EQ(d, expr2.solve());
}

TEST(tTranslator, can_divide)
{
	Translator expr1("34/2/3");

	Translator expr2("3.8/2");

	double d = 3.8 / 2;

	ASSERT_NO_THROW(expr1.solve());

	EXPECT_EQ(d, expr2.solve());
}

TEST(tTranslator, invalid_brackets)
{
	Translator expr1("((3.8/2)");

	Translator expr2("((3.8/2)+1))");

	Translator expr3("(3.8/2+1)(2+2))");

	ASSERT_ANY_THROW(expr1.solve());

	ASSERT_ANY_THROW(expr2.solve());

	ASSERT_ANY_THROW(expr3.solve());
}

TEST(tTranslator, combination_of_operations_without_errors)
{
	Translator expr("(3.45+4)*((45/5-2)+5)");

	ASSERT_NO_THROW(expr.solve());
}

TEST(tTranslator, combination_of_operations_and_priority_with_correct_answer)
{
	Translator expr("(3.45+4)*((45/5.0-2)+5.0)");

	double d = (3.45 + 4) * ((45 / 5.0 - 2) + 5.0);

	EXPECT_EQ(d, expr.solve());
}