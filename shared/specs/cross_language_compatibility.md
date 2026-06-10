# Cross-Language Behavior Compatibility

The Markdown UI file is the source of truth for every backend. Python and C++
must interpret the same source concepts consistently, even if their internal
runtime implementations differ.

## Checklist

- YAML members resolve to the same element names and element types.
- ASCII grid parsing produces the same element rectangles, named source cells,
  source cell character sizes, margins, and dimension modes.
- ASCII border characters are structural layout input only; they must not
  become element border properties.
- The star expansion marker (`*`) has the same source limit on every backend:
  at most two consecutive markers in one row and at most two vertical markers
  in one column.
- Resolved layout keeps absolute element positions derived from cell geometry
  and relative element rectangles.
- Type selectors, `@name` selectors, and nested named-cell selectors merge in
  the same order.
- Focus, edit, cursor, selected, checked, unchecked, and error style states map
  to equivalent runtime states.
- Keyboard navigation preserves navigation mode, edit mode, Tab/Shift+Tab,
  Enter, Escape, and element-specific key handling.
- Scroll containers use line-level offsets, bottom anchoring, cached child
  heights, visible-row rendering, wheel acceleration, and no dirty frame when a
  boundary wheel event cannot change the offset.
- Reusable child components keep focus ownership and tick propagation behavior
  consistent with Python.

## Automated Coverage

- `python/tests/test_cross_language_compatibility.py` compares C++ generator
  layout snapshots against Python parser snapshots for `calculator`,
  `formular`, and `cells`.
- `python/tests/test_application.py` exports Python resolved layout for the
  `cells` example and covers current ScrollView behavior.
- `cpp/tests/test_runtime_skeleton.cpp` covers C++ layout resolution,
  generated layout storage, elements, and ScrollView behavior.

## Remaining Compatibility Work

- Add C++ example targets for the real examples in Milestone 14.
- Add a C++ resolved-layout JSON export once generated example targets are
  wired into the CMake build.
- Compare chat panel ScrollView behavior through generated Python and C++
  examples after the C++ chat example exists.
