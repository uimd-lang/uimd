# UIMD 0.5.3 retained compatibility fixture

The generated sources in this directory are frozen outputs of the UIMD 0.5.3
native generator. Validation must compile and run them against the current
runtime without editing or regenerating them.

The application retains the public generated surfaces used by the related
TextInput alignment, ComboBox overlay hit-geometry, and reusable generated
ScrollView focus fixes. The gate exercises the first two interactions and
focuses/renders the retained ScrollView. Exact alpha-compositing behavior is
covered by each runtime's focused regression because an old generated source
fixture is a compatibility check, not a replacement for current behavioral
tests. The canonical input files are retained only to document provenance;
the compatibility test never invokes `uimd generate`.

The small application wrappers and build files are part of the frozen fixture.
`manifest.sha256` protects all retained inputs and outputs before and after the
test. Build products are written outside this directory.

Run the complete seven-port gate with:

```bash
python3 tools/previous_version_compatibility.py
```
