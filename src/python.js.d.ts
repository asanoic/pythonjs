interface FunctionInfo {
    func: (...args: Array<any>) => any;
    name: string;
}

interface PyModule {
    call(functionName: string, ...args: Array<any>): any;
}

class PythonJS {
    run(code: string, context?: Array<FunctionInfo>): Number;

    versions: {
        node: string;
        node_napi: string;
        python: string;
        python_capi: string;
    };

    import(moduleName: string, context?: Array<FunctionInfo>): PyModule;
}

export = new PythonJS();
