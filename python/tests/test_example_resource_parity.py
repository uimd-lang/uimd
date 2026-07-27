"""Cross-language example fixture parity checks."""

from pathlib import Path


PROJECT_ROOT = Path(__file__).resolve().parents[2]
TARGETS = ("python", "cpp", "csharp", "swift", "go")
FIXTURE_DIRECTORIES = (
    Path("contacts_manager/data"),
    Path("markdown_viewer/docs"),
    Path("text_editor/notes"),
)


def test_example_auxiliary_files_match_python_reference():
    """Every target must ship the same named fixture bytes as Python."""
    for relative_directory in FIXTURE_DIRECTORIES:
        reference_directory = PROJECT_ROOT / "python/examples" / relative_directory
        reference_files = {
            path.relative_to(reference_directory): path.read_bytes()
            for path in reference_directory.rglob("*")
            if path.is_file()
        }
        assert reference_files, f"reference fixture directory is empty: {reference_directory}"

        for target in TARGETS[1:]:
            target_directory = PROJECT_ROOT / target / "examples" / relative_directory
            target_files = {
                path.relative_to(target_directory): path.read_bytes()
                for path in target_directory.rglob("*")
                if path.is_file()
            } if target_directory.is_dir() else {}
            assert target_files == reference_files, (
                f"{target}/{relative_directory} fixtures differ from the Python reference"
            )
