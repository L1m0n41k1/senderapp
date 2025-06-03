# WhatsApp Sender Native - CMake Project File
cmake_minimum_required(VERSION 3.10)
project(WhatsAppSenderNative VERSION 1.0.0 LANGUAGES CXX)

# Настройка Qt
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Находим пакеты Qt
find_package(Qt5 COMPONENTS Core Widgets Gui Network Sql REQUIRED)

# Добавляем файлы ресурсов
set(RESOURCE_FILES
    resources/resources.qrc
)

# Исходные файлы
set(SOURCE_FILES
    src/main.cpp
)

# Заголовочные файлы
set(HEADER_FILES
)

# Создаем исполняемый файл
add_executable(${PROJECT_NAME} 
    ${SOURCE_FILES}
    ${HEADER_FILES}
    ${RESOURCE_FILES}
)

# Подключаем библиотеки Qt
target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt5::Core
    Qt5::Widgets
    Qt5::Gui
    Qt5::Network
    Qt5::Sql
)

# Включаем C++11
set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 11
    CXX_STANDARD_REQUIRED ON
)

# Устанавливаем выходной каталог
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
