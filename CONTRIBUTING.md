# Contributing

This repository currently uses SVN for local development. Do not use Git
commands inside the working copy unless the repository is deliberately exported
or mirrored for GitHub publication.

Before proposing changes:

```bash
python3 -m pytest python/tests
cmake -S cpp -B cpp/build
cmake --build cpp/build
ctest --test-dir cpp/build --output-on-failure
```

For runtime, compiler, generator, or shared behavior changes, regenerate all
UI sources and rebuild all examples for every supported backend.
