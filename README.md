# pattern-engine

NFA-based pattern engine. Uses a variation of [Thompson's construction algorithm](https://en.wikipedia.org/wiki/Thompson%27s_construction) to build NFA from a pattern string.

Supports:
- repetition operators: `*`, `+`, `?`
- grouping with parentheses: `()`
- alternatives: `|`
- characters (no character sets)

The project is for demo purposes only. Take a look at `Main.cpp` for usage.
