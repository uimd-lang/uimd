cmake_minimum_required(VERSION 3.20)

project(ui_cpp_runtime VERSION 0.3.0 LANGUAGES CXX)

if(NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS Debug Release RelWithDebInfo MinSizeRel)
endif()

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

find_package(Threads REQUIRED)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(UI_CPP_ENABLE_SANITIZERS "Enable address and undefined behavior sanitizers" OFF)
option(UIMD_BUILD_EXAMPLES "Build UIMD C++ examples" ON)
option(UIMD_BUILD_TESTS "Build UIMD C++ tests" ON)
option(UIMD_INSTALL "Install UIMD C++ targets" ON)

add_library(ui_cpp_runtime STATIC
    src/app/Application.cpp
    third_party/lodepng/lodepng.cpp
    src/app/Window.cpp
    src/core/Color.cpp
    src/core/Element.cpp
    src/core/Layout.cpp
    src/core/Style.cpp
    src/core/TextVisual.cpp
    src/core/Version.cpp
    src/elements/BasicElements.cpp
    src/elements/FrameBufferView.cpp
    src/elements/Image.cpp
    src/elements/InfoLabel.cpp
    src/elements/MessageTable.cpp
    src/elements/ReusableElement.cpp
    src/elements/ScrollView.cpp
    src/generated/GeneratedWindowRuntime.cpp
    src/terminal/Clipboard.cpp
    src/terminal/Input.cpp
    src/terminal/TerminalBackend.cpp
    src/terminal/TerminalBuffer.cpp
)

target_compile_definitions(ui_cpp_runtime PRIVATE UIMD_VERSION="${PROJECT_VERSION}")

target_include_directories(ui_cpp_runtime
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/third_party/lodepng>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/third_party/stb>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_link_libraries(ui_cpp_runtime PUBLIC Threads::Threads ${CMAKE_DL_LIBS})
target_compile_features(ui_cpp_runtime PUBLIC cxx_std_17)
set_target_properties(ui_cpp_runtime PROPERTIES EXPORT_NAME runtime)
add_library(uimd::runtime ALIAS ui_cpp_runtime)

# libsixel is intentionally runtime-loaded by Image when non-fallback Sixel
# rendering is required. Do not link apps directly to libsixel here; apps that do
# not use Sixel images must still launch without the system library installed.
message(STATUS "libsixel: runtime-loaded for non-fallback Image rendering")

add_library(ui_cpp_dialogs STATIC
    dialogs/file_browser.cpp
    dialogs/file_browser_ui.cpp
    dialogs/message_box.cpp
    dialogs/message_box_ok_ui.cpp
    dialogs/message_box_ok_cancel_ui.cpp
    dialogs/message_box_yes_no_ui.cpp
    dialogs/message_box_yes_no_cancel_ui.cpp
)

target_include_directories(ui_cpp_dialogs
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/dialogs>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/uimd/dialogs>
)

target_link_libraries(ui_cpp_dialogs PUBLIC ui_cpp_runtime)
target_compile_features(ui_cpp_dialogs PUBLIC cxx_std_17)
set_target_properties(ui_cpp_dialogs PROPERTIES EXPORT_NAME dialogs)
add_library(uimd::dialogs ALIAS ui_cpp_dialogs)

add_executable(uimd_mcp_tester
    tools/mcp_tester/main.cpp
    tools/mcp_tester/log_panel_ui.cpp
    tools/mcp_tester/mcp_tester_ui.cpp
)

target_include_directories(uimd_mcp_tester PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/mcp_tester
)
target_link_libraries(uimd_mcp_tester PRIVATE ui_cpp_runtime Threads::Threads)
target_compile_features(uimd_mcp_tester PRIVATE cxx_std_17)
set_target_properties(uimd_mcp_tester
    PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/tools/mcp_tester
)

add_executable(uimd
    tools/uimd/main.cpp
    tools/uimd/NativeGrid.cpp
    tools/uimd/NativeCppGenerator.cpp
    tools/uimd/NativeModel.cpp
    tools/uimd/NativePythonGenerator.cpp
)

target_compile_definitions(uimd PRIVATE
    UIMD_VERSION="${PROJECT_VERSION}"
    UIMD_NATIVE_SOURCE_ROOT="${CMAKE_CURRENT_SOURCE_DIR}/.."
)
target_compile_features(uimd PRIVATE cxx_std_17)
set_target_properties(uimd
    PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/tools/uimd
)

add_executable(uimd_init
    tools/uimd_init/main.cpp
)

target_compile_definitions(uimd_init PRIVATE
    UIMD_VERSION="${PROJECT_VERSION}"
)
target_compile_features(uimd_init PRIVATE cxx_std_17)
set_target_properties(uimd_init
    PROPERTIES
        OUTPUT_NAME uimd-init
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/tools/uimd_init
)

if(MSVC)
    target_compile_options(ui_cpp_runtime PRIVATE /W4 /permissive-)
else()
    target_compile_options(ui_cpp_runtime PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(uimd_mcp_tester PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(uimd PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(uimd_init PRIVATE -Wall -Wextra -Wpedantic)
endif()

if(UI_CPP_ENABLE_SANITIZERS AND NOT MSVC)
    target_compile_options(ui_cpp_runtime PRIVATE -fsanitize=address,undefined -fno-omit-frame-pointer)
    target_link_options(ui_cpp_runtime PRIVATE -fsanitize=address,undefined)
endif()

if(UIMD_BUILD_TESTS)
    enable_testing()

    add_executable(ui_cpp_tests
        tests/test_runtime_skeleton.cpp
        tests/test_dialogs.cpp
    )

    target_link_libraries(ui_cpp_tests PRIVATE ui_cpp_runtime ui_cpp_dialogs)
    target_compile_definitions(ui_cpp_tests PRIVATE UIMD_EXPECTED_VERSION="${PROJECT_VERSION}")

    add_test(NAME ui_cpp_tests COMMAND ui_cpp_tests)
endif()

function(add_ui_cpp_example name)
    add_executable(${name}
        examples/${name}/${name}.cpp
        examples/${name}/${name}_ui.cpp
    )
    target_link_libraries(${name} PRIVATE ui_cpp_runtime)
    set_target_properties(${name}
        PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/examples/${name}
    )
    if(UIMD_BUILD_TESTS)
        add_test(NAME example_${name} COMMAND ${name} --smoke)
    endif()
endfunction()

if(UIMD_BUILD_EXAMPLES)
    add_ui_cpp_example(calculator)
    add_ui_cpp_example(formular)
    add_ui_cpp_example(cells)
    add_ui_cpp_example(special_elements)
    add_ui_cpp_example(widget_gallery)
    add_ui_cpp_example(image_browser)
    target_sources(image_browser PRIVATE
        examples/image_browser/image_view_ui.cpp
        examples/image_browser/image_button_ui.cpp
        examples/image_browser/gallery_item_ui.cpp
        examples/image_browser/gallery_mosaic_ui.cpp
        examples/image_browser/gallery_scroll_ui.cpp
        examples/image_browser/gallery_view_ui.cpp
        examples/image_browser/image_list_button_ui.cpp
        examples/image_browser/image_list_item_ui.cpp
        examples/image_browser/image_list_scroll_ui.cpp
        examples/image_browser/image_list_view_ui.cpp
        examples/image_browser/image_show_dialog_ui.cpp
    )
    target_link_libraries(image_browser PRIVATE ui_cpp_dialogs)
    add_ui_cpp_example(image_gallery)
    add_ui_cpp_example(text_editor)
    target_link_libraries(text_editor PRIVATE ui_cpp_dialogs)
    add_ui_cpp_example(contacts_manager)
    target_sources(contacts_manager PRIVATE
        examples/contacts_manager/contact_form/contact_form.cpp
        examples/contacts_manager/contact_form/contact_form_ui.cpp
    )
    target_link_libraries(contacts_manager PRIVATE ui_cpp_dialogs)
    add_ui_cpp_example(markdown_viewer)
    target_sources(markdown_viewer PRIVATE
        examples/markdown_viewer/document_view/document_view.cpp
        examples/markdown_viewer/document_view/document_view_ui.cpp
        examples/markdown_viewer/blocks/code_block.cpp
        examples/markdown_viewer/blocks/code_block_ui.cpp
        examples/markdown_viewer/blocks/heading.cpp
        examples/markdown_viewer/blocks/heading_ui.cpp
        examples/markdown_viewer/blocks/paragraph.cpp
        examples/markdown_viewer/blocks/paragraph_ui.cpp
        examples/markdown_viewer/blocks/table_block.cpp
        examples/markdown_viewer/blocks/table_block_ui.cpp
    )
    add_ui_cpp_example(activity_feed)
    target_sources(activity_feed PRIVATE
        examples/activity_feed/activity_feed_panel/activity_feed_panel.cpp
        examples/activity_feed/activity_feed_panel/activity_feed_panel_ui.cpp
        examples/activity_feed/activity_item/activity_item.cpp
        examples/activity_feed/activity_item/activity_item_ui.cpp
        examples/activity_feed/settings/settings.cpp
        examples/activity_feed/settings/settings_ui.cpp
    )
    add_ui_cpp_example(task_board)
    target_sources(task_board PRIVATE
        examples/task_board/task_list/task_list.cpp
        examples/task_board/task_list/task_list_ui.cpp
        examples/task_board/task_row/task_row_ui.cpp
        examples/task_board/task_dialog/task_dialog.cpp
        examples/task_board/task_dialog/task_dialog_ui.cpp
    )
    target_link_libraries(task_board PRIVATE ui_cpp_dialogs)
    add_ui_cpp_example(expense_tracker)
    target_sources(expense_tracker PRIVATE
        examples/expense_tracker/expenses_view_ui.cpp
        examples/expense_tracker/categories_view_ui.cpp
        examples/expense_tracker/settings_view_ui.cpp
        examples/expense_tracker/expenses_list/expenses_list.cpp
        examples/expense_tracker/expenses_list/expenses_list_ui.cpp
        examples/expense_tracker/expense_row/expense_row_ui.cpp
    )
    target_link_libraries(expense_tracker PRIVATE ui_cpp_dialogs)

    if(UIMD_BUILD_TESTS)
        add_test(NAME example_activity_feed_logic COMMAND activity_feed --logic-test)
        add_test(NAME example_task_board_logic COMMAND task_board --logic-test)
        add_test(NAME example_expense_tracker_logic COMMAND expense_tracker --logic-test)
        add_test(NAME example_calculator_logic COMMAND calculator --logic-test)
        add_test(NAME example_formular_logic COMMAND formular --logic-test)
        add_test(NAME example_cells_logic COMMAND cells --logic-test)
        add_test(NAME example_special_elements_logic COMMAND special_elements --logic-test)
        add_test(NAME example_widget_gallery_logic COMMAND widget_gallery --logic-test)
        add_test(NAME example_image_gallery_logic COMMAND image_gallery --logic-test)
        add_test(NAME example_text_editor_logic COMMAND text_editor --logic-test)
        add_test(NAME example_contacts_manager_logic COMMAND contacts_manager --logic-test)
        add_test(NAME example_markdown_viewer_logic COMMAND markdown_viewer --logic-test)
    endif()
endif()

if(UIMD_INSTALL)
    install(
        TARGETS ui_cpp_runtime ui_cpp_dialogs uimd uimd_init
        EXPORT uimdTargets
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    )
    install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
    install(
        FILES
            dialogs/file_browser.hpp
            dialogs/message_box.hpp
            dialogs/message_box_ok_ui.hpp
            dialogs/message_box_ok_cancel_ui.hpp
            dialogs/message_box_yes_no_ui.hpp
            dialogs/message_box_yes_no_cancel_ui.hpp
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/uimd/dialogs
    )
    install(
        EXPORT uimdTargets
        FILE uimdTargets.cmake
        NAMESPACE uimd::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/uimd
    )
    configure_package_config_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/cmake/uimdConfig.cmake.in
        ${CMAKE_CURRENT_BINARY_DIR}/uimdConfig.cmake
        INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/uimd
    )
    write_basic_package_version_file(
        ${CMAKE_CURRENT_BINARY_DIR}/uimdConfigVersion.cmake
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY SameMajorVersion
    )
    install(
        FILES
            ${CMAKE_CURRENT_BINARY_DIR}/uimdConfig.cmake
            ${CMAKE_CURRENT_BINARY_DIR}/uimdConfigVersion.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/uimd
    )
endif()
