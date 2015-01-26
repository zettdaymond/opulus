import qbs 1.0

Product {
    name: "main"
    type: "application"
    files: "main.cpp"

    Depends { name: "Qt"; submodules: ["core" , "widgets"] }
    Depends { name: "cpp" }


    Depends {name : "libopulusGui"}
}
