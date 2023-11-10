# libpowenetics
[![Build Status - Azure Pipelines][build-button]][build-link]

[build-button]: https://img.shields.io/github/checks-status/UniStuttgart-VISUS/libpowenetics/master?label=Azure%20Pipelines&logo=Azure%20Pipelines
[build-link]: https://devops.visus.uni-stuttgart.de/tfs/VIS(US)/External%20Pipelines/_build/latest?definitionId=46&branchName=master

A native C/C++ library for the [Powenetics v2 power measurement kit](https://www.cybenetics.com/index.php?option=powenetics). The library is a clean-sheet design, which is, however, based on the implementation on the source code of the original Powenetics v2 tool, which Cybenetics generously provided to us. In contrast to their tool, this library is not intended for end users, but for application developers who want to include power measurements in their native applications and sychronise these measurements with their code.
