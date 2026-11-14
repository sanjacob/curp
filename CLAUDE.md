# CLAUDE.md

C library (C89-style, LGPL-2.1) that validates and extracts data from a Mexican
CURP (Clave Única de Registro de Población). Code comments, identifiers, docs
and README are in **Spanish**; keep new user-facing text in Spanish.

## Layout

- `include/curp.h` — public API: `curp_init`/`curp_exit` (must wrap all use;
  they build/free the profanity trie), `curp_validar`, `curp_cargar`,
  `curp_crear`, `curp_error`, `curp_entidad_iso`/`curp_entidad_nombre`,
  `CURP_ERROR_POSICION(code)`.
- `include/trie.h`, `src/trie.c` — trie used for palabras altisonantes.
- `src/curp.c` — validation/extraction logic.
- `tests/check_curp.c` — test runner (`main`), combines two suites:
  - `tests/check_curp_examples.c` — example-based tests ([Check](https://libcheck.github.io/check/)).
  - `tests/check_curp_pbt.c` + `tests/strategies.h` — property-based tests
    ([theft](https://github.com/silentbicycle/theft)).
- `doc/` — Sphinx docs (reStructuredText, Spanish), built by Read the Docs.

Error codes from `curp_validar` are negative and encode both reason and
position; reasons are ordered by priority (earliest position in the CURP wins).
See the table in `README.md`.

## Build & test

Dependencies: `gcc`, `make`, `check` (apt), `theft` (built from source, installs
to `/usr/local`, pkg-config name `libtheft`). Meson/CMake optional.

```bash
make          # NOTE: `all` runs `clean` first; produces ./libcurp.so
make check    # builds ./test and runs all suites (examples + PBT)

cmake -B build && cmake --build build && (cd build && ctest)
meson setup build && meson compile -C build && meson test -C build
```

CI (`.github/workflows/build.yml`) runs all three builds; when adding a test
source file, add it to `Makefile`, `CMakeLists.txt` and `meson.build`.

Build outputs (`build/`, `test`, `libcurp.so`) are untracked; `libcurp.so` is
not in `.gitignore`, so don't commit it.

## Conventions

- Tabs for indentation in C sources; K&R-ish brace style as in existing code.
- Every source file starts with the LGPL copyright header — copy it for new files.
- Keep `README.md`, `doc/*.rst` and `include/curp.h` consistent when changing
  the API or error codes.

## Branches & PRs

- Every PR gets its own branch named `claude/<type>/<short-kebab-name>`, where
  `<type>` is a conventional type: `fix`, `feat`, `docs`, `chore`, `refactor`,
  `test`, `ci`, `build`. E.g. `claude/fix/cmake-meson-test-linking`.
- Branch from the latest `master`; one topic per branch.
