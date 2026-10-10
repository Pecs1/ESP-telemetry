file(GLOB_RECURSE SHARED_SRCS "../atria/*.cpp")
idf_component_register(
    SRCS 
        "../boards/${BOARD_ENV_PATH}/main.cpp"
        ${SHARED_SRCS}
    PRIV_INCLUDE_DIRS 
        ".."
        "../boards/${BOARD_ENV_PATH}"
)
