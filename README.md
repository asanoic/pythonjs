python.js
=========

All python codes now are part of node.js

This project uses node napi and python capi to make python a part of node.js. This library only works on node.js (not browser)

API `run` is a good start when you use python.js, it accepts a python code and return exitCode when it done.

Helle world, example

```JavaScript
const pyjs = require("./python.js");
console.log(pyjs.versions);

const pyCode = `
from time import time,ctime
print('Say "Hello World" at', ctime(time()))
`;

const pyExitCode = pyjs.run(pyCode);
console.log(pyExitCode);
```

this outputs Hello World with current local time.

Calling js code on python side is also supported. All nodejs codes are builtin to python `nodejs` module, so only need import `nodejs`. On javascript side, pass function as well as its name (used on python side) as second parameter of `run`. `run` accepts array of function infomations. The example is like

```JavaScript
const pyjs = require("./python.js");
console.log(pyjs.versions);

const func = (a, b) => a.length * b;

const pyCode = `
from time import time,ctime
# import nodejs module for nodejs host context
import nodejs

print('Today is:', ctime(time()))
# call nodeMultiplication using nodejs.call
print('result of node call:', nodejs.call('nodeMultiplication', '42', 100))
`;

const context = [
    {
        func,
        name: "nodeMultiplication",
    },
];

const pyExitCode = pyjs.run(pyCode, context);

console.log(pyExitCode);
```

JavaScript calling also supports closure

```JavaScript
const pyjs = require("./python.js");
console.log(pyjs.versions);

function closure() {
    let inside = 50;
    return (a, b) => (a.length * b) * inside;
}

const pyCode = `
from time import time,ctime
import nodejs

print('Today is:', ctime(time()))
print('result of node call:', nodejs.call('nodeMultiplication', '42', 42))
`;

const context = [
    {
        func: closure(),
        name: "nodeMultiplication",
    },
];

const pyExitCode = pyjs.run(pyCode, context);

console.log(pyExitCode);
```

call `import` and `call` can invoke seperate python function using javascript.

more api declaration is in file `$/src/python.js.d.ts`. 

Have fun! All python codes now are part of node.js!


how to build
============

__if you don't want to make a build from source, try demo on release section.__

```
   git clone https://github.com/asanoic/pythonjs.git --recursive
   cmake [python-js-src-path] -DSEARCH_PATH=[gawk path]
   cmake --build .
```
`gawk` is for patching some files to make them work on mingw-w64, you can find it in [MSYS2](https://www.msys2.org/)
on windows you might need copy `python39.dll` and `lib` [$/capi/python/lib] to same folder of `python.js.node`, if you do not have python installed.
build work on Linux and Windows/MinGW-w64, other platforms or compilers might need change cmake file.

build requirement
===========
- C++ compiler supports C++14, recommand use [this](https://github.com/asanoic/asanoic-mingw64-build/releases)
- Python 3.x, Python 3.9.1 is included in this repo for windows, on linux you can build from source based on your install.
- napi headers which is included in this repo.

