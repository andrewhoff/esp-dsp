# Object name in the cmake project
set(OBJECT_NAME esp-dsp)

# Object file name in the file manager
set(OBJECT_FILE_NAME _${OBJECT_NAME}_all_${VARIANT})

set(COMPONENT_SRCS  "modules/common/misc/dsps_pwroftwo.cpp"
                    "modules/fft/float/dsps_fft2r_fc32_ansi.c"
                    "modules/fft/float/dsps_fft2r_fc32_ae32.S")

set(COMPONENT_ADD_INCLUDEDIRS   "modules/fft/include"
                                "modules/common/include")

set(COMPONENT_PRIV_INCLUDEDIRS  "modules/common/private_include")

set(INCS ${COMPONENT_ADD_INCLUDEDIRS} ${COMPONENT_PRIV_INCLUDEDIRS} idf)

set(ESP32_INCLUDE)

foreach(inc_file ${INCS})

    # Convert the relative path to an absolute one - for the include propagation
    get_filename_component(INC ${inc_file} ABSOLUTE)

    set(ESP32_INCLUDE ${ESP32_INCLUDE}
            ${INC}
            )

endforeach()

set(CHIRP_MATHS_INCLUDE ${CHIRP_MATHS_INCLUDE}
        ${ESP32_INCLUDE}
        PARENT_SCOPE
        )

# Create the object gathering
add_library(${OBJECT_FILE_NAME} OBJECT
    ${COMPONENT_SRCS}
    )

# Propagate object dependencies to the top level
set(CHIRP_MATHS_OBJECT ${CHIRP_MATHS_OBJECT}
        ${OBJECT_FILE_NAME}
        PARENT_SCOPE
        )

target_include_directories(${OBJECT_FILE_NAME} PRIVATE
    ${COMPONENT_ADD_INCLUDEDIRS}
    ${COMPONENT_PRIV_INCLUDEDIRS}
    idf
    )

target_compile_definitions(${OBJECT_FILE_NAME} PRIVATE
    CONFIG_DSP_OPTIMIZED
)

#register_component()
