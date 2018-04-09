#pragma once

#include <stack>

#include "CNFA.h"

/**
 * @brief CRegex Regular expression type. Preforms regex compilation. Supports:
 *   - char recognition
 *   - `(pattern)` Grouping
 *   - `|` Alternatives
 *   - `*` Kleene star: Zero or multiple repetition
 *   - `+` One or multiple repetiton operator
 *   - `?` Optional occurance operator
 */
class CRegex
{
public:
	CRegex();

	/**
	 * @brief compile Compile regular expression string into NFA. Performs sligtly modified 
	 *        Thompson's construction.
	 *
	 * @param regex Regular expression string
	 *
	 * @return NFA which knows how to match strings
	 * @throws std::invalid_argument exception if invalid pattern
	 */
	CNFA compile(std::string regex);

private:
	/**
	* @brief compile_iter Compile regular expression string into a set of NFA. Accepts iterators, which allows
	*        effectively match substrings (we pass a substring for groups and alternatives)
	*
	* @param begin Regex group start
	* @param end Regex group end
	*
	* @return NFA which knows how to match a strings
	* @throws std::invalid_argument exception if invalid pattern
	*/
	CNFA compile_iter(std::string::iterator &begin, const std::string::iterator &end);

	/**
	 * @brief makeState Make new state with consequent IDs
	 */
	std::shared_ptr<CState> makeState();

	/**
	 * @brief concat Connect top 2 NFAs in a single NFA
	 */
	void concat(std::stack<CNFA> &nfas);

	/**
	 * @brief handleChar Handles character in a regex. Creates NFA with transition from top state to a new state
	 *
	 * @param character Character for transition
	 * @param nfas NFA stack for current group
	 */
	void handleChar(char character, std::stack<CNFA> &nfas);

	/**
	* @brief handleAlt Handles spotted alternatives. Creates a NFA with epsilon transitions for top 2 NFAs
	*
	* @param nfas NFA stack for current group
	*/
	void handleAlt(std::stack<CNFA> &nfas);

	/**
	* @brief handleRep Handles spotted repetition. Creates loop for the top NFA. In case of zero repetitions, creates epsilon transition
	*        to the final state
	*
	* @param at_least_once If at least one operator `+` or zero or more `*`
	* @param nfas NFA stack for current group
	*/
	void handleRep(bool at_least_once, std::stack<CNFA> &nfas);

	/**
	* @brief handleQmark Handles spotted question mark operator. Adds epsilon transition to the top NFA
	*
	* @param nfas NFA stack for current group
	*/
	void handleQmark(std::stack<CNFA> &nfas);

private:
	size_t _state_count;
};

