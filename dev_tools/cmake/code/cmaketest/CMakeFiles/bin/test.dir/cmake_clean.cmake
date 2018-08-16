FILE(REMOVE_RECURSE
  "CMakeFiles/bin/test.dir/src/main.cpp.o"
  "CMakeFiles/bin/test.dir/src/foo.cpp.o"
  "bin/test.pdb"
  "bin/test"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang CXX)
  INCLUDE(CMakeFiles/bin/test.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
