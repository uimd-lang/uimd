"""Shared image data for the image browser example."""

import os

INITIAL_LIST_COUNT = 20
IMAGE_EXTENSION_FILTER = "(png|jpg|jpeg|gif|bmp|tga|ppm|pgm)"
IMAGE_SAMPLE_DIR = "shared/assets/image_samples"


def sample_extension(index):
    remainder = int(index) % 6
    if remainder == 0:
        return ".bmp"
    if remainder == 1:
        return ".gif"
    if remainder == 2:
        return ".png"
    if remainder == 3:
        return ".tga"
    return ".jpg"


IMAGE_LIBRARY = (
    ("Camera", "shared/assets/image_samples/camera.png"),
    ("Astronaut", "shared/assets/image_samples/astronaut.png"),
    ("Coffee", "shared/assets/image_samples/coffee.png"),
    ("Coins", "shared/assets/image_samples/coins.png"),
    ("Chelsea", "shared/assets/image_samples/chelsea.png"),
    *(
        (f"Sample {index}", f"shared/assets/image_samples/sample_{index}{sample_extension(index)}")
        for index in range(6, 51)
    ),
)


def image_name_from_path(path):
    stem = os.path.splitext(os.path.basename(str(path).rstrip("/")))[0]
    return _title_case_image_name(stem or str(path))


def image_display_path(path, project_dir=None):
    text = str(path or "")
    if not text:
        return ""
    if project_dir:
        try:
            absolute = os.path.abspath(text)
            root = os.path.abspath(project_dir)
            if absolute == root or absolute.startswith(root + os.sep):
                return os.path.relpath(absolute, root)
        except (OSError, ValueError):
            pass
    return text


def make_image_record(name, source):
    return {
        "name": name,
        "source": source,
        "selected": False,
    }


def _title_case_image_name(name):
    text = str(name or "").replace("_", " ").replace("-", " ").strip()
    if not text:
        return ""
    return " ".join(part[:1].upper() + part[1:] for part in text.split())


def make_initial_image_records():
    return [
        make_image_record(name, source)
        for name, source in IMAGE_LIBRARY[:INITIAL_LIST_COUNT]
    ]
