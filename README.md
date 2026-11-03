# worldgen

## Test coverage

CI reports unit-test coverage on each run's summary page and uploads the full HTML report as the `coverage-report` artifact. To generate it locally (requires [gcovr](https://gcovr.com)):

```bash
cmake --workflow --preset coverage
mkdir -p coverage && gcovr --root . --filter src/ --filter cli/ --html-details coverage/index.html
```