find src -iname '*.hpp' -o -iname '*.cpp' | xargs clang-format -i
find src -iname '*.h' -o -iname '*.tpp' | xargs clang-format -i
find tests -iname '*.hpp' -o -iname '*.cpp' | xargs clang-format -i
find tests -iname '*.h' -o -iname '*.tpp' | xargs clang-format -i
prettier --write */*/*/*/*/*/*.json > /dev/null 2>&1
prettier --write */*/*/*/*/*/*.md > /dev/null 2>&1
autopep8 -i tools/*.py
autopep8 -i conan/conanfile.py
yamlfmt .github/workflows
cmake-format -i CMakeLists.txt