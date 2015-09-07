# C++ source files
set(SOURCE_FILES_SKIP_LIST
${SourcePath}/skip_list.h
${SourcePath}/skip_list.hpp
${SourcePath}/random.h
)

set(SOURCE_FILES_TEST
${TestPath}/main.cpp
)

# set appropriate source groups
source_group(skip_list  FILES  ${SOURCE_FILES_SKIP_LIST})
source_group(test  FILES  ${SOURCE_FILES_TEST})

# set all source files
set(SOURCE_FILES
${SOURCE_FILES_SKIP_LIST}
${SOURCE_FILES_TEST}
)