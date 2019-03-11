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

# Create the object gathering
add_library(${OBJECT_FILE_NAME} OBJECT
    ${COMPONENT_SRCS}
    )

target_include_directories(${OBJECT_FILE_NAME} PRIVATE
    ${COMPONENT_ADD_INCLUDEDIRS}
    ${COMPONENT_PRIV_INCLUDEDIRS}
    )

target_compile_definitions(${OBJECT_FILE_NAME} PRIVATE
    CONFIG_DSP_OPTIMIZED
)

#register_component()
