const pyjs = require("./python.js");
console.log(pyjs.versions);

function closure() {
    let inside = 50;
    return (a, b) => {
    console.log(a, b);
    return (a.length * b) * inside;
    }
}

function combine(...args) {
   return args;
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
print('result of node call:', nodejs.call('nodeMultiplication', (4,2), 42.1))
print('length of return list: ', len(nodejs.call('combine', 1, 2, (3, 4), '5')))
`;

const context = [
    {
        func: closure(),
        name: "nodeMultiplication",
    },
    {
        func: combine,
        name: "combine",
    },
];

const pyExitCode = pyjs.run(pyCode, context);

console.log(pyExitCode);
