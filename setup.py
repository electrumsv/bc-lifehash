from setuptools import Extension, setup

setup(
    ext_modules=[
        Extension(
            name="lifehash",
            sources=[
                "src/bit-aggregator.cpp",
                "src/bit-enumerator.cpp",
                "src/cell-grid.cpp",
                "src/color-func.cpp",
                "src/color-grid.cpp",
                "src/color.cpp",
                "src/format-utils.cpp",
                "src/gradients.cpp",
                "src/hex.cpp",
                "src/hsb-color.cpp",
                "src/lifehash.cpp",
                "src/memzero.cpp",
                "src/patterns.cpp",
                "src/point.cpp",
                "src/sha256.cpp",
                "src/lifehash-python.cpp",
            ],
        ),
    ]
)