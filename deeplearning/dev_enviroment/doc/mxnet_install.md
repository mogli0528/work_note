## Installing MXNet

Indicate your preferred configuration. Then, follow the customized commands to install MXNet.

LinuxMacOSWindowsCloudDevices
PythonScalaRJuliaPerl
CPUGPU
PipVirtualenvDockerBuild from Source
The following installation instructions have been tested on Ubuntu 14.04 and 16.04.

Prerequisites

Install the following NVIDIA libraries to setup MXNet with GPU support:

Install CUDA 9.0 following the NVIDIA’s installation guide.
Install cuDNN 7 for CUDA 9.0 following the NVIDIA’s installation guide. You may need to register with NVIDIA for downloading the cuDNN library.
Note: Make sure to add CUDA install path to LD_LIBRARY_PATH.

Example - export LD_LIBRARY_PATH=/usr/local/cuda/lib64/:$LD_LIBRARY_PATH


Step 1 Install prerequisites - wget and latest pip.

Installing MXNet with pip requires a latest version of pip. Install the latest version of pip by issuing the following command in the terminal.

$ sudo apt-get update
$ sudo apt-get install -y wget python
$ wget https://bootstrap.pypa.io/get-pip.py && sudo python get-pip.py
Step 2 Install MXNet with GPU support using CUDA 9.0

$ pip install mxnet-cu90
Step 3 Install Graphviz. (Optional, needed for graph visualization using mxnet.viz package).

sudo apt-get install graphviz
pip install graphviz
Step 4 Validate the installation by running simple MXNet code described here.

Experimental Choice If You would like to install mxnet with Intel MKL, try the experimental pip package with MKL:

$ pip install mxnet-cu90mkl
Validate MXNet Installation

Start the python terminal.

$ python
Run a short MXNet python program to create a 2X3 matrix of ones a on a GPU, multiply each element in the matrix by 2 followed by adding 1. We expect the output to be a 2X3 matrix with all elements being 3. We use mx.gpu(), to set MXNet context to be GPUs.

>>> import mxnet as mx
>>> a = mx.nd.ones((2, 3), mx.gpu())
>>> b = a * 2 + 1
>>> b.asnumpy()
array([[ 3.,  3.,  3.],
       [ 3.,  3.,  3.]], dtype=float32)