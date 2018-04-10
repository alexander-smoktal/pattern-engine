#include "CState.h"

CState::CState(size_t id) 
	: _epsilons()
	, _transitions()
	, _id(id)
	, _is_final_state(false) {}

void CState::deinit() {
	for (auto &eps : _epsilons) {
		if (eps->_id < _id) {
			eps.reset();
		}
	}

	for (auto &transitions : _transitions) {
		if (transitions.second->_id < _id) {
			transitions.second.reset();
		}
	}
}

void CState::addTransition(const char character, std::shared_ptr<CState> state) {
	if (!state) {
		throw std::invalid_argument("Empty epsilon transition state");
	}

	if (_transitions.count(character)) {
		throw std::invalid_argument("State already contains transition for the character");
	}

	_transitions[character] = state;
}

void CState::addEpsilonTransition(std::shared_ptr<CState> state) {
	if (!state) {
		throw std::invalid_argument("Empty epsilon transition state");
	}

	_is_final_state = false;
	_epsilons.push_back(state);
}

std::string CState::toString(std::unordered_set<size_t> &visited) {
	std::string name = "s:" + std::to_string(_id);

	if (visited.count(_id)) {
		return name;
	}

	visited.insert(_id);

	std::string result{ "((<" + name + ">: { " };

	for (const auto &trans : _transitions) {
		result += "'" + std::string(1, trans.first) + "': " + trans.second->toString(visited) + ", ";
	}

	result += " }, [ ";

	for (const auto &eps : _epsilons) {
		result += eps->toString(visited) + ", ";
	}

	result += " ], " + std::to_string(_is_final_state) + "))";
	return result;
}
