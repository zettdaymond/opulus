import qbs 1.0

DynamicLibrary {
    name: "libopulusCore"
    Depends { name: "Qt"; submodules : ["core", "xml"]}
    Depends { name: "cpp" }

    files : [
        "*.h",
        "petrinet.cpp",
        "item.cpp",
        "node.cpp",
        "place.cpp",
        "transition.cpp",
        "abstractarc.cpp",
        "arc.cpp",
        "inhibitorarc.cpp",
        "simulation.cpp",
        "marking.cpp",
        "analysisrunner.cpp",
        "analysisreporter.cpp",
        "matrix_util.cpp"
    ]

    Export {
       Depends { name: "cpp" }
       Depends { name: "Qt"; submodules : ["core", "xml"]}
       cpp.includePaths: ["."]
   }
}
