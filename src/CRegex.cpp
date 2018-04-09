#include "CRegex.h"

CRegex::CRegex()
	: _state_count(0) {}

CNFA CRegex::compile(std::string regex) {
	if (!regex.size()) {
		throw std::invalid_argument("Empty regex");
	}

	auto begin = regex.begin();
	return compileIter(begin, regex.end());
}

CNFA CRegex::compileIter(std::string::iterator &begin, const std::string::iterator &end) {
	std::stack<CNFA> parse_stack;

	for (; begin != end && *begin != ')'; ++begin) {

		if (*begin == '(') {
			auto group_nfa = compileIter(++begin, end); // Parse group

			if (begin == end || *begin != ')') {
				throw std::invalid_argument("Unclosed parentheses");
			}

			parse_stack.push(group_nfa);
		}
		else if (*begin == '|') {
			auto alt_nfa = compileIter(++begin, end); // Parse right alternative group

			parse_stack.push(alt_nfa);

			handleAlt(parse_stack);
			--begin; // We decrease iterator, because we've already skipped '|'
		}
		else if (*begin == '*') {
			handleRep(false, parse_stack);
		}
		else if (*begin == '+') {
			handleRep(true, parse_stack);
		}
		else if (*begin == '?') {
			handleQmark(parse_stack);
		}
		else {
			handleChar(*begin, parse_stack);
		}

		// In case we haven't finished and next pattern is a special operator, we don't 
		// concatenate patterns on top so operator could wrap it.
		if (parse_stack.size() > 1) {
			auto next = begin + 1;

			if (next == end 
				|| *next != '*' && *next != '+' && *next != '?') {
				concat(parse_stack);
			}
		}
	}

	if (parse_stack.size() != 1) {
		throw std::invalid_argument("Invalid regex. Unhandled group or alternative");
	}

	return parse_stack.top();
}

std::shared_ptr<CState> CRegex::makeState() {
	return std::make_shared<CState>(std::string("S: ") + std::to_string(++_state_count));
}

void CRegex::concat(std::stack<CNFA> &nfas) {
	CNFA next = nfas.top();
	nfas.pop();
	CNFA prev = nfas.top();
	nfas.pop();

	prev.finalState()->addEpsilonTransition(next.startState());

	nfas.push(CNFA(prev.startState(), next.finalState()));
}

void CRegex::handleChar(char character, std::stack<CNFA> &nfas) {
	auto start_state = makeState();
	auto end_state = makeState();

	start_state->addTransition(character, end_state);

	nfas.push(CNFA(start_state, end_state));
}

void CRegex::handleAlt(std::stack<CNFA> &nfas) {
	auto start_state = makeState();
	auto final_state = makeState();

	CNFA alt1 = nfas.top();
	nfas.pop();
	alt1.finalState()->addEpsilonTransition(final_state);

	CNFA alt2 = nfas.top();
	nfas.pop();
	alt2.finalState()->addEpsilonTransition(final_state);

	start_state->addEpsilonTransition(alt1.startState());
	start_state->addEpsilonTransition(alt2.startState());
	nfas.push(CNFA(start_state, final_state));
}

void CRegex::handleRep(bool at_least_once, std::stack<CNFA> &nfas) {
	auto underlying_patt = nfas.top();
	nfas.pop();

	auto s0 = makeState();
	s0->addEpsilonTransition(underlying_patt.startState());

	auto s1 = makeState();

	if (!at_least_once) {
		s0->addEpsilonTransition(s1);
	}

	underlying_patt.finalState()->addEpsilonTransition(s1);
	underlying_patt.finalState()->addEpsilonTransition(underlying_patt.startState());

	nfas.push(CNFA(s0, s1));
}

void CRegex::handleQmark(std::stack<CNFA> &nfas) {
	auto underlying_patt = nfas.top();

	underlying_patt.startState()->addEpsilonTransition(underlying_patt.finalState());
}
