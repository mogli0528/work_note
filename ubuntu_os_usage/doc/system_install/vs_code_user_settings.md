# VS Code 用户配置


## 插件  

1) Python 补全  

Python 插件.   

2) C/CPP 代码补全   

* C/C++  
* C/C++ Clang Command Adapter   
* Clang Complete

~~~json
// c_cpp_properties.json

{
    "configurations": [
        {
            "name": "Linux",
            "includePath": [
                "${workspaceFolder}/**",
                "${workspaceFolder}/tdmc/topline_armline_v0.1/include"
            ],
            "defines": [],
            "compilerPath": "/usr/bin/clang",
            "cStandard": "c11",
            "cppStandard": "c++14",
            "intelliSenseMode": "clang-x64"
        }
    ],
    "version": 4
}
~~~

3) VS Code 中运行 Python 代码  

~~~json
// task.json
{
    // See https://go.microsoft.com/fwlink/?LinkId=733558
    // for the documentation about the tasks.json format
    "version": "2.0.0",
    "tasks": [
        {
            "label": "python3",
            "type": "shell",
            "command": "/home/klm/work/python-venv/py34TF/bin/python3",
            "args": [
                "${file}"
            ],
            "group": {
                "kind": "build",
                "isDefault": true
            }
        }
    ]
}
~~~


## User Setting  

```json
{
    "workbench.colorTheme": "Visual Studio Dark",
    "editor.minimap.enabled": false,
    "workbench.sideBar.location": "left",

    "editor.rulers": [79],
    "files.autoSave": "onFocusChange",
    "editor.renderWhitespace": "all",
    "editor.wordWrap": "on",
    "extensions.ignoreRecommendations": true,

    // cpp configurations
    // 打字很卡
    "search.followSymlinks": false,

    // 不显示错误信息  
    "C_Cpp.errorSquiggles": "Disabled",
    // Controls whether the IntelliSense engine will automatically switch to the Tag Parser for translation units containing #include errors.
    "C_Cpp.intelliSenseEngineFallback": "Enabled",
    "C_Cpp.dimInactiveRegions": false,

    "clang.executable": "clang",
    "clang.completion.enable": true,
    "clang.cxxflags": [
        "-std=c++11",
        "-I/usr/include/",
        "-I/usr/include/c++/4.8/",
        "-I/usr/include/boost/"
    ],
    "clang.cflags": [
        "-std=c99",
        "-Ic:/usr/include",
    ],

    // When enabled, Emmet abbreviations are expanded when pressing TAB.
    "emmet.triggerExpansionOnTab": true,
    "workbench.statusBar.feedback.visible": false,

    "[cpp]": {
      "editor.quickSuggestions": true
    },
    "[c]": {
        "editor.quickSuggestions": true
    },
    "window.zoomLevel": 0,

    // Choose the Math expression rendering method here. You can also disable math rendering if you want by choosing 'None'.
    "markdown-preview-enhanced.mathRenderingOption": "MathJax",
    "editor.largeFileOptimizations": false,

    // python
    "python.pythonPath": "/usr/lib/python3.4",

    // python tensorflow
    "python.autoComplete.extraPaths": [
        "/home/klm/work/python-venv/py34TF",
        "/home/klm/work/python-venv/py34TF/lib/python3.4/site-packages/",
        "/home/klm/work/python-venv/py34TF/lib/python3.4/site-packages/numpy/",
    ],
    // "workbench.statusBar.visible": true,
    // "files.autoGuessEncoding": true,
    // Automatically add brackets for functions.
    "python.autoComplete.addBrackets": true,
    // Insert snippets when their prefix matches. Works best when 'quickSuggestions' aren't enabled.
    "editor.tabCompletion": false,
    "python.autoComplete.preloadModules": [
        "tensorflow"
    ],
}
```

## Work Space Settings

~~~json
{
    "C_Cpp.intelliSenseEngineFallback": "Enabled",
    "C_Cpp.dimInactiveRegions": false,
    // python
    "python.pythonPath": "/usr/bin/python3.4",
}
~~~
