const fs = require("fs");

const pyjs = require("./python.js");
console.log(pyjs);

const pyCode = fs.readFileSync("./pycode.py", "utf-8");

function Multiplication(a, b) {
    return a * b.length;
}

const pyExitCode = pyjs.run(pyCode, [{name: "nodeMultiplication", func: Multiplication}]);

console.log(pyExitCode);
