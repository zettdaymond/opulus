import qbs 1.0

DynamicLibrary {
    name : "libopulusGui"
    Depends { name : "Qt"; submodules : ["core","gui", "svg", "xml", "printsupport"] }
    Depends { name : "cpp"}

    Depends { name : "libopulusCore" }

    cpp.includePaths: ["."]

    files : [
        "*.h",
        "*.cpp",

        "commands/*.h",
        "commands/*.cpp",

        "mainwindow.ui",
        "analyserstatus.ui",
        "matrixwidget.ui",

        "gui.qrc",
    ]

    Export {
       Depends { name: "cpp" }
       Depends { name: "Qt"; submodules : ["core","gui", "svg", "xml", "printsupport"] }
       Depends { name : "libopulusCore" }
       cpp.includePaths: ["."]
   }
}
