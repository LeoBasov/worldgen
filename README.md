# worldgen

## Test coverage

CI reports unit-test coverage on each run's summary page and uploads the full HTML report as the `coverage-report` artifact. To generate it locally (requires [gcovr](https://gcovr.com)):

```bash
cmake --workflow --preset coverage
mkdir -p coverage && gcovr --root . --filter src/ --filter cli/ --html-details coverage/index.html
```

## Profiling

The `profile` preset builds with gprof instrumentation (`-pg`, optimisations on, function inlining off so helpers stay visible). Run the CLI from an output directory, then generate the report:

```bash
cmake --workflow --preset profile
mkdir -p profile && cd profile
../build/profile/cli/cli --resolution 1024 1024 --rivers 200
gprof ../build/profile/cli/cli gmon.out > report.txt
```

`report.txt` contains a flat profile (self time per function) followed by a call graph (time including children).