stm-main: mono-repo for personal projects
---
Started out with intention to work on STMicro fun projects with capability to build firmware and host software on `Intel` and `ARM64` build machines. Intended target runtime platforms are `x86_64`, `aarch64` and `arm` (RPi) and hence it also supports cross-compiling the host software for the same. Scope of the repo has expanded since and now
its intentions is to support any and all projects and essentially functions as an organized scratchpad.

### Building
On `Linux` build machines, it should work OOTB. For `OSX` build machine, see this [link](https://github.com/tejasce/stm-main/wiki/Building-stm-main-on-OSX). Try `make help`.

#### Build environment in a container
The build environment for repo is version controled within the repo and is implemented using a [Dockerfile](https://github.com/tejasce/stm-main/blob/master/build/Dockerfile.buildenv). Once the build environment docker image is created, source is built in "build environment shell". This makes the build environment agnostic to OS of the host machine.
