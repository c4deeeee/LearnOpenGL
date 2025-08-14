cmake_minimum_required(VERSION 3.16)

# 프로젝트 설정 (glad.c 빌드를 위해 C와 C++ 모두 활성화)
project(LearnOpenGL VERSION 1.0 LANGUAGES C CXX)

# 표준 / 인코딩 / 런타임
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if (MSVC)
	add_compile_options(/utf-8)
endif()

# 디렉터리 변수
set(ROOT_DIR ${CMAKE_SOURCE_DIR})
set(SRC_DIR  ${ROOT_DIR}/source)
set(LIB_DIR  ${ROOT_DIR}/library)

# GLAD
set(GLAD_INCLUDE_DIR ${LIB_DIR}/glad/include)
set(GLAD_SRC_FILE    ${LIB_DIR}/glad/src/glad.c)

# GLFW
set(GLFW_INCLUDE_DIR ${LIB_DIR}/glfw-3.4/include)
set(GLFW_LIB_FILE    ${LIB_DIR}/glfw-3.4/lib/glfw3.lib)  # 64bit

# 소스 수집
file(GLOB_RECURSE SRC_FILES "${SRC_DIR}/*.cpp")

# 실행 파일
add_executable(${PROJECT_NAME}
	${SRC_FILES}
	${GLAD_SRC_FILE}          # 반드시 포함 (LNK2019/2001 방지)
)

# 인클루드 경로
target_include_directories(${PROJECT_NAME} PRIVATE
	${SRC_DIR}
	${GLAD_INCLUDE_DIR}
	${GLFW_INCLUDE_DIR}
)

# 링크 (요청대로 GLFW 부분은 linker 한 줄만 유지)
target_link_libraries(${PROJECT_NAME} PRIVATE
	opengl32.lib
	"${GLFW_LIB_FILE}"
)

# 출력 경로 (bin/lib)
set_target_properties(${PROJECT_NAME} PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
	ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
	LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
)