python.js
=========

All python codes now are part of node.js

This project uses node napi and python capi to make python a part of node.js, and only work on node.js (not browser)

For first version it only has one API `run`, it will accept a python code and return exitCode when it done.

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

this outputs current local time.

Calling js code on python side is also supported. All nodejs codes are buildin to python `nodejs` module, so only need import `nodejs`. On javascript side, pass function as well as its name (used on python side) as second parameter of `run`. it accepts array of function infomations. The example is like

```JavaScript
const pyjs = require("./python.js");
console.log(pyjs.versions);

const func = (a, b) => a.length * b;

const pyCode = `
from time import time,ctime
import nodejs

print('Today is:', ctime(time()))
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


Have fun! All python codes now are part of node.js!
