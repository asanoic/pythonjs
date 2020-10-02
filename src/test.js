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

def multiply(a,b):
    print("Will compute", a, "times", b)
    c = 0
    for i in range(0, a):
        c = c + b
    return c

print('Today is:', ctime(time()))
print('return list from node: ', nodejs.call('reverse', {'abc' : 12, 'xyz' : 21}, False, [1, 2], (3, 4), '5'))
print('result of node call:', nodejs.call('nodeMultiplication', (4,2), 4.2))
`;

const context = [
    {
        func: closure(),
        name: "nodeMultiplication",
    },
    {
        func: reverse,
        name: "reverse",
    },
];

const pyExitCode = pyjs.run(pyCode, context);

console.log(pyExitCode);
