# Migration Audit

Date: 2026-05-11

## Baseline

- Initial `svn status`: clean.
- Baseline verification: `python3 -m pytest` passed with 269 tests.

## Root-Level Python References

- Runtime imports are concentrated in Python runtime modules, tests, generated UI files, and handwritten examples.
- Legacy compiler imports are concentrated in `python/compile.py`, `python/compiler/code_generator.py`, and compiler tests.
- Example path assumptions are used by application tests and handwritten example launchers.
- Theme lookup was root-layout dependent and was updated to prefer `shared/themes`.

## Generated Files

- Generated Python files live next to their `.md` source files under `python/examples`.
- Regenerating `cells_ui.py` after migration produced only style dictionary ordering noise, so that generated diff was reverted.

## Migration Choices

- Keep root `compile.py` as a legacy compatibility wrapper; new workflows use `./uimd`.
- Keep existing Python import names (`runtime`, `compiler`) by running Python tools with `python/` on `sys.path`.
- Move shared themes to `shared/themes`.
- Do not create shared root-level examples; examples remain language-specific.
