This repo shows some simple cases to use the CMake to build the Margo example.

### the example-helloworld

This example send a simple string to the server and there is no response from the server

the server:
```
zw241@amarel2:~/cworkspace/src/margoexp/example-helloworld/build$ ./margoserver tcp://
Server running at address ofi+tcp;ofi_rxm://192.168.34.22:39859
Hello World!
Hello World!
Hello World!
Hello World!
Hello World!
```

the client:

```
~/cworkspace/src/margoexp/example-helloworld/build$ ./margoclient tcp://192.168.34.22:39859
```

### the example-datastructure

this example show how to send and get the customized data structure

server:

```
zw241@amarel2:~/cworkspace/src/margoexp/example-datastructure/build$ ./margoclient na+sm://31462/0
Got response: 42+43 = 85
Got response: 44+45 = 89
Got response: 46+47 = 93
Got response: 48+49 = 97
```

client:

```
zw241@amarel2:~/cworkspace/src/margoexp/example-datastructure/build$ ./margoserver 
Server running at address na+sm://31462/0
Computed 42 + 43 = 85
Computed 44 + 45 = 89
Computed 46 + 47 = 93
Computed 48 + 49 = 97
```

### the example-bulk

this example shows how to transfer the bulk data between client and server

server:
```
zw241@slepner022:~/cworkspace/src/margoexp/example-bulk/build$ ./margoserver verbs://
# accepting RPCs on address "ofi+verbs;ofi_rxm://192.168.40.22:54065"
Got RPC request with input_val: 0
Got RPC request with input_val: 1
Got RPC request with input_val: 2
Got RPC request with input_val: 3
Got RPC request to shutdown
# Margo diagnostics
# Wed Jan 15 00:39:33 2020

# RPC breadcrumbs for RPCs that were registered on this process:
# 0x9245	my_shutdown_rpc
# 0xe282	my_rpc
# <stat>	<avg>	<min>	<max>	<count>
# Time consumed by HG_Trigger()
trigger_elapsed	0.000002469	0.000000000	0.000404119	525
# Time consumed by HG_Progress() when called with timeout==0
progress_elapsed_zero_timeout	0.000002742	0.000000477	0.000007868	8
# Time consumed by HG_Progress() when called with timeout!=0
progress_elapsed_nonzero_timeout	0.099763229	0.000207424	0.100379705	506
# Timeout values passed to HG_Progress()
progress_timeout_value	98.443579767	0.000000000	100.000000000	514
```

client:

```
zw241@amarel2:~/cworkspace/src/margoexp/example-bulk/build$ srun ./margoclient verbs://192.168.40.22:54065
srun: job 83487186 queued and waiting for resources
srun: job 83487186 has been allocated resources
ULT [0] running.
ULT [1] running.
ULT [2] running.
ULT [3] running.
Got response ret: 0
ULT [0] done.
Got response ret: 0
ULT [1] done.
Got response ret: 0
ULT [2] done.
Got response ret: 0
ULT [3] done.
# Margo diagnostics
# Wed Jan 15 00:39:33 2020

# RPC breadcrumbs for RPCs that were registered on this process:
# 0x9245	my_shutdown_rpc
# 0xe282	my_rpc
# <stat>	<avg>	<min>	<max>	<count>
# Time consumed by HG_Trigger()
trigger_elapsed	0.000000035	0.000000000	0.000006437	2446
# Time consumed by HG_Progress() when called with timeout==0
progress_elapsed_zero_timeout	0.000001059	0.000000715	0.000257015	2438
# Time consumed by HG_Progress() when called with timeout!=0
progress_elapsed_nonzero_timeout	0.000094414	0.000094414	0.000094414	1
# Timeout values passed to HG_Progress()
progress_timeout_value	0.041000410	100.000000000	100.000000000	2439
# RPC statistics
0xe282 0x0000 0x0000 0x0000 	0.006198764	0.002577782	0.016272545	4
# RPC statistics
0x9245 0x0000 0x0000 0x0000 	0.000098705	0.000098705	0.000098705	1
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

The user list of the mercury:

mercury@lists.mcs.anl.gov
