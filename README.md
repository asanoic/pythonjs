python.js
=========

All python codes now are part of node.js

This project uses node napi and python capi to make python a part of node.js. This library only works on node.js (not browser)

For first version, it only has one API `run`, it will accept a python code and return exitCode when it done.

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
import nodejs                # import nodejs module for nodejs host context

print('Today is:', ctime(time()))
print('result of node call:', nodejs.call('nodeMultiplication', '42', 100)) # call nodeMultiplication using nodejs.call
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


Have fun! All python codes now are part of node.js!


how to build
============

```
   git clone https://github.com/asanoic/pythonjs.git --recursive
   cmake [python-js-src-path]
   cmake --build .
```   
on windows you might need copy `python38.dll` and `python38.zip` [$/python-capi/win] to same folder of `python.js.node`, if you do not have python installed.

build requirement
===========
- C++ compiler supports C++14, recommand use [this](https://github.com/asanoic/asanoic-mingw64-build/releases)
- Python 3.x, Python 3.8.6 is included in this repo for windows, on linux you can build from source based on your install.
- napi headers which is included in this repo.

