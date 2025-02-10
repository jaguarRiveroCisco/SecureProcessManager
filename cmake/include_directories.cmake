# include_directories.cmake

include_directories(
    include/consolecontroller
    include/maincontroller
    include/processcontroller
    include/processmonitors
    include/process
    include/process/networkprocess
    include/process/systemprocess
    include/concurrency
    include/tools
    include/logger
    include/networkprocess
    include/systemprocess
    include/config
    include/api
    /opt/homebrew/Cellar/asio/1.30.2/include  # Add the ASIO include directory
)
