# C++ source files
set(SOURCE_FILES_SKIP_LIST
${SourcePath}/skip_list.h
${SourcePath}/skip_list.hpp
${SourcePath}/random.h
)

set(SOURCE_FILES_TEST
${TestPath}/main.cpp
${TestPath}/tests.h
)

set(SOURCE_FILES_BENCH
${BenchPath}/benchmark.cpp
)

# set appropriate source groups
source_group(skip_list  FILES  ${SOURCE_FILES_SKIP_LIST})
source_group(test  FILES  ${SOURCE_FILES_TEST})
source_group(benchmark  FILES  ${SOURCE_FILES_BENCH})


set(SOURCE_FILES_TEST_PROJ
${SOURCE_FILES_TEST}
${SOURCE_FILES_SKIP_LIST}
)

set(SOURCE_FILES_BENCH_PROJ
${SOURCE_FILES_BENCH}
${SOURCE_FILES_SKIP_LIST}
)
