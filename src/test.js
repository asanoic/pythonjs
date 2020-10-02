const pyjs = require("./python.js");
console.log(pyjs.versions);

function closure() {
    let inside = 50;
    return (a, b) => (a.length * b) * inside;
}

function reverse(...args) {
   return args.reverse();
}

const pyCode = `
from time import time,ctime
import nodejs

print('Today is:', ctime(time()))
print('return list from node: ', nodejs.call('reverse', {'abc' : 12, 'xyz' : 21}, False, [1, 2], (3, 4), '5'))
print('result of node call:', nodejs.call('nodeMultiply', (4,2), 4.2))
`;

const context = [
    {
        func: closure(),
        name: "nodeMultiply",
    },
    {
        func: reverse,
        name: "reverse",
    },
    {
        func: x => x * 100,
        name: "helper",
    },
];

//const pyExitCode = pyjs.run(pyCode, context);
//console.log(pyExitCode);

const pyModule = pyjs.import("pycode", context);
console.log(pyModule.call("multiply", 3, 15));
