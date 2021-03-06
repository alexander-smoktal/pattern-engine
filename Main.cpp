#include <iostream>

#include "src/CRegex.h"

struct TestCase {
	const char *pattern;
	const char *string;
	bool if_matches;
};

static std::vector<TestCase> s_test_cases = {
	{ "(ab|a)(bc|c)", "abc", true },
	{ "(ab|a)(bc|c)", "acb", false },
	{ "(ab)c|abc", "abc", true },
	{ "(ab)c|abc", "ab", false },
	{ "(a*)(b?)(b+)", "aaabbbb", true },
	{ "(a*)(b?)(b+)", "aaaa", false },
	{ "((a|a)|a)", "a", true },
	{ "((a|a)|a)", "aa", false },
	{ "(a*)(a|aa)", "aaaa", true },
	{ "(a*)(a|aa)", "b", false },
	{ "a(b)|c(d)|a(e)f", "aef", true },
	{ "a(b)|c(d)|a(e)f", "adf", false },
	{ "(a|b)c|a(b|c)", "ac", true },
	{ "(a|b)c|a(b|c)", "acc", false },
	{ "(a|b)c|a(b|c)", "ab", true },
	{ "(a|b)c|a(b|c)", "acc", false },
	{ "(a|b)*c|(a|ab)*c", "abc", true },
	{ "(a|b)*c|(a|ab)*c", "bbbcabbbc", false },
	{ "a?(ab|ba)ab", "abab", true },
	{ "a?(ab|ba)ab", "aaabab", false },
	{ "(aa|aaa)*|(a|aaaaa)", "aa", true },
	{ "\na", "\na", true },
	{ "(a)(b)(c)", "abc", true },
	{ "((((((((((x))))))))))", "x", true },
	{ "((((((((((x))))))))))*", "xx", true },
	{ "a?(ab|ba)*", "ababababababababababababababababa", true },
	{ "a*a*a*a*a*b", "aaaaaaaab", true },
	{ "abc", "abc", true },
	{ "ab*c", "abc", true },
	{ "ab*bc", "abbc", true },
	{ "ab*bc", "abbbbc", true },
	{ "ab+bc", "abbc", true },
	{ "ab+bc", "abbbbc", true },
	{ "ab?bc", "abbc", true },
	{ "ab?bc", "abc", true },
	{ "ab|cd", "ab", true },
	{ "(a)b(c)", "abc", true },
	{ "a*", "aaa", true },
	{ "(a+|b)*", "ab", true },
	{ "(a+|b)+", "ab", true },
	{ "a|b|c|d|e", "e", true },
	{ "(a|b|c|d|e)f", "ef", true },
	{ "abcd*efg", "abcdefg", true },
	{ "(ab|ab*)bc", "abc", true },
	{ "(ab|a)b*c", "abc", true },
	{ "((a)(b)c)(d)", "abcd", true },
	{ "(a|ab)(c|bcd)", "abcd", true },
	{ "(a|ab)(bcd|c)", "abcd", true },
	{ "(ab|a)(c|bcd)", "abcd", true },
	{ "(ab|a)(bcd|c)", "abcd", true },
	{ "((a|ab)(c|bcd))(d*)", "abcd", true },
	{ "((a|ab)(bcd|c))(d*)", "abcd", true },
	{ "((ab|a)(c|bcd))(d*)", "abcd", true },
	{ "((ab|a)(bcd|c))(d*)", "abcd", true },
	{ "(a|ab)((c|bcd)(d*))", "abcd", true },
	{ "(a|ab)((bcd|c)(d*))", "abcd", true },
	{ "(ab|a)((c|bcd)(d*))", "abcd", true },
	{ "(ab|a)((bcd|c)(d*))", "abcd", true },
	{ "(a*)(b|abc)", "abc", true },
	{ "(a*)(abc|b)", "abc", true },
	{ "((a*)(b|abc))(c*)", "abc", true },
	{ "((a*)(abc|b))(c*)", "abc", true },
	{ "(a*)((b|abc))(c*)", "abc", true },
	{ "(a*)((abc|b)(c*))", "abc", true },
	{ "(a*)(b|abc)", "abc", true },
	{ "(a*)(abc|b)", "abc", true },
	{ "((a*)(b|abc))(c*)", "abc", true },
	{ "((a*)(abc|b))(c*)", "abc", true },
	{ "(a*)((b|abc)(c*))", "abc", true },
	{ "(a*)((abc|b)(c*))", "abc", true },
	{ "(a|ab)", "ab", true },
	{ "(ab|a)", "ab", true },
	{ "(a|ab)(b*)", "ab", true },
	{ "(ab|a)(b*)", "ab", true },
};

int main()
{
	// Some tests
	for (const auto &test_case : s_test_cases) {
		std::cout << "Test case: '" << test_case.pattern << "', '" << test_case.string << "', " << test_case.if_matches;

		CRegex regex;
		auto nfa = regex.compile(test_case.pattern);

		if (nfa.match(test_case.string) == test_case.if_matches) {
			std::cerr << " ...Passed" << std::endl;
		} 
		else {
			std::cerr << " ...Failed" << std::endl;

			exit(EXIT_FAILURE);
		}
	}

	std::string pattern = "a*bcc";
	std::string source = "mmasn bccsas aaaabccd abcwf abccjh";

	CRegex parser;

	{
		auto nfa = parser.compile(pattern);

		std::cout << "Unique occurances: " << nfa.countGroups(source) << std::endl;
		std::cout << "Occurances: " << nfa.count(source) << std::endl;
	}

	exit(EXIT_SUCCESS);
}
