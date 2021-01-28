
set (FRAMEBUFFER_DIR source/framebuffer)

set(RAY_TRACER_FILES
    ${RAY_TRACER_FILES}
    ${FRAMEBUFFER_DIR}/RenderTarget.hpp
    ${FRAMEBUFFER_DIR}/RenderTarget.cpp
    ${FRAMEBUFFER_DIR}/Image.hpp
    ${FRAMEBUFFER_DIR}/ImagePNG.cpp
    ${FRAMEBUFFER_DIR}/ImageTGA.cpp
)