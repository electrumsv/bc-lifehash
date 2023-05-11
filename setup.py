import platform
from setuptools import Extension, setup
import sysconfig

base_compile_args = sysconfig.get_config_var('CFLAGS')
if base_compile_args is None:
    base_compile_args = ""
extra_compile_args = base_compile_args.split()

if platform.system() == "Darwin":
    extra_compile_args += ["-std=c++11"]

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
            extra_compile_args=extra_compile_args,
            language="c++11",
        ),
    ]
)