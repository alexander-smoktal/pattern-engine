#include "CNFA.h"

CNFA::CNFA(const std::shared_ptr<CState> &start_state, const std::shared_ptr<CState> &end_state)
	: _start_state(start_state)
	, _final_state(end_state) {
	_final_state->setIsFinalState(true);
}

CNFA::~CNFA() {
	// NFA start state is the only entry point for the NFA graph. If we are going to 
	// destroy entry state, we resolve graph cyclic dependency first.
	if (_start_state.use_count() == 1) {
		_start_state->deinit();
	}
}

void CNFA::addState(const std::shared_ptr<CState> &state, std::unordered_set<std::shared_ptr<CState>> &state_set) {
	if (state_set.count(state)) {
		return;
	}

	state_set.insert(state);

	for (const auto &eps : state->epsilonTransitions()) {
		addState(eps, state_set);
	}
}

void CNFA::addMultistate(const std::shared_ptr<CState> &state, std::vector<std::shared_ptr<CState>> &state_vector) {
	state_vector.push_back(state);

	for (const auto &eps : state->epsilonTransitions()) {
		addMultistate(eps, state_vector);
	}
}

bool CNFA::match(const std::string &source) const {
	if (!source.size()) {
		throw std::invalid_argument("Empty string");
	}

	// current_states contain intermediate states across all string parsing
	std::unordered_set<std::shared_ptr<CState>> current_states;
	addState(_start_state, current_states);

	for (const auto &character : source) {
		std::unordered_set<std::shared_ptr<CState>> next_states;

		// For each state check if it accepts the character. If so, move transition for the character into the intermediate states
		for (const auto &state : current_states) {
			if (state->transitions().count(character)) {
				auto transition_state = state->transitions().at(character);

				addState(transition_state, next_states);
			}
		}

		current_states = next_states;
	}

	// Check if any of current states is a final state. If so, string fully matches the pattern
	for (const auto &state : current_states) {
		if (state->isFinalState()) {
			return true;
		}
	}

	return false;
}

int CNFA::countGroups(const std::string &source) const {
	if (!source.size()) {
		throw std::invalid_argument("Empty string");
	}

	int result{ 0 };

	// current_states contain intermediate states across all string parsing
	std::unordered_set<std::shared_ptr<CState>> current_states;

	for (const auto &character : source) {
		// The function itself is very similar to the match function. The differences are that we add own start state
		// for each characetter to see if we may start matching here. Also we check for finite states during iteration
		addState(_start_state, current_states);
		std::unordered_set<std::shared_ptr<CState>> next_states;

		// For each state check if it accepts the character. If so, move transition for the character into the intermediate states
		for (const auto &state : current_states) {
			if (state->transitions().count(character)) {
				auto transition_state = state->transitions().at(character);

				// Here we check if we get final state, which means we got a match.
				// Here we reset states to initial to start new group matching
				if (transition_state->isFinalState()) {
					++result;

					next_states.clear();
					next_states.insert(_start_state);

					break;
				}
				else {
					addState(transition_state, next_states);
				}
			}
		}

		current_states = next_states;
	}

	return result;
}

int CNFA::count(const std::string &source) const {
	if (!source.size()) {
		throw std::invalid_argument("Empty string");
	}

	int result{ 0 };

	// current_states contain intermediate states across all string parsing
	std::vector<std::shared_ptr<CState>> current_states;

	for (const auto &character : source) {
		// The function itself is very similar to the match function. The differences are that we add own start state
		// for each characetter to see if we may start matching here. Also we check for finite states during iteration.
		// Note: we use multistates here to search for overlapping matches.
		addMultistate(_start_state, current_states);
		std::vector<std::shared_ptr<CState>> next_states;

		// For each state check if it accepts the character. If so, move transition for the character into the intermediate states
		for (const auto &state : current_states) {
			if (state->transitions().count(character)) {
				auto transition_state = state->transitions().at(character);

				addMultistate(transition_state, next_states);

				// Here we check if we get final state, which means we got a match
				if (transition_state->isFinalState()) {
					++result;
				}
			}
		}

		current_states = next_states;
	}

	return result;
}

std::string CNFA::toString() const {
	std::unordered_set<size_t> set;
	return _start_state->toString(set);
}