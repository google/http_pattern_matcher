
This repository stores codes to support http template match.

http template syntax is defined [here](https://github.com/googleapis/googleapis/blob/master/google/api/http.proto#L224)

This is not an officially supported Google product

For contributors:
If you make any changes, please make sure to use Bazel to pass all unit tests by running:

```
bazel test //test/...
```
Please format your codes by running:

```
clang-format-10 -style=Google -i include/http_template_match/*.h src/*.cc test/*.cc
```


