#pragma once

#include "CState.h"

/**
 * @brief CNFA Non finite automata class for regular expressions
 */
class CNFA
{
public:
	/**
	 * @brief CNFA Constructor. Creates NFA
	 *
	 * @param start_state NFA start state
	 * @param end_state NFA end state
	 */
	CNFA(const std::shared_ptr<CState> &start_state,
		 const std::shared_ptr<CState> &end_state);

	/**
	 * @brief match Check if source string matches the pattern coded in NFA
	 *
	 * @param source String to match
	 */
	bool match(const std::string &source) const;

	/**
	* @brief count Counts unique pattern matches in a source string (`unique` means they don't overlap)
	*
	* @param source String to match
	*/
	int countGroups(const std::string &source) const;

	/**
	* @brief count Counts pattern matches in a source string. Returns total amount of matches even if they overlap.
	*
	* @param source String to match
	*/
	int count(const std::string &source) const;

	/**
	 * @brief toString Stringify the NFA. For debug purposes
	 *
	 * @return Stringified state
	 */
	std::string toString() const;

	/**
	 * @brief startState Start state accessor
	 *
	 * @return Start state
	 */
	std::shared_ptr<CState> startState() {
		return _start_state;
	}

	/**
	* @brief endState Start state accessor
	*
	* @return End state
	*/
	std::shared_ptr<CState> endState() {
		return _end_state;
	}
private:
	/**
	 * @brief addState Utility function. Add the state to the state set
	 *
	 * @param state State to add
	 * @param state_set Set to add to
	 */
	static void addState(const std::shared_ptr<CState> &state,
		                 std::unordered_set<std::shared_ptr<CState>> &state_set);

	/**
	* @brief addMultistate Utility function. Add the state to the state vector. Doesn't check if state is already added. This allows
	*        to count intersecting matches.
	*
	* @param state State to add
	* @param state_vector Vector to add to
	*/
	static void addMultistate(const std::shared_ptr<CState> &state,
						      std::vector<std::shared_ptr<CState>> &state_vector);
private:
	std::shared_ptr<CState> _start_state;
	std::shared_ptr<CState> _end_state;
};

