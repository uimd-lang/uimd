# Getting Started With Python

Create and run a minimal UIMD app:

```bash
mkdir hello-ui
cd hello-ui
python3 -m venv .venv
source .venv/bin/activate
python3 -m pip install uimd

uimd new hello
uimd run hello.uimd
```

Generated files:

```text
hello.uimd      UIMD source file
hello.py        handwritten application logic
hello_ui.py     generated UI code
```

Normal edit cycle:

```bash
uimd generate hello.uimd
python3 hello.py
```

Edit `hello.uimd` for layout, members, and styling. Edit `hello.py` for
behavior and application data.
