idf_component_register(SRCS header.c support.c wrappers.c handlers.c tasks.c
        INCLUDE_DIRS "include"
        PRIV_INCLUDE_DIRS "priv_include"
        REQUIRES ieee802154
)