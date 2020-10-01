declare namespace pythonjs {
    function run(code: string, context?: Array<{
        func: (...args: Array<any>) => any;
        name: string;
    }>): Number;

    var versions: {
        node: string;
        node_napi: string;
        python: string;
        python_capi: string;
    };
}

export = pythonjs;
