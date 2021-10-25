mkdir include
mkdir src
mkdir build
echo "cmake_minimum_required(VERSION 3.12) 

set(PROJECT_NAME $1)
set(CMAKE_EXPORT_COMPILE_COMMANDS on)

project(\${PROJECT_NAME})
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_FLAGS \"-g\")

file(GLOB SRC_FILES \"src/*.c\")
include_directories(\"include/\")

add_executable(\${PROJECT_NAME} \${SRC_FILES})" >> CMakeLists.txt

echo '

int main(){

    return 0;
}
' >> ./src/main.c

echo "{
    \"configurations\":{
        \"Launch\":{
            \"adapter\": \"vscode-cpptools\",
            \"configuration\": {
                \"request\": \"launch\",
                \"program\": \"build/$1\",
                \"cwd\": \"\${workspaceFolder}\",
                \"externalConsole\": true,
                \"MiMode\": \"gdb\"
            }
        }
    }
}" >> .vimspector.json
cmake -S ./ -B ./build/
ln -s build/compile_commands.json ./compile_commands.json


