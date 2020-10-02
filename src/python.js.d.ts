declare namespace pythonjs {
    interface FunctionInfo {
        func: (...args: Array<any>) => any;
        name: string;
    }

    function run(code: string, context?: Array<FunctionInfo>): Number;

    var versions: {
        node: string;
        node_napi: string;
        python: string;
        python_capi: string;
    };

    interface PyModule {
        call(functionName: string, context?: Array<FunctionInfo>): any;
    }

    function import(moduleName: string, context?: Array<FunctionInfo>): PyModule;
}

export = pythonjs;
