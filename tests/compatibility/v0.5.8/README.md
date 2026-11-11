# UIMD 0.5.8 retained compatibility fixture

The generated sources in this directory are frozen outputs of the UIMD 0.5.8
native generator built from commit `7d0dab2aedb48a8df6c78d33eb0185aadeeee897`.
`provenance.json` records the generator tree, local binary hash, version output,
and exact generation commands. The baseline was extracted into an isolated
source checkout with `git archive`; current runtime changes were not present
while producing these sources. Validation must compile and run them against the current
runtime without editing or regenerating them.

The application retains the public generated surfaces used by the related
TextInput alignment, ComboBox overlay hit-geometry, and reusable generated
ScrollView focus fixes. The gate exercises the first two interactions and
focuses/renders the retained ScrollView. Exact alpha-compositing behavior is
covered by each runtime's focused regression because an old generated source
fixture is a compatibility check, not a replacement for current behavioral
tests. The canonical input files are retained only to document provenance;
the compatibility test never invokes `uimd generate`.

The application/build wrappers retain the documented 0.5.3 API construction
patterns, with their version references updated to 0.5.8 before freezing.
Both the original 0.5.8 runtime and the updated runtime must build and exercise
the same files. The small wrappers and build files are part of the frozen fixture.
`manifest.sha256` protects all retained inputs and outputs before and after the
test. Build products are written outside this directory.

Run the complete seven-port gate with:

```bash
python3 tools/previous_version_compatibility.py
```
