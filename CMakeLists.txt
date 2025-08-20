cmake_minimum_required(VERSION 3.28)

# --- Project ---
project(LearnOpenGL LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- Windows 전용 ---
if(NOT WIN32)
  message(FATAL_ERROR "This project is configured for Windows/MSVC only.")
endif()

# --- 경로 설정 ---
set(ROOT_DIR ${CMAKE_SOURCE_DIR})
set(SRC_DIR  ${ROOT_DIR}/source)
set(LIB_DIR  ${ROOT_DIR}/library)
set(RESC_DIR ${ROOT_DIR}/resource) 

# --- vcpkg 통합 제거 ---
set(VCPKG_APPLOCAL_DEPS OFF CACHE BOOL "Disable vcpkg applocal" FORCE)
set(CMAKE_TOOLCHAIN_FILE "" CACHE STRING "No toolchain (disable vcpkg)" FORCE)

# --- 출력 경로 ---
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# --- MSVC 런타임 일관성 (/MD[d]) ---
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")

# --- 소스 자동 수집 ---
file(GLOB_RECURSE PROJECT_SOURCES CONFIGURE_DEPENDS
    "${SRC_DIR}/*.c" "${SRC_DIR}/*.cpp"
)
file(GLOB_RECURSE PROJECT_HEADERS CONFIGURE_DEPENDS
    "${SRC_DIR}/*.h" "${SRC_DIR}/*.hpp"
)

# glad.c 는 library/에 있으므로 명시적으로 추가
set(GLAD_C "${LIB_DIR}/glad/src/glad.c")

# --- 실행 파일 ---
add_executable(${PROJECT_NAME}
    ${PROJECT_SOURCES}
    ${PROJECT_HEADERS}
    ${GLAD_C}
)

# --- 포함 경로 ---
target_include_directories(${PROJECT_NAME} PRIVATE
    ${SRC_DIR}
    ${LIB_DIR}/glad/include
    ${LIB_DIR}/glfw-3.4/include
    ${LIB_DIR}/stb_image/include
    ${LIB_DIR}/glm-1.0.1-light
)

# --- 링킹 ---
set(GLFW_LIB_FILE "${LIB_DIR}/glfw-3.4/lib/glfw3.lib")
target_link_libraries(${PROJECT_NAME} PRIVATE
    opengl32.lib
    "${GLFW_LIB_FILE}"
)

# --- 셰이더 복사(빌드 후) ---
# source/GLSL -> (빌드폴더)/bin/GLSL 로 복사
set(SHADER_SRC_DIR "${SRC_DIR}/GLSL")
set(SHADER_DST_DIR "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/GLSL")
set(TEXTURE_SRC_DIR "${RESC_DIR}")
set(TEXTURE_DST_DIR "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/texture")

# 개별 파일 지정 (요청: vertex.glsl, fragment.glsl)
set(SHADER_FILES
    "${SHADER_SRC_DIR}/vertex.glsl"
    "${SHADER_SRC_DIR}/fragment.glsl"
)

# 빌드 후 항상 복사 (Debug/Release 모두 해당 빌드 폴더로 복사됨)
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -E make_directory "${SHADER_DST_DIR}"
    COMMAND "${CMAKE_COMMAND}" -E copy_if_different
            "${SHADER_SRC_DIR}/vertex.glsl"   "${SHADER_DST_DIR}/vertex.glsl"
    COMMAND "${CMAKE_COMMAND}" -E copy_if_different
            "${SHADER_SRC_DIR}/fragment.glsl" "${SHADER_DST_DIR}/fragment.glsl"
    COMMAND "${CMAKE_COMMAND}" -E make_directory "${TEXTURE_DST_DIR}"
    COMMAND "${CMAKE_COMMAND}" -E copy_if_different
            "${TEXTURE_SRC_DIR}/container.jpg"   "${TEXTURE_DST_DIR}/container.jpg"
    COMMAND "${CMAKE_COMMAND}" -E copy_if_different
            "${TEXTURE_SRC_DIR}/awesomeface.png"   "${TEXTURE_DST_DIR}/awesomeface.png"
)

# --- 경고 수준/최적화(선택) ---
if(MSVC)
  target_compile_options(${PROJECT_NAME} PRIVATE /W4 /permissive-)
endif()