# Collect all immediate subdirectories of src/
file(GLOB SUBDIRS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
     "${CMAKE_CURRENT_SOURCE_DIR}/*")

# We'll store all module libraries here
set(MODULE_LIBS "")

# ===== PASS 1: create one library per subdirectory =====
foreach(dir ${SUBDIRS})
    if(NOT IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/${dir}")
        continue()
    endif()

    # Target name for the library = directory name
    set(lib_target "${dir}")

    # All .cpp sources in this subdirectory, except main.cpp
    file(GLOB_RECURSE module_sources CONFIGURE_DEPENDS
        "${dir}/*.cpp"
    )

    # Remove main.cpp from library sources (if present)
    foreach(src ${module_sources})
        if(src MATCHES "/main\\.cpp$")
            list(REMOVE_ITEM module_sources "${src}")
        endif()
    endforeach()

    # Skip if there is no source file other than main.cpp
    if(NOT module_sources)
        message(STATUS "Skipping library for ${dir}: no non-main sources")
    else()
        message(STATUS "Adding module library: ${lib_target}")
        add_library(${lib_target} ${module_sources})

        # find_package(fmt CONFIG REQUIRED)
        # target_link_libraries(${lib_target} PUBLIC fmt::fmt-header-only)

        # Public include dirs (adjust as you like)
        target_include_directories(${lib_target}
            PUBLIC
                ${CMAKE_CURRENT_SOURCE_DIR}/${dir}
                ${CMAKE_CURRENT_SOURCE_DIR}/../include
        )

        # Collect library for later linking
        list(APPEND MODULE_LIBS ${lib_target})
    endif()
endforeach()

# ===== PASS 2: create executables for directories with main.cpp =====
foreach(dir ${SUBDIRS})
    if(NOT IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/${dir}")
        continue()
    endif()

    set(main_source "${CMAKE_CURRENT_SOURCE_DIR}/${dir}/main.cpp")
    if(EXISTS "${main_source}")
        # Executable name = "<dir>_exe" (change if you prefer)
        set(exe_target "${dir}_exe")

        message(STATUS "Adding executable: ${exe_target} (from ${dir}/main.cpp)")
        add_executable(${exe_target} "${main_source}")

        # Link to its own library if it exists
        if("${dir}" IN_LIST MODULE_LIBS)
            target_link_libraries(${exe_target} PRIVATE ${dir})
        endif()

        # Optionally link to all other module libs for easy cross-usage
        foreach(lib ${MODULE_LIBS})
            if(NOT lib STREQUAL "${dir}")  # avoid duplicate self-link
                target_link_libraries(${exe_target} PRIVATE ${lib})
            endif()
        endforeach()
    endif()
endforeach()