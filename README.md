

This repo shows a simple case to use the CMake to build the Margo example.
There are three typical networking patterns to run the server program. 

```
zw241@slepner087:~/cworkspace/build/build_margoexp$ ./margoserver verbs://
# accepting RPCs on address "ofi+verbs;ofi_rxm://192.168.40.87:44589"
...
zw241@slepner087:~/cworkspace/build/build_margoexp$ ./margoserver tcp://
# accepting RPCs on address "ofi+tcp;ofi_rxm://192.168.40.87:41504"
...
zw241@slepner087:~/cworkspace/build/build_margoexp$ ./margoserver na+sm://
# accepting RPCs on address "na+sm://7985/0"
```

### How to configure the Spack env for Mochi project

Before compiling the project by CMake, it is necessary to install the Spack and configure the Mochi project according to the following document:

https://xgitlab.cels.anl.gov/sds/mochi-boot-camp/blob/master/anl-2019/sessions/s2-hands-on/README.md

### The introduction for the Margo project

This pdf provides several typical examples to use the Margo RPC:

https://xgitlab.cels.anl.gov/sds/mochi-boot-camp/blob/master/anl-2019/sessions/s1-margo-thallium/mbc-s1-margo-and-thallium.pdf

This repo contains all the necessary introduction about the Mochi projects:

https://xgitlab.cels.anl.gov/sds/mochi-boot-camp/tree/master/anl-2019

This is the repo of the margo project:

https://xgitlab.cels.anl.gov/sds/margo

